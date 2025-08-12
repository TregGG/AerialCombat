// Fill out your copyright notice in the Description page of Project Settings.


#include "ACPlayerState.h"
//#include "Public/AbilityComponent.h"

AACPlayerState::AACPlayerState()
{
	// Create the Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilitySystemComponent"));
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetIsReplicated(true);
	}
}

UAbilityComponent* AACPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
