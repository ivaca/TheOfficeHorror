// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CustomGameSettings.generated.h"

/**
 * 
 */
UCLASS()
class HALLWAY_TUTORIAL_API UCustomGameSettings : public UGameUserSettings
{
	GENERATED_UCLASS_BODY()

public:

	
	
	UFUNCTION(BlueprintCallable)
	void SetEnableMic(bool Value);

	UFUNCTION(BlueprintCallable)
	void SetVoiceChatVolume(float Value);


	UFUNCTION(BlueprintPure)
	bool GetEnableMic() const; 

	UFUNCTION(BlueprintPure)
	float GetVoiceChatVolume() const;


	UFUNCTION(BlueprintCallable)
	static UCustomGameSettings* GetCustomGameSettings();
protected:
	UPROPERTY(Config)
	bool bEnableMic;
	UPROPERTY(Config, meta=(ClampMin=0.0f, ClampMax=5.0f))
	float VoiceChatVolume;
};
