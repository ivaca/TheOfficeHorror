// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Anomaly/Anomaly.h"

#include "Character/SNCharacter.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"

AAnomaly::AAnomaly()
{
	SetReplicates(true);

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(SceneComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("Audio");
	AudioComponent->SetAutoActivate(false);
	AudioComponent->SetupAttachment(RootComponent);

	ActivateTrigger = CreateDefaultSubobject<UBoxComponent>("Activate Trigger");
	ActivateTrigger->SetupAttachment(RootComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetHiddenInGame(true);

	ActivateTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ActivateTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	SetActorHiddenInGame(true);
}



void AAnomaly::BeginPlay()
{
	Super::BeginPlay();
}

void AAnomaly::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AAnomaly::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAnomaly, bIsEventActivated);
}

void AAnomaly::ActivateAnomaly()
{
	bIsActivated = true;
	SetActorHiddenInGame(false);

	OnActivateAnomaly();


}

void AAnomaly::OnRep_IsEventActivated()
{
	if (bIsEventActivated)
		StartAnomalyEvent();
}


void AAnomaly::DeactivateAnomaly()
{
	bIsActivated = false;
	SetActorHiddenInGame(true);
	OnDeactivateAnomaly();
}
