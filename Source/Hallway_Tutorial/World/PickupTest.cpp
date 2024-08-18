// Fill out your copyright notice in the Description page of Project Settings.


#include "World/PickupTest.h"


#include "Character/SNCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"
#include "Items/Item.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APickupTest::APickupTest()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");

	SetRootComponent(SceneComponent);

	TPPMesh = CreateDefaultSubobject<UStaticMeshComponent>("TPPMesh");
	TPPMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	FPPMesh = CreateDefaultSubobject<UStaticMeshComponent>("FPPMesh");
	FPPMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);


	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>("PickupInteractionComponent");
	InteractionComponent->InteractionNameText = FText::FromString("Pickup");
	InteractionComponent->InteractionDistance = 80.0f;
	InteractionComponent->InteractionActionText = FText::FromString("Take");

	InteractionComponent->SetupAttachment(RootComponent);
	TPPMesh->SetupAttachment(RootComponent);
	FPPMesh->SetupAttachment(RootComponent);

	AttachSocket = FName("GripPoint");

	PrimaryActorTick.TickGroup = TG_PrePhysics;
	SetReplicates(true);
	bNetUseOwnerRelevancy = true;
	SetReplicatingMovement(false);
	SetReplicateMovement(false);
	TPPMesh->SetOwnerNoSee(true);
	FPPMesh->SetOnlyOwnerSee(true);

	TPPMesh->SetIsReplicated(true);
}


void APickupTest::OnRep_Item()
{
	if (Item)
	{
		TPPMesh->SetStaticMesh(Item->PickupMesh);
		InteractionComponent->InteractionNameText = Item->ItemDisplayName;
	}
	InteractionComponent->RefreshWidget();
}


void APickupTest::PlayerPickUpSound_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickUpItemSound, GetActorLocation());
}

void APickupTest::AddedToInventory(UInventoryComponent* InventoryComponent, APickupTest* PickupItem)
{
	if (ASNCharacter* Character = Cast<ASNCharacter>(InventoryComponent->GetOwner()))
	{
		Character->EquipItem(PickupItem);
	}
	/*if (ASNCharacter* Character = Cast<ASNCharacter>(InventoryComponent->GetOwner()))
	{
		if (Character->GetCurrentEquippedItem() == nullptr) //if item NOT in hands
		{
			Character->EquipItem(PickupItem, true);
		}
		else //if some item already in hands
		{
			Character->EquipItem(PickupItem, false);
		}
	}*/
}

// Called when the game starts or when spawned
void APickupTest::BeginPlay()
{
	Super::BeginPlay();


	InteractionComponent->OnInteract.AddDynamic(this, &APickupTest::OnTakePickup);


	if (!bNetStartup)
	{
		AlignWithground();
	}
}

void APickupTest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(APickupTest, Item);
}


// Called every frame
void APickupTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickupTest::DropItem()
{
	TPPMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FPPMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	
	OnDropItem();
	
}


void APickupTest::OnTakePickup(ASNCharacter* ItemTaker)
{
	if (!ItemTaker )
	{
		return;
	}
	if (HasAuthority() && !IsPendingKill()) // If server equip item
	{
		if (UInventoryComponent* PlayerInventory = ItemTaker->PlayerInventory)
		{
			const FItemAddResult AddResult = PlayerInventory->TryAddItem(this);
			switch (AddResult.Result)
			{
			case EItemAddResult::IAR_AllItemsAdded:
				OnItemPicked(ItemTaker);
				PlayerPickUpSound();
				TPPMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				FPPMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				break;
			case EItemAddResult::IAR_NoItemsAdded:

				break;
			}

			//SetOwner(ItemTaker);
		}
	}
	else // if not server align item
	{
	}
}

void APickupTest::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);
	//DOREPLIFETIME_ACTIVE_OVERRIDE(APickupTest, ReplicatedMovement, false);
}
