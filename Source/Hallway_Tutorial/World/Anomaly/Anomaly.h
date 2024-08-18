// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"

#include "GameFramework/Actor.h"
#include "Anomaly.generated.h"


UCLASS()
class HALLWAY_TUTORIAL_API AAnomaly : public AActor
{
	GENERATED_BODY()

	
public:

	AAnomaly();
	
	UFUNCTION(BlueprintCallable)
	void ActivateAnomaly();

	UFUNCTION(BlueprintCallable)
	void DeactivateAnomaly();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActivated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_IsEventActivated)
	bool bIsEventActivated;

	UFUNCTION()
	void OnRep_IsEventActivated();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPlayerActionRequired;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnIsEventActivated();

	UFUNCTION(BlueprintImplementableEvent)
	void StartAnomalyEvent();

	UFUNCTION(BlueprintImplementableEvent)
	void StopAnomalyEvent();
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldActivateWithoutTrigger;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* ActivateTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bActivateEventWhenAnomalyActivated;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnActivateAnomaly();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeactivateAnomaly();

	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};


