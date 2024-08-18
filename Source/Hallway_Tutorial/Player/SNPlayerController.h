// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SNPlayerController.generated.h"


class ASNSpectatorPawnBase;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
/**
 * 
 */
DECLARE_DELEGATE_OneParam(FInputSwitchEquippedSlotDelegate, const uint8);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCrouch);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerMoveOrLook);


UCLASS()
class HALLWAY_TUTORIAL_API ASNPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASNPlayerController();
	virtual void PlayerTick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
	float Sensitivity = 0.7f;

	UPROPERTY(BlueprintAssignable)
	FOnCrouch OnCrouch;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerMoveOrLook OnPlayerMoveOrLook;


	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> InventoryWidget;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> MenuWidget;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASNSpectatorPawnBase> PlayerSpectatorClass;
	/** Set Player to play. Should be called only on server */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetPlayerPlay();

	/** Set Player to spectate. Should be called only on server */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetPlayerSpectate();

	virtual ASpectatorPawn* SpawnSpectatorPawn() override;

	UFUNCTION(BlueprintCallable)
	void PlayerDied();


	UFUNCTION(BlueprintPure)
	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputLocalPlayerSubsystem();

	UFUNCTION(BlueprintPure)
	UEnhancedInputComponent* GetUEnhancedInputComponent();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	bool bIsPlayerDead;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> MappingContext;


	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;


	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> VoiceChatAction;
	
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MenuAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> SprintAction;


	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> DropItemAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> InventoryAction;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> UseItemAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> Slot1Action;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> Slot2Action;
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> Slot3Action;


	UUserWidget* InventoryWidgetRef;
	UUserWidget* MenuWidgetRef;


	UFUNCTION()
	void EquipSlot(const uint8 Slot);


	void Move(const FInputActionValue& InputActionValue);
	void Look(const FInputActionValue& InputActionValue);


	void Jump();

	UFUNCTION()
	void ToggleVoiceChat(const bool On);
	
	void Crouch();
	void UnCrouch();

	void StartSprint();
	void StopSprint();

	void BeginInteract();
	void EndInteract();

	void DropItem();
	void UseItem();
	void ToggleMenu();
};
