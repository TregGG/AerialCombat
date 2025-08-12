// Fill out your copyright notice in the Description page of Project Settings.


#include "DronePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "ADroneBase.h"
#include "AbilityManager.h"

ADronePlayerController::ADronePlayerController()
{
    PrimaryActorTick.bCanEverTick = true;

}

void ADronePlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) {
        // Get the Enhanced Input Local Player Subsystem from the local player
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (DroneInputMappingContext)
            {

                // The '0' indicates the priority. Higher numbers mean higher priority.
                Subsystem->AddMappingContext(DroneInputMappingContext, 0);
                UE_LOG(LogTemp, Warning, TEXT("ADronePlayerController: AircraftInputMappingContext Assigned"));
            }
            else
            {
                // Log a warning if the mapping context UPROPERTY was not assigned
                UE_LOG(LogTemp, Warning, TEXT("ADronePlayerController: AircraftInputMappingContext not assigned! Input may not work. Please assign it in the Blueprint derived from ADronePlayerController."));
            }
        }
        else
        {
            // Log an error if the Enhanced Input Subsystem couldn't be retrieved
            UE_LOG(LogTemp, Error, TEXT("ADronePlayerController: UEnhancedInputLocalPlayerSubsystem not found!"));
        }
    }
    else
    {
        // Log an error if the LocalPlayer couldn't be retrieved
        UE_LOG(LogTemp, Error, TEXT("ADronePlayerController: LocalPlayer not found!"));
    }
}

void ADronePlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    ULocalPlayer* LocalPlayer = GetLocalPlayer();
    if (LocalPlayer) {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) 
            {
                Subsystem->ClearAllMappings();
                if (DroneInputMappingContext) {
                    Subsystem->AddMappingContext(DroneInputMappingContext, 0);

                }
            }
        
    }
}

void ADronePlayerController::HandleMoveInput(const FInputActionValue& InputActionValue)
{
    UE_LOG(LogTemp, Log, TEXT("HandleMoveInput: Called"));
    if (AADroneBase* DronePawn = Cast<AADroneBase>(GetPawn())) {
        FVector2D SteerVector = InputActionValue.Get<FVector2D>();
        DronePawn->SteerInputHandler(SteerVector);
        UE_LOG(LogTemp, Log, TEXT("HandleMoveInput: SteerVector = %s"), *SteerVector.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HandleMoveInput: No valid drone pawn!"));
    }
}

void ADronePlayerController::HandleMoveInputCompleted(const FInputActionValue& Value)
{
    if (AADroneBase* DronePawn = Cast<AADroneBase>(GetPawn())) {
        FVector2D SteerVector = FVector2D::ZeroVector;
        DronePawn->SteerInputHandler(SteerVector);
    }
}

void ADronePlayerController::HandleCameraRotation(const FInputActionValue& InputActionValue)
{
    UE_LOG(LogTemp, Log, TEXT("HandleCameraRotation: Called"));
    if (AADroneBase* DronePawn = Cast<AADroneBase>(GetPawn())) {
        FVector2D RotationVector = InputActionValue.Get<FVector2D>();
        DronePawn->CameraInput(RotationVector);
        UE_LOG(LogTemp, Log, TEXT("HandleCameraRotation: RotationVector = %s"), *RotationVector.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HandleCameraRotation: No valid drone pawn!"));
    }
}

void ADronePlayerController::HandleCameraRotationCompleted(const FInputActionValue& InputActionValue)
{
    if (AADroneBase* DronePawn = Cast<AADroneBase>(GetPawn())) {
        FVector2D RotationVector = FVector2D::ZeroVector;
        DronePawn->CameraInput(RotationVector);
    }
}

void ADronePlayerController::HandleShootInput(const FInputActionValue& InputActionValue)
{
    if (InputActionValue.Get<bool>()) {
        if (AADroneBase* DronePawn = Cast<AADroneBase>(GetPawn())) {
            if (DronePawn->AbilityManager)
            {
                DronePawn->AbilityManager->UseShoot();
                UE_LOG(LogTemp, Log, TEXT("HandleShootInput: Called shoot ability via GAS"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("HandleShootInput: AbilityManager is nullptr!"));
            }
        }
    }
}

void ADronePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (UEnhancedInputComponent* EnchancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
        if (InputDroneMove) {
            EnchancedInputComponent->BindAction(InputDroneMove, ETriggerEvent::Triggered, this, &ADronePlayerController::HandleMoveInput);
            EnchancedInputComponent->BindAction(InputDroneMove, ETriggerEvent::Completed, this, &ADronePlayerController::HandleMoveInputCompleted);
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("Unable to get InputdroneMovement"));
        }
        if (InputCameraRotation) {
            EnchancedInputComponent->BindAction(InputCameraRotation, ETriggerEvent::Triggered, this, &ADronePlayerController::HandleCameraRotation);
            EnchancedInputComponent->BindAction(InputCameraRotation, ETriggerEvent::Completed, this, &ADronePlayerController::HandleCameraRotation);
            //UE_LOG(LogTemp, Error, TEXT("Was Able to get InputCameraRotation"));

        }
        if (InputShooting) {
            //EnchancedInputComponent->BindAction(InputShooting, ETriggerEvent::Triggered, this, &ADronePlayerController::HandleShootInput);
        }
    }
}




