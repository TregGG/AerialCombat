// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ACPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class AAAircraftBase;
/**
 * 
 */
UCLASS()
class MYPROJECT_API AACPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AACPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta= (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> AircraftInputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta= (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InputAircraftSteer;			
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta= (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InputAircraftThrust;

	void HandleSteerInput(const FInputActionValue& InputActionValue);
	void HandleThrustInput(const FInputActionValue& InputActionValue);
	void HandleSteerInputCompleted(const FInputActionValue& Value);
	void HandleThrustInputCompleted(const FInputActionValue& Value);

public :
	virtual void SetupInputComponent() override;

	
};
