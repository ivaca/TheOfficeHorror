// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "World/PickupTest.h"
#include "SNCharacter.generated.h"

class USpotLightComponent;
class UPointLightComponent;
class UPhysicsHandleComponent;
class USpringArmComponent;
class ASNPlayerController;
class UInteractionComponent;
class ASNEquipableItem;
struct FInputActionValue;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractableFound, EItemType, ItemType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractableLost);



USTRUCT(Blueprintable)
struct FInteractionData
{
	GENERATED_BODY()
	FInteractionData()
	{
		ViewedInteractionComponent = nullptr;
		ViewedInteractionActor = nullptr;
		CurrentInteractable = nullptr;
		LastInteractionCheckTime = 0.0f;
	}

	UPROPERTY(BlueprintReadWrite)
	AActor* CurrentInteractable;
	UPROPERTY(BlueprintReadWrite)
	class UInteractionComponent* ViewedInteractionComponent;

	UPROPERTY(BlueprintReadWrite)
	class UPrimitiveComponent* ViewedInteractionActor;

	UPROPERTY(BlueprintReadWrite)
	class USceneComponent* ViewedInteractionScene;

	UPROPERTY()
	float LastInteractionCheckTime;
};

UCLASS()
class HALLWAY_TUTORIAL_API ASNCharacter : public ASNCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASNCharacter(const FObjectInitializer& ObjectInitializer);


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayerDie();

	UPROPERTY(BlueprintAssignable, Category="Item")
	FOnInteractableFound OnInteractableFound;

	UPROPERTY(BlueprintAssignable, Category="Item")
	FOnInteractableLost OnInteractableLost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Components")
	class UInventoryComponent* PlayerInventory;


	UPROPERTY(EditAnywhere, Category="Camera")
	float ViewPitchMax;
	UPROPERTY(EditAnywhere, Category="Camera")
	float ViewPitchMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hands")
	USkeletalMeshComponent* HandsMesh;
	UPROPERTY(EditAnywhere, Category="Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category="Components")
	USpringArmComponent* SpringCameraSmoothArm;

	UFUNCTION(BlueprintCallable, Category="Item")
	void DropItem(class APickupTest* Item);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDropItem(class APickupTest* Item);


	UFUNCTION(BlueprintCallable, Category="Item")
	void UseItem(class APickupTest* Item);


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpotLightComponent* FlashlightComponent3;

	UFUNCTION
	(Server, Reliable, WithValidation)
	void ServerUseItem(class APickupTest* Item);


	UPROPERTY(EditDefaultsOnly, Category="Item")
	TSubclassOf<class APickup> PickupClass;

	void BeginInteract();
	void EndInteract();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCameraComponent* Camera;

	void EquipItem(UItem* Item);

	void EquipItem(APickupTest* Item, bool PutInHands = false);


	void UnEquipItem();

	UPROPERTY(VisibleAnywhere, Category="Inventory")
	uint8 CurrentSlot = 0;


	void SetupCameraLimits();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_EquippedItem)
	class APickupTest* EquippedItem;


	UPROPERTY(VisibleAnywhere, Replicated)
	class APickupTest* EquippedPickupTest;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE APickupTest* GetCurrentEquippedItem()
	{
		return EquippedItem;
	}

	UFUNCTION(BlueprintImplementableEvent)
	void OnItemEquipped();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	UPhysicsHandleComponent* PhysicsHandleComponent;


	UFUNCTION(Server, Reliable)
	void HasItem(APickupTest* Item);

	UFUNCTION(BlueprintImplementableEvent)
	void OnInteract(ASNCharacter* Character, UInteractionComponent* InteractionComponent);
	
	
	

protected:
	void StartCrouch(const FInputActionValue& Value);
	void StopCrouch(const FInputActionValue& Value);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UFUNCTION()
	void OnRep_EquippedItem();


	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	float InteractionCheckFrequency;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
	float InteractionCheckDistance;


	void PerformInteractionCheck();

	void CouldntFindInteractable();
	void FoundNewInteractable(UInteractionComponent* Interactable);


	void Interact();
	

	
	

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void InteractWithItem(ASNCharacter* Character, APickupTest* Item);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBeginInteract();



	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEndInteract();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(BlueprintReadWrite)
	FInteractionData InteractionData;

	FORCEINLINE class UInteractionComponent* GetInteractable() const
	{
		return InteractionData.ViewedInteractionComponent;
	}

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
