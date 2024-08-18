// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Pickup.h"


#include "Character/SNCharacter.h"
#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"
#include "Engine/ActorChannel.h"
#include "Items/Item.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APickup::APickup()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);


	SetRootComponent(PickupMesh);

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>("PickupInteractionComponent");
	InteractionComponent->InteractionNameText = FText::FromString("Pickup");
	InteractionComponent->InteractionDistance = 0.75f;
	InteractionComponent->InteractionActionText = FText::FromString("Take");

	InteractionComponent->SetupAttachment(PickupMesh);

	bReplicates = true;
	AttachSocket = FName("GripPoint");
}

void APickup::InitializePickup(const TSubclassOf<UItem> ItemClass)
{
	if (HasAuthority() && ItemClass)
	{
		Item = NewObject<UItem>(this, ItemClass);
		Item->MarkDirtyForReplication();
		OnRep_Item();
	}
}

void APickup::OnRep_Item()
{
	if (Item)
	{
		PickupMesh->SetStaticMesh(Item->PickupMesh);
		InteractionComponent->InteractionNameText = Item->ItemDisplayName;
	}
	InteractionComponent->RefreshWidget();
}


#if WITH_EDITOR
void APickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property != NULL ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(APickup, ItemTemplate))
	{
		if (ItemTemplate)
		{
			PickupMesh->SetStaticMesh(ItemTemplate->PickupMesh);
		}
	}
}


#endif
// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();


	InteractionComponent->OnInteract.AddDynamic(this, &APickup::OnTakePickup);

	if (ItemTemplate && bNetStartup && HasAuthority())
	{
		InitializePickup(ItemTemplate->GetClass());
	}

	if (!bNetStartup)
	{
		AlignWithground();
	}

	if (Item)
	{
		Item->MarkDirtyForReplication();
	}
}

void APickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickup, Item);
}

bool APickup::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	if (Item && Channel->KeyNeedsToReplicate(Item->GetUniqueID(), Item->RepKey))
	{
		bWroteSomething |= Channel->ReplicateSubobject(Item, *Bunch, *RepFlags);
	}
	return bWroteSomething;
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickup::OnTakePickup(ASNCharacter* Taker)
{
	if (!Taker)
	{
		return;
	}
	if (HasAuthority() && !IsPendingKill() && Item)
	{
		if (UInventoryComponent* PlayerInventory = Taker->PlayerInventory)
		{
			if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("APickup::OnTakePickup"));	
			//const FItemAddResult AddResult = PlayerInventory->TryAddItem(this);
			Destroy();
		}
	}
}
