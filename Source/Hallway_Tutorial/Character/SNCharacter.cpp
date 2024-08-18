// Fill out your copyright notice in the Description page of Project Settings.


#include "SNCharacter.h"

#include "SNCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/Item.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Player/SNPlayerController.h"
#include "World/Pickup.h"
#include "World/SNEquipableItem.h"

ASNCharacter::ASNCharacter(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer.SetDefaultSubobjectClass<USNCharacterMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>("PlayerInventory");
	PlayerInventory->SetCapacity(20);

	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 100.0f;
	GetMesh()->SetOwnerNoSee(true);
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;


	SpringCameraSmoothArm = CreateDefaultSubobject<USpringArmComponent>("SpringSmooth");
	SpringCameraSmoothArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringCameraSmoothArm);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(Camera);

	FlashlightComponent3 = CreateDefaultSubobject<USpotLightComponent>("Flashlight");
	FlashlightComponent3->SetupAttachment(SpringArm);


	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>("HandsMesh");
	HandsMesh->SetupAttachment(SpringArm);


	ViewPitchMax = 80.0f;
	ViewPitchMin = -80.0f;

	
	PhysicsHandleComponent = CreateDefaultSubobject<UPhysicsHandleComponent>("PhysicsHandle");
}


void ASNCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetupCameraLimits();
}

void ASNCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformInteractionCheck();
}

void ASNCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASNCharacter::DropItem(APickupTest* Item)
{
	if (PlayerInventory && Item && PlayerInventory->FindItem(Item))
	{
		if (!HasAuthority())
		{
			ServerDropItem(Item);
			return;
		}

		if (HasAuthority())
		{
			PlayerInventory->ConsumeItem(Item);


			Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			Item->SetActorHiddenInGame(false);

			Item->DropItem();
			Item->AlignWithground();

			/*EquippedItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

			EquippedItem->TPPMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			EquippedItem->FPPMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

			EquippedItem->TPPMesh->AttachToComponent(EquippedItem->SceneComponent,
			                                         FAttachmentTransformRules::KeepWorldTransform);
			EquippedItem->FPPMesh->AttachToComponent(EquippedItem->SceneComponent,
			                                         FAttachmentTransformRules::KeepWorldTransform);


			EquippedItem->DropItem();
			EquippedItem->AlignWithground();
			EquippedItem->SetOwner(nullptr);

			EquippedItem = nullptr;*/
			//APickup* Pickup = GetWorld()->SpawnActor<APickup>(PickupClass, SpawnTransform, SpawnParameters);
			//Pickup->InitializePickup(Item->GetClass());

			//UnEquipItem();
		}
	}
}

void ASNCharacter::UseItem(APickupTest* Item)
{
	if (!Item) return;

	if (!HasAuthority() && Item)
	{
		ServerUseItem(Item);
		EquippedItem->Use_CLIENT();
		return;
	}
	EquippedItem->Use_CLIENT();
	EquippedItem->Use();
}


void ASNCharacter::ServerUseItem_Implementation(APickupTest* Item)
{
	UseItem(Item);
}

bool ASNCharacter::ServerUseItem_Validate(APickupTest* Item)
{
	return true;
}

void ASNCharacter::ServerDropItem_Implementation(APickupTest* Item)
{
	DropItem(Item);
}

bool ASNCharacter::ServerDropItem_Validate(APickupTest* Item)
{
	return true;
}


void ASNCharacter::StartCrouch(const FInputActionValue& Value)
{
	Crouch();
}

void ASNCharacter::StopCrouch(const FInputActionValue& Value)
{
	UnCrouch();
}

void ASNCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASNCharacter, EquippedItem);
}

