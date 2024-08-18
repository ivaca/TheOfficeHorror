// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/PickupTest.h"
#include "SNNote.generated.h"

/**
 * 
 */
UCLASS()
class HALLWAY_TUTORIAL_API ASNNote : public APickupTest
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Note")
	UTexture2D* NoteImage;
};
