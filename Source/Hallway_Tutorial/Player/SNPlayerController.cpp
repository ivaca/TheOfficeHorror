// Fill out your copyright notice in the Description page of Project Settings.


#include "SNPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/SNCharacter.h"
#include "Character/SNCharacterMovementComponent.h"
#include "Character/SNPlayerCameraManager.h"
#include "Character/SNSpectatorPawnBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpectatorPawn.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "World/SNEquipableItem.h"


ASNPlayerController::ASNPlayerController()
{
	bReplicates = true;
	PlayerCameraManagerClass = ASNPlayerCameraManager::StaticClass();
}

void ASNPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void ASNPlayerController::SetPlayerPlay()
{
	if (!HasAuthority())
	{
		return;
	}

	// Update the state on server
	PlayerState->SetIsSpectator(false);
	ChangeState(NAME_Playing);

	bPlayerIsWaiting = false;

	// Push the state update to the client
	ClientGotoState(NAME_Playing);
}

void ASNPlayerController::SetPlayerSpectate()
{
	// Only proceed if we're on the server
	if (!HasAuthority())
	{
		return;
	}

	// Update the state on server
	PlayerState->SetIsSpectator(true);
	ChangeState(NAME_Spectating);

	bPlayerIsWaiting = true;

	// Push the state update to the client
	ClientGotoState(NAME_Spectating);
}

ASpectatorPawn* ASNPlayerController::SpawnSpectatorPawn()
{
	return Super::SpawnSpectatorPawn();
}

void ASNPlayerController::PlayerDied()
{
	bIsPlayerDead = true;

	if (InventoryWidgetRef)
	{
		InventoryWidgetRef->RemoveFromParent();
		InventoryWidgetRef = nullptr;
		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
		SetIgnoreMoveInput(false);
		SetIgnoreLookInput(false);
	}
}

UEnhancedInputLocalPlayerSubsystem* ASNPlayerController::GetEnhancedInputLocalPlayerSubsystem()
{
	return ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
}

UEnhancedInputComponent* ASNPlayerController::GetUEnhancedInputComponent()
{
	return CastChecked<UEnhancedInputComponent>(InputComponent);
}

void ASNPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(MappingContext);

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	if (SubSystem)
	{
		SubSystem->AddMappingContext(MappingContext, 1);
		if (!SubSystem->GetUserSettings()->IsMappingContextRegistered(MappingContext))
		{
			SubSystem->GetUserSettings()->RegisterInputMappingContext(MappingContext);
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("Register")));
		}
	}
}

void ASNPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();


	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASNPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASNPlayerController::Look);

	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this,
	                                   &ASNPlayerController::BeginInteract);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this,
	                                   &ASNPlayerController::Jump);

	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this
	                                   ,
	                                   &ASNPlayerController::EndInteract);

	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this,
	                                   &ASNPlayerController::Crouch);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this
	                                   ,
	                                   &ASNPlayerController::UnCrouch);


	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this,
	                                   &ASNPlayerController::StartSprint);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this
	                                   ,
	                                   &ASNPlayerController::StopSprint);
	EnhancedInputComponent->BindAction(DropItemAction, ETriggerEvent::Started, this,
	                                   &ASNPlayerController::DropItem);
	EnhancedInputComponent->BindAction(UseItemAction, ETriggerEvent::Started, this,
	                                   &ASNPlayerController::UseItem);
	EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Started, this,
	                                   &ASNPlayerController::ToggleMenu);


	EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this,
	                                   &ASNPlayerController::ToggleInventory);


	EnhancedInputComponent->BindAction<ASNPlayerController, bool>(VoiceChatAction, ETriggerEvent::Started, this,
	                                                              &ASNPlayerController::ToggleVoiceChat, true);

	EnhancedInputComponent->BindAction<ASNPlayerController, bool>(VoiceChatAction, ETriggerEvent::Completed, this,
	                                                              &ASNPlayerController::ToggleVoiceChat, false);


	//SLOTS
	EnhancedInputComponent->BindAction<ASNPlayerController, uint8>(Slot1Action, ETriggerEvent::Started, this,
	                                                               &ASNPlayerController::EquipSlot, 0);

	EnhancedInputComponent->BindAction<ASNPlayerController, uint8>(Slot2Action, ETriggerEvent::Started, this,
	                                                               &ASNPlayerController::EquipSlot, 1);
}


void ASNPlayerController::EquipSlot(const uint8 Slot)
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (ASNCharacter* SNCharacter = Cast<ASNCharacter>(ControlledPawn))
		{
			UE_LOG(LogTemp, Error, TEXT("Trying to equip slot %d"), Slot);
			//SNCharacter->EquipItemOnSlot(Slot);
		}
	}
}


void ASNPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), InputAxisVector.Y);
		ControlledPawn->AddMovementInput(ControlledPawn->GetActorRightVector(), InputAxisVector.X);
		if (InputActionValue.IsNonZero())
		{
			OnPlayerMoveOrLook.Broadcast();
		}
	}
}

void ASNPlayerController::Look(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();


	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerYawInput(InputAxisVector.X * Sensitivity);
		ControlledPawn->AddControllerPitchInput(InputAxisVector.Y * Sensitivity);
		if (InputActionValue.IsNonZero())
		{
			OnPlayerMoveOrLook.Broadcast();
		}
	}
}

void ASNPlayerController::Jump()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (ASNCharacter* SNCharacter = Cast<ASNCharacter>(ControlledPawn))
		{
			SNCharacter->Jump();
		}
	}
}

void ASNPlayerController::Crouch()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (ASNCharacter* SNCharacter = Cast<ASNCharacter>(ControlledPawn))
		{
			if (auto MovementComp = Cast<USNCharacterMovementComponent>(SNCharacter->GetCharacterMovement()))
			{
				MovementComp->SetSprinting(false);
			}

			SNCharacter->Crouch();
			OnCrouch.Broadcast();
		}
	}
}

void ASNPlayerController::UnCrouch()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (ASNCharacter* SNCharacter = Cast<ASNCharacter>(ControlledPawn))
		{
			SNCharacter->UnCrouch();
		}
	}
}

void ASNPlayerController::StartSprint()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (ASNCharacter* SNCharacter = Cast<ASNCharacter>(ControlledPawn))
		{
			if (auto MovementComp = Cast<USNCharacterMovementComponent>(SNCharacter->GetCharacterMovement()))
			{
				if (!SNCharacter->bIsCrouched)
					MovementComp->SetSprinting(true);
			}
		}
	}
}

void ASNPlayerController::StopSprint()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (ASNCharacter* SNCharacter = Cast<ASNCharacter>(ControlledPawn))
		{
			if (auto MovementComp = Cast<USNCharacterMovementComponent>(SNCharacter->GetCharacterMovement()))
			{
				MovementComp->SetSprinting(false);
			}
		}
	}
}

void ASNPlayerController::BeginInteract()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (ASNCharacter* SNCharacter = Cast<ASNCharacter>(ControlledPawn))
		{
			SNCharacter->BeginInteract();
		}
	}
}

void ASNPlayerController::EndInteract()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (ASNCharacter* SNCharacter = Cast<ASNCharacter>(ControlledPawn))
		{
			SNCharacter->EndInteract();
		}
	}
}

void ASNPlayerController::DropItem()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (ASNCharacter* SNCharacter = Cast<ASNCharacter>(ControlledPawn))
		{
			SNCharacter->DropItem(SNCharacter->EquippedItem);
		}
	}
}

void ASNPlayerController::UseItem()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (ASNCharacter* SNCharacter = Cast<ASNCharacter>(ControlledPawn))
		{
			SNCharacter->UseItem(SNCharacter->EquippedItem);
		}
	}
}

void ASNPlayerController::ToggleMenu()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (bIsPlayerDead) return;
		if (MenuWidgetRef)
		{
			MenuWidgetRef->RemoveFromParent();
			MenuWidgetRef = nullptr;
			SetShowMouseCursor(false);
			SetInputMode(FInputModeGameOnly());
			SetIgnoreMoveInput(false);
			SetIgnoreLookInput(false);
		}
		else
		{
			MenuWidgetRef = CreateWidget(GetWorld(), MenuWidget);

			MenuWidgetRef->AddToViewport();
			SetShowMouseCursor(true);
			SetInputMode(FInputModeGameAndUI());
			SetIgnoreMoveInput(true);
			SetIgnoreLookInput(true);
		}
	}
}


void ASNPlayerController::ToggleInventory()
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		if (InventoryWidgetRef)
		{
			InventoryWidgetRef->RemoveFromParent();
			InventoryWidgetRef = nullptr;
			SetShowMouseCursor(false);
			SetInputMode(FInputModeGameOnly());
			SetIgnoreMoveInput(false);
			SetIgnoreLookInput(false);
		}
		else
		{
			InventoryWidgetRef = CreateWidget(GetWorld(), InventoryWidget);

			InventoryWidgetRef->AddToViewport();
			SetShowMouseCursor(true);
			SetInputMode(FInputModeGameAndUI());
			SetIgnoreMoveInput(true);
			SetIgnoreLookInput(true);
		}
	}
}

void ASNPlayerController::ToggleVoiceChat(const bool On)
{
	ConsoleCommand(FString::Printf(TEXT("ToggleSpeaking %d"), On));
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("ToggleSpeaking %d"), On));
}
