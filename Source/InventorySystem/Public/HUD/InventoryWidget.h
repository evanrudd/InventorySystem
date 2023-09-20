// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void UpdateInventory(class AItem* Item, int Index);

	FORCEINLINE void SetInventory(class Inventory* NewInventory) { PlayerInventory = NewInventory; }

private:
	UPROPERTY(meta = (BindWidget))
	TArray<class UTextBlock*> TextBlocksArray;

	UPROPERTY(meta = (BindWidget))
	TArray<class UImage*> ImageArray;

	UPROPERTY(meta = (BindWidget))
	class UButton* SortItemTypeButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* SortAmountButton;

	UFUNCTION(BlueprintCallable)
	void SortByAmount();

	UFUNCTION(BlueprintCallable)
	void SortByItemType();

	class Inventory* PlayerInventory;
};
