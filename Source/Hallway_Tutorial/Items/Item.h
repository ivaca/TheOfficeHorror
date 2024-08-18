// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"


/**
 * 
 */


class APickupTest;
class ASNEquipableItem;

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class HALLWAY_TUTORIAL_API UItem : public UObject
{
	GENERATED_BODY()
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override;

public:
	UItem();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item")
	class UStaticMesh* PickupMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item")
	class UTexture2D* Thumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item")
	FText ItemDisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item", meta=(MultiLine=true))
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Item")
	FText UseActionText;

	virtual void Use(class ASNCharacter* Character);
	virtual void AddedToInventory(class UInventoryComponent* InventoryComponent);
	
	virtual void AddedToInventory(class UInventoryComponent* InventoryComponent, APickupTest* Item);
	

	UFUNCTION(BlueprintCallable, Category = "Equippables")
	virtual void Equip(class ASNCharacter* Character);

	UFUNCTION(BlueprintCallable, Category = "Equippables")
	virtual void UnEquip(class ASNCharacter* Character);
	
	UPROPERTY()
	int32 RepKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	TSubclassOf<class ASNEquipableItem> EquipableItemClass;
	
	UPROPERTY()
	class UInventoryComponent* OwningInventory;

	UFUNCTION(BlueprintPure, Category="Item")
	virtual bool ShouldShowInInventory() const;

	void MarkDirtyForReplication();


	
};
