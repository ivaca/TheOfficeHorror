// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SNCharacterMovementComponent.h"

#include "GameFramework/Character.h"

USNCharacterMovementComponent::USNCharacterMovementComponent()
{
	SprintSpeed = 300.0f;
}

void USNCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);
	bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
}

FNetworkPredictionData_Client* USNCharacterMovementComponent::GetPredictionData_Client() const
{
	if (!ClientPredictionData)
	{
		if (USNCharacterMovementComponent* Temp = const_cast<USNCharacterMovementComponent*>(this))
		{
			Temp->ClientPredictionData = new FSNNetworkPredictionData_Client_Character(*this);
		}
	}
	return ClientPredictionData;
}

float USNCharacterMovementComponent::GetMaxSpeed() const
{
	float Speed = Super::GetMaxSpeed();

	if (bWantsToSprint)
	{
		Speed = SprintSpeed;
	}
	return Speed;
}

void USNCharacterMovementComponent::SetMovementSpeed(uint32 PlayerSpeed)
{
	SprintSpeed = PlayerSpeed;
}


void USNCharacterMovementComponent::SetSprinting(bool Sprint)
{
	bWantsToSprint = Sprint;
}

void FSNSavedMove_Character::Clear()
{
	FSavedMove_Character::Clear();
	bSavedWantsToSprint = false;
}

uint8 FSNSavedMove_Character::GetCompressedFlags() const
{
	uint8 Result = FSavedMove_Character::GetCompressedFlags();

	if (bSavedWantsToSprint)
	{
		Result |= FLAG_Custom_0;
	}
	return Result;
}

void FSNSavedMove_Character::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel,
                                        FNetworkPredictionData_Client_Character& ClientData)
{
	FSavedMove_Character::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	if (C)
	{
		if (USNCharacterMovementComponent* MovementComponent = Cast<USNCharacterMovementComponent>(
			C->GetMovementComponent()))
		{
			bSavedWantsToSprint = MovementComponent->bWantsToSprint;
		}
	}
}

void FSNSavedMove_Character::PrepMoveFor(ACharacter* C)
{
	FSavedMove_Character::PrepMoveFor(C);
	if (C)
	{
		if (USNCharacterMovementComponent* MovementComponent = Cast<USNCharacterMovementComponent>(
			C->GetMovementComponent()))
		{
			MovementComponent->bWantsToSprint = bSavedWantsToSprint;
		}
	}
}

bool FSNSavedMove_Character::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const
{
	if (bSavedWantsToSprint != ((FSNSavedMove_Character*)&NewMove)->bSavedWantsToSprint)
	{
		return false;
	}
	return FSavedMove_Character::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

FSavedMovePtr FSNNetworkPredictionData_Client_Character::AllocateNewMove()
{
	return FSavedMovePtr(new FSNSavedMove_Character());
}
