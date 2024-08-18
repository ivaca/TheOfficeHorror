// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SNSpectatorPawnBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

class UEnhancedInputLocalPlayerSubsystem;

void ASNSpectatorPawnBase::BeginPlay()
{
	Super::BeginPlay();
}

void ASNSpectatorPawnBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

}

void ASNSpectatorPawnBase::SetupPlayerInputComponent(UInputComponent* InInputComponent)
{
	Super::SetupPlayerInputComponent(InInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);


	EnhancedInputComponent->BindAction(ViewNextPlayerAction, ETriggerEvent::Started, this,
	                                   &ThisClass::ViewNextPlayer);

	EnhancedInputComponent->BindAction(ViewPrevPlayerAction, ETriggerEvent::Started, this,
	                                   &ThisClass::ViewPrevPlayer);
}

void ASNSpectatorPawnBase::ViewNextPlayer()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("EEE")));
	if (APlayerController* PC = GetController<APlayerController>())
	{
		PC->ServerViewNextPlayer();
	}
}

void ASNSpectatorPawnBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ASNSpectatorPawnBase::ViewPrevPlayer()
{
	if (APlayerController* PC = GetController<APlayerController>())
	{
		PC->ServerViewPrevPlayer();
	}
}
