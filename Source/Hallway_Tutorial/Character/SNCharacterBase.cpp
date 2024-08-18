// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SNCharacterBase.h"
#include "SNCharacterMovementComponent.h"

class USNCharacterMovementComponent;
// Sets default values


ASNCharacterBase::ASNCharacterBase(const FObjectInitializer& ObjectInitializer): Super(
	ObjectInitializer.SetDefaultSubobjectClass<USNCharacterMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
}



