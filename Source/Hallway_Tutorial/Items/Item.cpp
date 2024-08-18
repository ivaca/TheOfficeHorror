// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

#include "Character/SNCharacter.h"
#include "Components/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "World/SNEquipableItem.h"

#define LOCTEXT_NAMESPACE "Item"

void UItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	
}

bool UItem::IsSupportedForNetworking() const
{
	return true;
}

UItem::UItem()
{
	ItemDisplayName = LOCTEXT("ItemName", "Item");
	UseActionText = LOCTEXT("ItemUseActionText", "Use");
	RepKey = 0;
}

void UItem::Use(ASNCharacter* Character)
{
	if (Character && Character->HasAuthority())
	{
		//Character->EquippedItem->Use(Character);
	}
}

void UItem::AddedToInventory(UInventoryComponent* InventoryComponent)
{
	//If the player looted an item don't equip it
	if (ASNCharacter* Character = Cast<ASNCharacter>(InventoryComponent->GetOwner()))
	{
		if (Character->EquippedItem == nullptr)
		{
			Equip(Character);
			
		}
	}
}
void UItem::AddedToInventory(UInventoryComponent* InventoryComponent, APickupTest* Item)
{
	if (ASNCharacter* Character = Cast<ASNCharacter>(InventoryComponent->GetOwner()))
	{
		if (Character->EquippedItem == nullptr)
		{
			 Character->EquipItem(Item);
		}
	}
}


void UItem::Equip(ASNCharacter* Character)
{
	if (ASNCharacter* Character2 = Cast<ASNCharacter>(GetOuter()))
	{
		if (Character) 
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("UItem::Equip"));
			return Character->EquipItem(this);
		}
	}
}

void UItem::UnEquip(ASNCharacter* Character)
{
}


bool UItem::ShouldShowInInventory() const
{
	return true;
}

void UItem::MarkDirtyForReplication()
{
	++RepKey;

	if (OwningInventory)
	{
		++OwningInventory->ReplicatedItemsKey;
	}
}
#undef LOCTEXT_NAMESPACE
