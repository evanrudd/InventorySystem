// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InventoryHUD.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API AInventoryHUD : public AHUD
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInventoryWidget> InventoryWidgetClass;

	UPROPERTY()
	UInventoryWidget* InventoryWidget;

public:
	FORCEINLINE UInventoryWidget* GetInventoryWidget() const { return InventoryWidget; }

protected:
	virtual void BeginPlay() override;
};
