// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ACPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSteerInput, FVector2D, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnYawInput, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThrustInput, float, Value);

UCLASS()
class MYPROJECT_API AACPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* InputMappingContext;

public:
	// MOVEMENT INPUTS
	// Delegates (events other classes can bind to)
	UPROPERTY(BlueprintAssignable, Category="Input")
	FOnSteerInput OnSteerInput;
	UPROPERTY(BlueprintAssignable, Category="Input")
	FOnYawInput OnYawInput;
	UPROPERTY(BlueprintAssignable, Category="Input")
	FOnThrustInput OnThrustInput;
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Thrust;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Steer;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Yaw;
	void ProcessThrust(const FInputActionValue& Value);
	void ProcessSteer(const FInputActionValue& Value);
	void ProcessYaw(const FInputActionValue& Value);
	
	virtual void SetupInputComponent() override;
};
