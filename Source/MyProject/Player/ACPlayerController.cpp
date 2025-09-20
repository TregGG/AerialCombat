// Fill out your copyright notice in the Description page of Project Settings.


#include "ACPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"


void AACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComp->BindAction(IA_Steer, ETriggerEvent::Ongoing, this, &AACPlayerController::ProcessSteer);
		EnhancedInputComp->BindAction(IA_Yaw,   ETriggerEvent::Ongoing, this, &AACPlayerController::ProcessYaw);
		EnhancedInputComp->BindAction(IA_Thrust,ETriggerEvent::Ongoing, this, &AACPlayerController::ProcessThrust);
	}

}


void AACPlayerController::ProcessSteer(const FInputActionValue& Value)
{
	FVector2D Input = Value.Get<FVector2D>();
	OnSteerInput.Broadcast(Input);
}

void AACPlayerController::ProcessYaw(const FInputActionValue& Value)
{
	float Input = Value.Get<float>();
	OnYawInput.Broadcast(Input);
}

void AACPlayerController::ProcessThrust(const FInputActionValue& Value)
{
	float Input = Value.Get<float>();
	OnThrustInput.Broadcast(Input);
}
