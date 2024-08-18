// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "World/PickupTest.h"
#include "InventoryComponent.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);


UENUM()
enum class EItemAddResult:uint8
{
	IAR_NoItemsAdded UMETA(DisplayName = "Item Not Added"),
	IAR_AllItemsAdded UMETA(DisplayName = "Item Added"),
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()


	FItemAddResult()
	{
	}
	
	
	UPROPERTY(BlueprintReadOnly, Category="Item Add Result")
	EItemAddResult Result;

	UPROPERTY(BlueprintReadOnly, Category="Item Add Result")
	FText ErrorText;

	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.Result = EItemAddResult::IAR_NoItemsAdded;
		AddedNoneResult.ErrorText = ErrorText;
		return AddedNoneResult;
	}

	static FItemAddResult AddedAll()
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.Result = EItemAddResult::IAR_AllItemsAdded;
		return AddedNoneResult;
	}

	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HALLWAY_TUTORIAL_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	friend class UItem;

public:
	// Sets default values for this component's properties
	UInventoryComponent();

	UFUNCTION(BlueprintCallable, Category="Inventory")
	FItemAddResult TryAddItem(class APickupTest* Item);


	
	int32 ConsumeItem(class APickupTest* Item);
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool RemoveItem(class APickupTest* Item);

	UFUNCTION(BlueprintPure, Category="Inventory")
	bool HasItem(TSubclassOf<UItem> ItemClass) const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	APickupTest* FindItem(APickupTest* Item) const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	APickupTest* GetItemByIndex(uint8 Index) const;
	

	UFUNCTION(BlueprintPure, Category="Inventory")
	FORCEINLINE int32 GetCapacity() const { return Capacity; }

	UFUNCTION(BlueprintPure, Category="Inventory")
	FORCEINLINE TArray<class APickupTest*> GetItems() const { return Items; }

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void SetCapacity(const int32 NewCapacity) ;
		
	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryUpdated OnInventoryUpdated;


	UFUNCTION(Client, Reliable)
	void ClientRefreshInventory();
protected:


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory", meta=(ClampMin=0, ClampMax=200))
	int32 Capacity;

	UPROPERTY(ReplicatedUsing=OnRep_Items, VisibleAnywhere, Category="Inventory")
	TArray<class APickupTest*> Items;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

private:
	
	APickupTest* AddItem(class APickupTest* Item);

	UFUNCTION()
	void OnRep_Items();

	UPROPERTY()
	int32 ReplicatedItemsKey;

	FItemAddResult TryAddItem_Internal(class APickupTest* Item);

};
