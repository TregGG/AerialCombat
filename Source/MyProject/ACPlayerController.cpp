// Fill out your copyright notice in the Description page of Project Settings.


#include "ACPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "AAircraftBase.h"


AACPlayerController::AACPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}
void AACPlayerController::BeginPlay()
{
	Super::BeginPlay();
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer()) {
		// Get the Enhanced Input Local Player Subsystem from the local player
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if(AircraftInputMappingContext) 
            {
                
                // The '0' indicates the priority. Higher numbers mean higher priority.
                Subsystem->AddMappingContext(AircraftInputMappingContext, 0);
                UE_LOG(LogTemp, Warning, TEXT("AACPlayerController: AircraftInputMappingContext Assigned"));
            }
            else
            {
                // Log a warning if the mapping context UPROPERTY was not assigned
                UE_LOG(LogTemp, Warning, TEXT("AACPlayerController: AircraftInputMappingContext not assigned! Input may not work. Please assign it in the Blueprint derived from AACPlayerController."));
            }
        }
        else
        {
            // Log an error if the Enhanced Input Subsystem couldn't be retrieved
            UE_LOG(LogTemp, Error, TEXT("AACPlayerController: UEnhancedInputLocalPlayerSubsystem not found!"));
        }
    }
    else
    {
        // Log an error if the LocalPlayer couldn't be retrieved
        UE_LOG(LogTemp, Error, TEXT("AACPlayerController: LocalPlayer not found!"));
    }

	
}
void AACPlayerController::OnPossess(APawn* InPawn) {
    Super::OnPossess(InPawn);
    ULocalPlayer* LocalPlayer = GetLocalPlayer();
    if (LocalPlayer) {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
            Subsystem->ClearAllMappings();
            if(AircraftInputMappingContext) 
            {
                
                Subsystem->AddMappingContext(AircraftInputMappingContext, 0);
                UE_LOG(LogTemp, Warning, TEXT("AACPlayerController: AircraftInputMappingContext Assigned OnPossess"));

            }
            else {
                UE_LOG(LogTemp, Error, TEXT("Unable to get AircraftInputMappingContext"));
            }
        }
    }
}
void AACPlayerController::SetupInputComponent() {
    Super::SetupInputComponent();
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
        if (InputAircraftSteer) {
            EnhancedInputComponent->BindAction(InputAircraftSteer, ETriggerEvent::Triggered, this, &AACPlayerController::HandleSteerInput);
            EnhancedInputComponent->BindAction(InputAircraftSteer, ETriggerEvent::Completed, this, &AACPlayerController::HandleSteerInputCompleted);
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("Unable to get InputAircraftSteer"));
        }
        if (InputAircraftThrust) {
            EnhancedInputComponent->BindAction(InputAircraftThrust, ETriggerEvent::Triggered, this, &AACPlayerController::HandleThrustInput);
            EnhancedInputComponent->BindAction(InputAircraftThrust, ETriggerEvent::Completed, this, &AACPlayerController::HandleThrustInputCompleted);

        }
        else {
            UE_LOG(LogTemp, Error, TEXT("Unable to get InputAircraftThrust"));
        }
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Unable to get InputComponent from UEnhancedInputComponent"));
    }
}
void AACPlayerController::HandleSteerInput(const FInputActionValue& Value) {
    UE_LOG(LogTemp, Log, TEXT("HandleSteerInput: Called"));
    if (AAAircraftBase* AircraftPawn = Cast<AAAircraftBase>(GetPawn())) {
        FVector2D SteerVector = Value.Get<FVector2D>();
        AircraftPawn->processPitch(SteerVector.Y);
        AircraftPawn->processRoll(SteerVector.X);
        UE_LOG(LogTemp, Log, TEXT("HandleSteerInput: SteerVector = %s"), *SteerVector.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HandleSteerInput: No valid aircraft pawn!"));
    }
}
void AACPlayerController::HandleSteerInputCompleted(const FInputActionValue& Value)
{
    if (AAAircraftBase* AircraftPawn = Cast<AAAircraftBase>(GetPawn()))
    {
        // When steer input is completed (finger lifted), set steer input to zero.
        // This will cause the Steer method on the Pawn to set AngularVelocity to zero.
        AircraftPawn->processPitch(0.0f);
        AircraftPawn->processRoll(0.0f);
    }
}
void AACPlayerController::HandleThrustInput(const FInputActionValue& Value) {
    UE_LOG(LogTemp, Log, TEXT("HandleThrustInput: Called"));
    if (AAAircraftBase* AircraftPawn = Cast<AAAircraftBase>(GetPawn())) {
        float ThrustValue = Value.Get<float>();
        AircraftPawn->ThrustInput = ThrustValue;
        UE_LOG(LogTemp, Log, TEXT("HandleThrustInput: ThrustValue = %f"), ThrustValue);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HandleThrustInput: No valid aircraft pawn!"));
    }
}
void AACPlayerController::HandleThrustInputCompleted(const FInputActionValue& Value) {
    if (AAAircraftBase* AircraftPawn = Cast<AAAircraftBase>(GetPawn())) {
        //float ThrustValue = Value.Get<float>();
        AircraftPawn->ThrustInput = 0.0f;

    }
}

