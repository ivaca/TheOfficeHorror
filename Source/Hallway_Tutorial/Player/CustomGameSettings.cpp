// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CustomGameSettings.h"

UCustomGameSettings::UCustomGameSettings(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	bEnableMic = true;
	VoiceChatVolume = 1.0f;
}

void UCustomGameSettings::SetEnableMic(bool Value)
{
	bEnableMic = Value;
}

void UCustomGameSettings::SetVoiceChatVolume(float Value)
{
	VoiceChatVolume = Value;
}

bool UCustomGameSettings::GetEnableMic() const
{
	return bEnableMic;
}

float UCustomGameSettings::GetVoiceChatVolume() const
{
	return bEnableMic;
}

UCustomGameSettings* UCustomGameSettings::GetCustomGameSettings()
{
	return Cast<UCustomGameSettings>(UGameUserSettings::GetGameUserSettings());
}
