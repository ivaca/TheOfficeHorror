// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InteractionComponent.h"
#include "GameFramework/Actor.h"

#include "Pickup.generated.h"

class UItem;

UCLASS()
class HALLWAY_TUTORIAL_API APickup : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickup();

	void InitializePickup(const TSubclassOf<UItem> ItemClass);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
	class UItem* ItemTemplate;

	UFUNCTION(BlueprintImplementableEvent)
	void AlignWithground();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, ReplicatedUsing=OnRep_Item)
	class UItem* Item;

	UFUNCTION()
	void OnRep_Item();


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditDefaultsOnly, Category="Components")
	UInteractionComponent* InteractionComponent;

	UPROPERTY(EditDefaultsOnly, Category = Effects)
	FName AttachSocket;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
	void OnTakePickup(class ASNCharacter* Taker);
};