void ASNCharacter::OnRep_EquippedItem()
{
	//AlignItemInHands();


	if (!EquippedItem) return;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("OnRep_EquippedItem")));
	EquippedItem->TPPMesh->AttachToComponent(
		GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		"GripPoint");


	if (PlayerInventory->FindItem(EquippedItem))
	{
		EquippedItem->SetOwner(this);
		EquippedItem->FPPMesh->AttachToComponent(
			HandsMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			"GripPoint");
		EquippedItem->OnItemPicked(this);
	}
	else if (EquippedItem->GetOwner())
	{
		EquippedItem->OnItemPicked(nullptr);
	}


	//OnItemEquipped();
}


void ASNCharacter::SetupCameraLimits()
{
	auto CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	CameraManager->ViewPitchMax = ViewPitchMax;
	CameraManager->ViewPitchMin = ViewPitchMin;
}


void ASNCharacter::PerformInteractionCheck()
{
	if (GetController() == nullptr)return;

	FVector EyesLoc;
	FRotator EyesRot;

	GetController()->GetPlayerViewPoint(EyesLoc, EyesRot);

	FVector TraceStart = EyesLoc;
	FVector TraceEnd = (EyesRot.Vector() * InteractionCheckDistance) + TraceStart;

	FHitResult TraceHit;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		if (TraceHit.GetActor())
		{
			if (UInteractionComponent* InteractionComponent = Cast<UInteractionComponent>(
				TraceHit.GetActor()->GetComponentByClass(UInteractionComponent::StaticClass())))
			{
				
				float Distance = (TraceStart - TraceHit.ImpactPoint).Size();
				if (Distance <= InteractionComponent->InteractionDistance)
				{
					InteractionData.ViewedInteractionActor = TraceHit.GetComponent();
					InteractionData.ViewedInteractionScene = TraceHit.GetComponent();
					InteractionData.CurrentInteractable = TraceHit.GetActor();
					FoundNewInteractable(InteractionComponent);

					
				}
				else if (Distance > InteractionComponent->InteractionDistance && GetInteractable())
				{
					CouldntFindInteractable();
				}
				return;
			}
			else if (TraceHit.GetActor()->ActorHasTag("PhysicsItem"))
			{
				OnInteractableFound.Broadcast(EItemType::THROWABLE);
				InteractionData.ViewedInteractionScene = TraceHit.GetComponent();
				return;
			}
		}
	}
	CouldntFindInteractable();
}

void ASNCharacter::CouldntFindInteractable()
{
	if (UInteractionComponent* Interactable = GetInteractable())
	{
		Interactable->EndFocus(this);
		OnInteractableLost.Broadcast();
	}
	if (InteractionData.ViewedInteractionScene)
	{
		OnInteractableLost.Broadcast();
	}

	InteractionData.ViewedInteractionScene = nullptr;			
	InteractionData.ViewedInteractionComponent = nullptr;
	InteractionData.ViewedInteractionActor = nullptr;
}


void ASNCharacter::FoundNewInteractable(UInteractionComponent* Interactable)
{
	if (UInteractionComponent* OldInteractable = GetInteractable())
	{
		OldInteractable->EndFocus(this);
	}
	if (Interactable != GetInteractable())
	{
		if (IsValid(InteractionData.ViewedInteractionScene) &&
			IsValid(InteractionData.ViewedInteractionScene->GetAttachParentActor()) && InteractionData.
			ViewedInteractionScene->GetAttachParentActor()->ActorHasTag("Note"))
		{
			OnInteractableFound.Broadcast(EItemType::INTERACTABLE);
		}
		else
		{
			OnInteractableFound.Broadcast(EItemType::PICKABLE);
		}
	}


	InteractionData.ViewedInteractionComponent = Interactable;
	Interactable->BeginFocus(this);
}


bool ASNCharacter::ServerBeginInteract_Validate()
{
	//
	return true;
}

void ASNCharacter::ServerEndInteract_Implementation()
{
	EndInteract();
}

bool ASNCharacter::ServerEndInteract_Validate()
{
	return true;
}

void ASNCharacter::Interact()
{
	if (UInteractionComponent* InteractionComponent = GetInteractable())
	{
		InteractionComponent->Interact(this);
	}
}

