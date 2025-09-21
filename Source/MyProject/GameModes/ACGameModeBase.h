// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ACGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AACGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	;
};



