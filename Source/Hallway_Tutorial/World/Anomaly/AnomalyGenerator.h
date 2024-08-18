// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AnomalyGenerator.generated.h"

class AAnomaly;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnomalySpawned);


UCLASS()
class HALLWAY_TUTORIAL_API AAnomalyGenerator : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	AAnomaly* SpawnAnomaly();



	
	TArray<AActor*> Anomalies;

	uint8 AnomalyIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAnomalyExists;

	UPROPERTY(BlueprintAssignable)
	FOnAnomalySpawned OnAnomalySpawned;

protected:
	virtual void BeginPlay() override;
};
