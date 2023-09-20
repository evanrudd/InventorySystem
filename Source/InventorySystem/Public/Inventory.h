// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"

/**
 * 
 */

enum class SortFilter
{
	SF_Type,
	SF_Amount
};

class INVENTORYSYSTEM_API Inventory
{


public:
	Inventory();
	~Inventory();

	void AddItem(AItem* NewItem);
	void PrintInventory();
	void Sort(SortFilter Filter);
	
private:
	UPROPERTY(EditAnywhere, Category = Inventory)
	TArray<AItem*> Items;

	class UInventoryWidget* InventoryWidget;

	void MergeSort(int32 Low, int32 High);
	void Merge(int32 Low, int32 Middle, int32 High);
public:
	FORCEINLINE void SetInventoryWidget(class UInventoryWidget* NewInventoryWidget) { InventoryWidget = NewInventoryWidget; }
};
