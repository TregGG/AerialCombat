// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "Public/AbilityComponent.h"
#include "ACPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AACPlayerState : public APlayerState, public  IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AACPlayerState();

	virtual UAbilityComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY()
	TObjectPtr<UAbilityComponent> AbilitySystemComponent;
};
