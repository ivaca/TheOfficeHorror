// Fill out your copyright notice in the Description page of Project Settings.


#include "World/SNEquipableItem.h"

#include "Character/SNCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASNEquipableItem::ASNEquipableItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->bReceivesDecals = false;
	ItemMesh->SetCollisionObjectType(ECC_WorldDynamic);
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	SetRootComponent(ItemMesh);

	AttachSocket = FName("GripPoint");
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	bReplicates = true;
	bNetUseOwnerRelevancy = true;
}

void ASNEquipableItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASNEquipableItem, Item, COND_InitialOnly);
}

void ASNEquipableItem::Use(ASNCharacter* Character)
{
		OnItemUse.Broadcast();
}

// Called when the game starts or when spawned
void ASNEquipableItem::BeginPlay()
{
	Super::BeginPlay();
}

void ASNEquipableItem::OnEquip()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("AWeapon::OnEquip()"));
	AttachMeshToPawn();
}

void ASNEquipableItem::AttachMeshToPawn()
{
	if (auto PawnOwner = Cast<ASNCharacter>(GetOwner()))
	{
		AttachToComponent(PawnOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocket);
	}
}


// Called every frame
void ASNEquipableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
