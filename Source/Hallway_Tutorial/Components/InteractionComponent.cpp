// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InteractionComponent.h"

#include "Character/SNCharacterBase.h"
#include "HUD/InteractionWidget.h"
#include "Net/UnrealNetwork.h"

void UInteractionComponent::Deactivate()
{
	Super::Deactivate();
}

bool UInteractionComponent::CanInteract(ASNCharacter* Character) const
{
	return IsActive() && GetOwner() != nullptr && Character != nullptr;
}

void UInteractionComponent::SetInteractableNameText(const FText& NewNameText)
{
	InteractionNameText = NewNameText;
	RefreshWidget();
}

void UInteractionComponent::SetInteractableActionText(const FText& NewActionText)
{
	InteractionActionText = NewActionText;
	RefreshWidget();
}


void UInteractionComponent::RefreshWidget()
{
	if (!bHiddenInGame)
	{
		if (UInteractionWidget* InteractionWidget = Cast<UInteractionWidget>(GetUserWidgetObject()))
		{
			InteractionWidget->UpdateInteractionWidget(this);
		}
	}
}

UInteractionComponent::UInteractionComponent()
{
	SetComponentTickEnabled(false);


	InteractionDistance = 50.0f;
	InteractionNameText = FText::FromString("Change this");
	bAllowMultipleInteractors = false;

	Space = EWidgetSpace::Screen;
	DrawSize = FIntPoint(400, 100);
	bDrawAtDesiredSize = true;

	SetActive(true);
	SetHiddenInGame(true);
}


void UInteractionComponent::BeginFocus(ASNCharacter* Character)
{
	if (!IsActive() || !GetOwner() || !Character) return;

	OnBeginFocus.Broadcast(Character);
	SetHiddenInGame(false);


	TArray<UPrimitiveComponent*> comps;
	GetOwner()->GetComponents<UPrimitiveComponent>(comps);

	for (auto& VisualComp : comps)
	{
		if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(VisualComp))
		{
			Prim->SetRenderCustomDepth(true);
		}
	}
	RefreshWidget();


	
}


void UInteractionComponent::EndFocus(ASNCharacter* Character)
{
	OnEndFocus.Broadcast(Character);
	SetHiddenInGame(true);

	TArray<UPrimitiveComponent*> comps;
	GetOwner()->GetComponents<UPrimitiveComponent>(comps);

	for (auto& VisualComp : comps)
	{
		if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(VisualComp))
		{
			Prim->SetRenderCustomDepth(false);
		}
	}
}

void UInteractionComponent::BeginInteract(ASNCharacter* Character)
{
	if (!bCanInteractWith)return;
	if (CanInteract(Character))
	{
		OnBeginInteract.Broadcast(Character);
	}
}

void UInteractionComponent::EndInteract(ASNCharacter* Character)
{
	OnEndInteract.Broadcast(Character);
}

void UInteractionComponent::Interact(ASNCharacter* Character)
{
	OnInteract.Broadcast(Character);
}

void UInteractionComponent::Interact_Client(ASNCharacter* Character)
{
	OnInteractClient.Broadcast(Character);
}

void UInteractionComponent::InteractWithItem(ASNCharacter* Character, APickupTest* Item)
{
	OnInteractWithItem.Broadcast(Character, Item);
}
