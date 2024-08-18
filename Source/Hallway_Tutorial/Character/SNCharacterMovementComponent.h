// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SNCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class HALLWAY_TUTORIAL_API USNCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	USNCharacterMovementComponent();
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	virtual float GetMaxSpeed() const override;
	uint8 bWantsToSprint : 1;

	UPROPERTY(Category="Character Movement: Sprinting", EditAnywhere, BlueprintReadWrite,
		meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"))
	float SprintSpeed;


	UFUNCTION(Exec, Category="Commands")
	void SetMovementSpeed(uint32 PlayerSpeed);

	
	void SetSprinting(bool Sprint);
};

class FSNSavedMove_Character : public FSavedMove_Character
{
public:
	virtual void Clear() override;
	virtual uint8 GetCompressedFlags() const override;
	virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel,
	                        FNetworkPredictionData_Client_Character& ClientData) override;
	virtual void PrepMoveFor(ACharacter* C) override;
	virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;

	uint8 bSavedWantsToSprint : 1;
};

class FSNNetworkPredictionData_Client_Character : public FNetworkPredictionData_Client_Character
{
public:
	typedef FNetworkPredictionData_Client_Character Super;

	FSNNetworkPredictionData_Client_Character(const UCharacterMovementComponent& CharacterMovementComponent): Super(
		CharacterMovementComponent)
	{
	}

	virtual FSavedMovePtr AllocateNewMove() override;
};
