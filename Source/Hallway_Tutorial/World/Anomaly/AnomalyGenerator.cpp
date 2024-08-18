// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Anomaly/AnomalyGenerator.h"
#include "Anomaly.h"
#include "Algo/RandomShuffle.h"
#include "Kismet/GameplayStatics.h"


void AAnomalyGenerator::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAnomaly::StaticClass(), Anomalies);
		Algo::RandomShuffle(Anomalies);
	}
}

AAnomaly* AAnomalyGenerator::SpawnAnomaly()
{
	checkf(Anomalies.IsValidIndex(AnomalyIndex), TEXT("Anomaly Index Is Invalid!"));

	auto Anomaly = Cast<AAnomaly>(Anomalies[AnomalyIndex]);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, FString::Printf(TEXT("%s"),*Anomaly->GetName() ));
	Anomaly->ActivateAnomaly();
	OnAnomalySpawned.Broadcast();
	bIsAnomalyExists = true;
	++AnomalyIndex;
	return Anomaly;
}
