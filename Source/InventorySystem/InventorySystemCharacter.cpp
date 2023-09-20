// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventorySystemCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inventory.h"
#include "Item.h"
#include "HUD/InventoryHUD.h"
#include "HUD/InventoryWidget.h"


//////////////////////////////////////////////////////////////////////////
// AInventorySystemCharacter

AInventorySystemCharacter::AInventorySystemCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	CharacterInventory = new Inventory();
}

AInventorySystemCharacter::~AInventorySystemCharacter()
{
	// Clean up the Inventory instance
	if (CharacterInventory)
	{
		delete CharacterInventory;
		CharacterInventory = nullptr;
	}
}

void AInventorySystemCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		AInventoryHUD* InventoryHUD = Cast<AInventoryHUD>(PlayerController->GetHUD());
		if (InventoryHUD)
		{
			InventoryWidget = InventoryHUD->GetInventoryWidget();
			if (CharacterInventory)
			{
				CharacterInventory->SetInventoryWidget(InventoryWidget);
				InventoryWidget->SetInventory(CharacterInventory);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AInventorySystemCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AInventorySystemCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AInventorySystemCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AInventorySystemCharacter::Look);

		//Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AInventorySystemCharacter::Interact);

		//Inventory
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &AInventorySystemCharacter::ToggleInventory);

	}

}

void AInventorySystemCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && InventoryWidget->GetVisibility() != ESlateVisibility::Visible)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AInventorySystemCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr && InventoryWidget->GetVisibility() != ESlateVisibility::Visible)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AInventorySystemCharacter::Interact(const FInputActionValue& Value)
{
	if (OverlappingItem)
	{
		// Create the debug message using UE_LOG
		FString DebugMessage = FString::Printf(TEXT("Interacted with item: "));
		DebugMessage += OverlappingItem->GetName();
		FColor DebugColor = FColor::Green;
		float DisplayTime = 5.0f; // Duration the message will be displayed on the screen (in seconds)
		GEngine->AddOnScreenDebugMessage(-1, DisplayTime, DebugColor, DebugMessage);
		CharacterInventory->AddItem(OverlappingItem);
	}
	else
	{
		if (GEngine)
		{
			// Create the debug message using UE_LOG
			FString DebugMessage = FString::Printf(TEXT("There is nothing to be interacted with!"));

			// Log the message with the desired verbosity level and category
			UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugMessage);

			// Optionally, you can also print the message on the screen using the GEngine's AddOnScreenDebugMessage function
			FColor DebugColor = FColor::Magenta;
			float DisplayTime = 5.0f; // Duration the message will be displayed on the screen (in seconds)
			GEngine->AddOnScreenDebugMessage(-1, DisplayTime, DebugColor, DebugMessage);
		}
	}
}

void AInventorySystemCharacter::ToggleInventory(const FInputActionValue& Value)
{
	/*CharacterInventory->PrintInventory();*/

	if (InventoryWidget)
	{
		ESlateVisibility Visibility = InventoryWidget->GetVisibility();
		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		if (Visibility == ESlateVisibility::Visible)
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			PlayerController->bShowMouseCursor = false;
			PlayerController->bEnableClickEvents = false;
			PlayerController->bEnableMouseOverEvents = false;
		}
		else
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
			PlayerController->bShowMouseCursor = true;
			PlayerController->bEnableClickEvents = true;
			PlayerController->bEnableMouseOverEvents = true;
		}
	}
}

void AInventorySystemCharacter::Jump()
{
	if (InventoryWidget->GetVisibility() != ESlateVisibility::Visible)
	{
		Super::Jump();
	}
}




