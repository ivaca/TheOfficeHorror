// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/PickupTest.h"
#include "DoorKey.generated.h"

/**
 * 
 */
UCLASS()
class HALLWAY_TUTORIAL_API ADoorKey : public APickupTest
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Key")
	FText KeyName;
};
