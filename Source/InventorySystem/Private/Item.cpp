// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "InventorySystem/InventorySystemCharacter.h"
#include "Inventory.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	PickupRadius = CreateDefaultSubobject<USphereComponent>(TEXT("PickupRadius"));
	PickupRadius->SetupAttachment(GetRootComponent());
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	PickupRadius->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnRadiusOverlap);
	PickupRadius->OnComponentEndOverlap.AddDynamic(this, &AItem::OnRadiusEndOverlap);
}

void AItem::OnRadiusOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AInventorySystemCharacter* Character = Cast<AInventorySystemCharacter>(OtherActor);
	if (Character)
	{
		//DrawEquipUI();
		Character->SetOverlappingItem(this);
		// Get Reference to Inventory class stored in Character
		// Call function in Inventory to check if character has this item
		// if (character has this item)
		// {
		//		
	}
}

void AItem::OnRadiusEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AInventorySystemCharacter* Character = Cast<AInventorySystemCharacter>(OtherActor);
	if (Character)
	{
		Character->SetOverlappingItem(nullptr);
	}
}

void AItem::IncrementItemCount()
{
	if (IsStackable)
	{
		ItemCount++;
	}
}

void AItem::HideActor(bool Hide)
{
	/*SetActorHiddenInGame(Hide);
	SetActorEnableCollision(!Hide);*/
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

