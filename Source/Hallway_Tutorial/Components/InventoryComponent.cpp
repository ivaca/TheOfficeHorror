// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Engine/ActorChannel.h"
#include "Items/Item.h"
#include "Net/UnrealNetwork.h"
#include "World/PickupTest.h"
#include "World/SNEquipableItem.h"

#define LOCTEXT_NAMESPACE "Inventory"
// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	SetIsReplicatedByDefault(true);
}

FItemAddResult UInventoryComponent::TryAddItem(APickupTest* Item)
{
	return TryAddItem_Internal(Item);
}


int32 UInventoryComponent::ConsumeItem(APickupTest* Item)
{
	if (GetOwner() && GetOwner()->HasAuthority() && Item)
	{
		RemoveItem(Item);
	}
	return 0;
}

bool UInventoryComponent::RemoveItem(APickupTest* Item)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		if (Item)
		{
			Items.RemoveSingle(Item);

			ReplicatedItemsKey++;
			ClientRefreshInventory();
			return true;
		}
	}
	return false;
}

bool UInventoryComponent::HasItem(TSubclassOf<UItem> ItemClass) const
{
	return true;
}

APickupTest* UInventoryComponent::FindItem(APickupTest* Item) const
{
	if (Item)
	{
		for (auto& InvItem : Items)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("2")));
			if (InvItem && InvItem->GetClass() == Item->GetClass())
			{
				return InvItem;
			}
		}
	}
	return nullptr;
}

APickupTest* UInventoryComponent::GetItemByIndex(uint8 Index) const
{
	if (Items.IsValidIndex(Index))
	{
		return Items[Index];
	}
	return nullptr;
}

void UInventoryComponent::SetCapacity(const int32 NewCapacity)
{
	Capacity = NewCapacity;
}

void UInventoryComponent::ClientRefreshInventory_Implementation()
{
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryComponent, Items);
}


APickupTest* UInventoryComponent::AddItem(APickupTest* Item)
{
	if (GetOwner() && GetOwner()->HasAuthority())
	{
		//UItem* NewItem = NewObject<UItem>(GetOwner(), Item->ItemTemplate->GetClass());
		//Item->ItemTemplate->OwningInventory = this;
		Items.Add(Item);
		Item->AddedToInventory(this, Item);
		
		//Item->MarkDirtyForReplication();
		return Item;
	}
	return nullptr;
}

void UInventoryComponent::OnRep_Items()
{
	OnInventoryUpdated.Broadcast();
}

FItemAddResult UInventoryComponent::TryAddItem_Internal(APickupTest* Item)
{
	
	if (GetOwner() && GetOwner()->HasAuthority() && Items.Num()!=Capacity)
	{
		AddItem(Item);
		return FItemAddResult::AddedAll();
	}
	return FItemAddResult::AddedNone(LOCTEXT("InventoryFullText",
	                                         "Couldn't add Item to Inventory!"));
}

#undef LOCTEXT_NAMESPACE