void ASNCharacter::InteractWithItem_Implementation(ASNCharacter* Character, APickupTest* Item)
{
	if (UInteractionComponent* InteractionComponent = GetInteractable())
	{
		InteractionComponent->BeginInteract(this);
		InteractionComponent->InteractWithItem(Character, Item);
		OnInteract(this, InteractionComponent);
	}
}

void ASNCharacter::BeginInteract()
{
	UInteractionComponent* Interactable = GetInteractable();
	if (Interactable && Interactable->bIsItemRequired)
	{
		Cast<ASNPlayerController>(GetController())->ToggleInventory();
		return;
	}
	if (InteractionData.ViewedInteractionScene && InteractionData.ViewedInteractionScene->GetAttachParentActor())
	{
		if (InteractionData.ViewedInteractionScene->GetAttachParentActor()->ActorHasTag("Note") && Interactable)
		{
			Interactable->BeginInteract(this);
			return;
		}
	}
	if (Interactable && Interactable->bIsInteractionLocal && IsLocallyControlled())
	{
		Interactable->Interact_Client(this);
		
	}

	if (!HasAuthority())
	{
		ServerBeginInteract();
		
		return;
	}
	if (HasAuthority())
	{
		PerformInteractionCheck();
		
	}
	if (Interactable)
	{
		Interactable->BeginInteract(this);
		Interact();
		OnInteract(this, Interactable);
	}
}

void ASNCharacter::ServerBeginInteract_Implementation()
{
	BeginInteract();
}

void ASNCharacter::EndInteract()
{
	if (!HasAuthority())
	{
		ServerEndInteract();
	}


	if (UInteractionComponent* Interactable = GetInteractable())
	{
		Interactable->EndInteract(this);
	}
}

void ASNCharacter::EquipItem(UItem* Item)
{
	/*if (Item && Item->EquipableItemClass && HasAuthority())
	{
		if (EquippedItem)
		{
			if (HasAuthority() && EquippedItem)
			{
				EquippedItem->Destroy();
				EquippedItem = nullptr;
				OnRep_EquippedItem();
			}
		}

		//Spawn the weapon in
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.Owner = SpawnParams.Instigator = this;

		if (ASNEquipableItem* ItemInstanced = GetWorld()->SpawnActor<
			ASNEquipableItem>(Item->EquipableItemClass, SpawnParams))
		{
			ItemInstanced->Item = It		//EquippedItem = ItemInstanced;em;
	
			OnRep_EquippedItem();
			ItemInstanced->OnEquip();
		}
	}*/
}


void ASNCharacter::EquipItem(APickupTest* Item, bool PutInHands)
{
	Item->AttachToComponent(SpringArm, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Item->SetActorHiddenInGame(true);

	//	OnRep_EquippedItem();
	/*if (!PutInHands)
	{
		Item->AttachToComponent(SpringArm, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Item->SetActorHiddenInGame(true);
	}
	else
	{
		if (Item && HasAuthority())
		{
			if (EquippedItem)
			{
				if (HasAuthority() && EquippedItem)
				{
					///EquippedItem->Destroy();
					///EquippedItem = nullptr;
					//OnRep_EquippedItem();
				}
				EquippedItem->SetActorHiddenInGame(true);
			}


			EquippedItem = Item;
			EquippedItem->SetActorHiddenInGame(false);

			OnRep_EquippedItem();
		}
	}*/
}


void ASNCharacter::HasItem_Implementation(APickupTest* Item)
{
	if (PlayerInventory->FindItem(EquippedItem))
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Server has item"));
	}
}

void ASNCharacter::UnEquipItem()
{
	if (HasAuthority() && EquippedItem)
	{
		//EquippedItem->OnUnEquip();
		//EquippedItem->Destroy();
		//EquippedItem = nullptr;
		OnRep_EquippedItem();
	}

	if (EquippedItem)
	{
		//EquippedItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//EquippedItem->AlignWithground();
	}
}
