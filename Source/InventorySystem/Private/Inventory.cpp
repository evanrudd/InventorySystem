// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "Item.h"
#include "HUD/InventoryWidget.h"

Inventory::Inventory()
{
	Items.Empty();
}

Inventory::~Inventory()
{
	Items.Empty();
}

void Inventory::AddItem(AItem* NewItem)
{
	if (NewItem)
	{
		if (NewItem->GetIsStackable())
		{
			FString NewItemIdentifier = NewItem->GetItemIdentifier();
			for (int i = 0; i < Items.Num(); i++)
			{
				if (Items[i]->GetItemIdentifier() == NewItemIdentifier)
				{
					Items[i]->IncrementItemCount();
					NewItem->Destroy();
					InventoryWidget->UpdateInventory(Items[i], i);
					return;
				}
			}
		}
		Items.Add(NewItem);
		InventoryWidget->UpdateInventory(NewItem, Items.Num() - 1);
		NewItem->HideActor(true);
	}
}

void Inventory::PrintInventory()
{
	FString DebugMessage = FString::Printf(TEXT("Items: "));

	for (AItem* AnItem : Items)
	{
		DebugMessage += AnItem->GetName();
		DebugMessage += FString::Printf(TEXT(": "));
		DebugMessage += FString::FromInt(AnItem->GetItemCount());
		DebugMessage += FString::Printf(TEXT(", "));
	}

	FColor DebugColor = FColor::Silver;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, DebugColor, DebugMessage);
}

void Inventory::Sort(SortFilter Filter)
{
	switch (Filter)
	{
	case SortFilter::SF_Type:
		break;
	case SortFilter::SF_Amount:
		if (Items.Num() > 1)
		{
			MergeSort(0, Items.Num() - 1);

			if (InventoryWidget)
			{
				for (int i = 0; i < Items.Num(); i++)
				{
					InventoryWidget->UpdateInventory(Items[i], i);
				}
			}
		}
		break;
	default:
		break;
	}
}

// MergeSort function to sort the Items array based on item count
void Inventory::MergeSort(int32 Low, int32 High)
{
	if (Low < High)
	{
		int32 Middle = (Low + High) / 2;

		// Recursively sort the left and right sub-arrays
		MergeSort(Low, Middle);
		MergeSort(Middle + 1, High);

		// Merge the two sorted sub-arrays
		Merge(Low, Middle, High);
	}
}

// Merge function to merge two sorted sub-arrays
void Inventory::Merge(int32 Low, int32 Middle, int32 High)
{
	int32 LeftSize = Middle - Low + 1;
	int32 RightSize = High - Middle;

	// Create temporary arrays to hold the left and right sub-arrays
	TArray<AItem*> LeftArray;
	TArray<AItem*> RightArray;

	for (int32 i = 0; i < LeftSize; i++)
	{
		LeftArray.Add(Items[Low + i]);
	}

	for (int32 i = 0; i < RightSize; i++)
	{
		RightArray.Add(Items[Middle + 1 + i]);
	}

	int32 LeftIndex = 0;
	int32 RightIndex = 0;
	int32 MergedIndex = Low;

	// Merge the left and right sub-arrays into the original array
	while (LeftIndex < LeftSize && RightIndex < RightSize)
	{
		if (LeftArray[LeftIndex]->GetItemCount() >= RightArray[RightIndex]->GetItemCount())
		{
			Items[MergedIndex] = LeftArray[LeftIndex];
			LeftIndex++;
		}
		else
		{
			Items[MergedIndex] = RightArray[RightIndex];
			RightIndex++;
		}
		MergedIndex++;
	}

	// Copy any remaining elements from the left and right sub-arrays
	while (LeftIndex < LeftSize)
	{
		Items[MergedIndex] = LeftArray[LeftIndex];
		LeftIndex++;
		MergedIndex++;
	}

	while (RightIndex < RightSize)
	{
		Items[MergedIndex] = RightArray[RightIndex];
		RightIndex++;
		MergedIndex++;
	}
}