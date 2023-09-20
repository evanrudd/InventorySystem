// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class INVENTORYSYSTEM_API AItem : public AActor
{
	GENERATED_BODY()

public:
	enum class ItemTypes
	{
		IT_Material,
		IT_Weapon,
		IT_Armor
	};


public:	
	AItem();
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE bool GetIsStackable() const { return IsStackable; }
	FORCEINLINE FString GetItemIdentifier() const { return ItemIdentifier; }
	FORCEINLINE int32 GetItemCount() const { return ItemCount; }
	FORCEINLINE class UTexture2D* GetItemImage() const { return ItemImage; }
	void IncrementItemCount();
	void HideActor(bool Hide);

protected:
	virtual void BeginPlay() override;
	
	bool IsStackable;
	FString ItemIdentifier;
	ItemTypes ItemType;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemMesh, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ItemMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* PickupRadius;
	UPROPERTY(EditDefaultsOnly, Category = Image, meta = (AllowPrivateAccess = "true"))
	class UTexture2D* ItemImage;

	int32 ItemCount = 1;

	UFUNCTION()
	void OnRadiusOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnRadiusEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
