// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DronePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class AADroneBase;
/**
 * 
 */
UCLASS()
class MYPROJECT_API ADronePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ADronePlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta= (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DroneInputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta= (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InputDroneMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InputCameraRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> InputShooting;



	void HandleMoveInput(const FInputActionValue& InputActionValue);
	void HandleMoveInputCompleted(const FInputActionValue& Value);
	void HandleCameraRotation(const FInputActionValue& InputActionValue);
	void HandleCameraRotationCompleted(const FInputActionValue& InputActionValue);

	void HandleShootInput(const FInputActionValue& InputActionValue);


public:
	virtual void SetupInputComponent() override;

};
