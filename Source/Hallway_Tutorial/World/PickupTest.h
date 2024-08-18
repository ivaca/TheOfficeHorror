// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InteractionComponent.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"

#include "PickupTest.generated.h"

class UItem;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemDrop);

/*
UENUM(BlueprintType)
enum class EItemType:uint8
{
	NONE = 0 UMETA(DisplayName = "None"),
	FLASHLIGHT = 1 UMETA(DisplayName = "Flashlight"),
	CAMERA = 2 UMETA(DisplayName = "Camera"),
	ELSE = 5 UMETA(DisplayName = "Else"),
};
*/
UENUM(BlueprintType)
enum class EItemType:uint8
{
	INTERACTABLE = 0 UMETA(DisplayName = "INTERACTABLE"),
	PICKABLE = 1 UMETA(DisplayName = "PICKABLE"),
	THROWABLE = 2 UMETA(DisplayName = "THROWABLE")
};

UCLASS()
class HALLWAY_TUTORIAL_API APickupTest : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickupTest();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	class UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	FText ItemDisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item", meta=(MultiLine=true))
	FText ItemDescription;


	UFUNCTION(BlueprintImplementableEvent)
	void AlignWithground();


	UFUNCTION(BlueprintImplementableEvent)
	void Use();


	UFUNCTION(NetMulticast, Reliable)
	void PlayerPickUpSound();

	UFUNCTION(BlueprintImplementableEvent)
	void Use_CLIENT();

	virtual void AddedToInventory(class UInventoryComponent* InventoryComponent, APickupTest* Item);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	EItemType ItemType;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item")
	USoundBase* PickUpItemSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UStaticMeshComponent* TPPMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UStaticMeshComponent* FPPMesh;


	ASNCharacter* Taker;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, ReplicatedUsing=OnRep_Item)
	class UItem* Item;


	UFUNCTION()
	void OnRep_Item();


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Components")
	UInteractionComponent* InteractionComponent;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	FName AttachSocket;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void DropItem();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDropItem();


	UFUNCTION()
	void OnTakePickup(class ASNCharacter* Taker);

	UFUNCTION(BlueprintImplementableEvent)
	void OnItemPicked(ASNCharacter* ItemTaker);

	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
};
