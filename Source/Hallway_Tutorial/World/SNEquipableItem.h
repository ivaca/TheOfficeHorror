// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SNEquipableItem.generated.h"

class ASNCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemUse);
UCLASS()
class HALLWAY_TUTORIAL_API ASNEquipableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASNEquipableItem();

	virtual void OnEquip();
	void AttachMeshToPawn();

	UPROPERTY(EditDefaultsOnly)
	FName AttachSocket;

	UPROPERTY(EditAnywhere, Category = Components)
	UStaticMeshComponent* ItemMesh;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated, BlueprintReadOnly, Transient)
	class UItem* Item;


	
	void Use(ASNCharacter* Character);

	UPROPERTY(BlueprintAssignable)
	FOnItemUse OnItemUse;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
