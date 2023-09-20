// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventoryWidget.h"
#include "Item.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Inventory.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (int32 i = 0; i < 20; i++)
	{
		FName TextBlockWidgetName = FName(*FString::Printf(TEXT("TextBlock%d"), i));
		UTextBlock* TextBlock = Cast<UTextBlock>(WidgetTree->FindWidget(TextBlockWidgetName));
		if (TextBlock)
		{
			TextBlocksArray.Add(TextBlock);
		}

		FName ImageWidgetName = FName(*FString::Printf(TEXT("Image%d"), i));
		UImage* Image = Cast<UImage>(WidgetTree->FindWidget(ImageWidgetName));
		if (Image)
		{
			ImageArray.Add(Image);
		}
	}
}

void UInventoryWidget::UpdateInventory(AItem* Item, int Index)
{	
	FText Amount = FText::FromString(FString::FromInt(Item->GetItemCount()));
	TextBlocksArray[Index]->SetText(Amount);
	ImageArray[Index]->SetBrushFromTexture(Item->GetItemImage());
}

void UInventoryWidget::SortByAmount()
{
	PlayerInventory->Sort(SortFilter::SF_Amount);

	FString DebugMessage = FString::Printf(TEXT("Sorted by Amount"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, DebugMessage);
}

void UInventoryWidget::SortByItemType()
{
	PlayerInventory->Sort(SortFilter::SF_Type);

	FString DebugMessage = FString::Printf(TEXT("Sorted by Item Type"));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, DebugMessage);
}
