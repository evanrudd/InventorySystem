// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventoryHUD.h"
#include "HUD/InventoryWidget.h"

void AInventoryHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && InventoryWidgetClass)
		{
			InventoryWidget = CreateWidget<UInventoryWidget>(Controller, InventoryWidgetClass);
			InventoryWidget->AddToViewport();
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
