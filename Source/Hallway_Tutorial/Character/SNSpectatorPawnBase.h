// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/SpectatorPawn.h"
#include "SNSpectatorPawnBase.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class HALLWAY_TUTORIAL_API ASNSpectatorPawnBase : public ASpectatorPawn
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
public:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> MappingContext;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ViewNextPlayerAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ViewPrevPlayerAction;
protected:
virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** Move camera to next player */
	void ViewNextPlayer();

	virtual void PossessedBy(AController* NewController) override;
	/** Move camera to previous player */
	void ViewPrevPlayer();
};
