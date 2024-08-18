// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractionComponent.generated.h"


class APickupTest;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginInteract, class ASNCharacter*, Character);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndInteract, class ASNCharacter*, Character);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginFocus, class ASNCharacter*, Character);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndFocus, class ASNCharacter*, Character);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, class ASNCharacter*, Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractClient, class ASNCharacter*, Character);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractWithItem, class ASNCharacter*, Character, class APickupTest*, Item);


/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HALLWAY_TUTORIAL_API UInteractionComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:
	virtual void Deactivate() override;
	bool CanInteract(class ASNCharacter* Character) const;

public:
	UInteractionComponent();

	void RefreshWidget();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	bool bCanInteractWith = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	bool bIsInteractionLocal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	bool bIsItemRequired;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	float InteractionDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	FText InteractionNameText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	FText InteractionActionText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	bool bAllowMultipleInteractors;

	UFUNCTION(BlueprintCallable, Category="Interaction")
	void SetInteractableNameText(const FText& NewNameText);

	UFUNCTION(BlueprintCallable, Category="Interaction")
	void SetInteractableActionText(const FText& NewActionText);

	void BeginFocus(class ASNCharacter* Character);
	void EndFocus(class ASNCharacter* Character);

	void BeginInteract(class ASNCharacter* Character);
	void EndInteract(class ASNCharacter* Character);
	
	UFUNCTION(BlueprintCallable)
	void Interact(class ASNCharacter* Character);
	
	UFUNCTION(BlueprintCallable)
	void Interact_Client(class ASNCharacter* Character);

	UFUNCTION(BlueprintCallable)
	void InteractWithItem(class ASNCharacter* Character, APickupTest* Item);


	UPROPERTY( BlueprintAssignable)
	FOnBeginInteract OnBeginInteract;

	UPROPERTY( BlueprintAssignable)
	FOnEndInteract OnEndInteract;

	UPROPERTY( BlueprintAssignable)
	FOnBeginFocus OnBeginFocus;

	UPROPERTY( BlueprintAssignable)
	FOnEndFocus OnEndFocus;

	UPROPERTY( BlueprintAssignable)
	FOnInteractClient OnInteractClient;
	
	
	UPROPERTY( BlueprintAssignable)
	FOnInteract OnInteract;
	
	UPROPERTY( BlueprintAssignable)
	FOnInteractWithItem OnInteractWithItem;


	
};
