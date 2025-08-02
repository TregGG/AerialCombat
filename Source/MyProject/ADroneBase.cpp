// Fill out your copyright notice in the Description page of Project Settings.


#include "ADroneBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"

AADroneBase::AADroneBase()
{
    PrimaryActorTick.bCanEverTick = true;
    DroneVelocity = FVector::ZeroVector;
    SteerInput = FVector2D::ZeroVector;
	CameraInputStick = FVector2D::ZeroVector;
    CamRotation = FRotator::ZeroRotator;
    DroneVisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneVisualMesh"));
    DroneVisualMesh->AddLocalRotation(DroneMeshOffestRotation);
    DroneVisualMesh->SetupAttachment(RootComponent);
    AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));
    
    //FirePoint
    FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
    FirePoint->SetupAttachment(RootComponent);
  
}

void AADroneBase::BeginPlay()
{
    /*if (AbilityComponentClass) {
        AbilityComponent = NewObject<UAbilityComponent>(this, AbilityComponentClass);
        if (AbilityComponent) {
            AbilityComponent->RegisterComponent();
            AbilityComponent->OnComponentCreated();
            AbilityComponent->InitializeComponent();
            UE_LOG(LogTemp,Log, TEXT("AbilityComponent initialized !!"))
        }
    }*/

}

void AADroneBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
	//Camera Rotation
    CamRotation += FRotator(CameraStats.CameraPitchSpeed*CameraInputStick.Y	, CameraStats.CameraYawSpeed*CameraInputStick.X, 0.f);
    //UE_LOG(LogTemp, Log, TEXT("CamRotation : %s"), *CamRotation.ToString());
    //CamRotation.Yaw = FMath::Abs(CamRotation.Yaw) >= 180 ? CamRotation.Yaw - 360*FMath::Sign(CamRotation.Yaw) : CamRotation.Yaw;
    CamRotation.Pitch = FMath::Clamp(CamRotation.Pitch,  CameraStats.CameraPitchMin, CameraStats.CameraPitchMax);
    CameraBoom->SetWorldRotation(CamRotation);
    // Simple acceleration-based movement

    //Declaration of Variables0
    FVector Forward = GetActorForwardVector();
    FVector Right = GetActorRightVector();
    FVector currentCamForwardVector = CameraBoom->GetForwardVector();
    FRotator currentRotation = GetActorRotation();
	FVector currentCamRightVector = CameraBoom->GetRightVector();


    currentRotation.Yaw = FMath::FInterpTo(currentRotation.Yaw, CamRotation.Yaw, DeltaTime, DroneBuildStats.YawRotationSpeed);
    //UE_LOG(LogTemp, Log, TEXT("CurrentRotation : %s "), *currentRotation.ToString());
    if (FMath::Abs(CamRotation.Pitch) >= DroneBuildStats.VerticalClimbAngleCutOff) {
        currentRotation.Pitch = DroneBuildStats.VerticalClimbAngleCutOff * FMath::Sign(CamRotation.Pitch);
    }
    else {
        currentRotation.Pitch = CamRotation.Pitch;
    }

	
    FVector DesiredMove = (Forward * SteerInput.Y + Right * SteerInput.X).GetClampedToMaxSize(1.0f);
    FVector Acceleration = DesiredMove * DroneBuildStats.Acceleration;

    // Apply drag
    FVector Drag = -DroneVelocity * DroneBuildStats.DragCoeff;

    // Update velocity
    DroneVelocity += (Acceleration + Drag) * DeltaTime;
    DroneVelocity = DroneVelocity.GetClampedToMaxSize(DroneBuildStats.Speed);

    // Move the drone
    FVector NewRotation(currentRotation.Pitch, currentRotation.Yaw, currentRotation.Roll);
	AircraftMovementComp->SetLinearVelocity(DroneVelocity);
    AircraftMovementComp->SetAngularRotation(NewRotation);//Fvector Pitch,Yaw,Roll


    //For Mesh
    //const float ForwardSpeed = FVector::DotProduct(DroneVelocity, Forward);
    //const float RightSpeed = FVector::DotProduct(DroneVelocity, Right);
    float SpeedWiseTilt = FMath::Clamp(FMath::Abs(DroneVelocity.Size()),DroneBuildStats.MinTiltAngle, 90);
    
    TargetVisualMeshRotation.Pitch = FMath::Clamp(-SteerInput.Y * SpeedWiseTilt * 0.1f, -DroneBuildStats.PitchTiltAngle, DroneBuildStats.PitchTiltAngle); // Adjust scale/clamp
    TargetVisualMeshRotation.Roll = FMath::Clamp(SteerInput.X   *SpeedWiseTilt * 0.1f, -DroneBuildStats.RollTiltAngle, DroneBuildStats.RollTiltAngle); // or set it based on yaw/banking
    ////For input=0,0
    if (SteerInput.IsNearlyZero()) {
        TargetVisualMeshRotation = FRotator::ZeroRotator;
    }
    TargetVisualMeshRotation.Pitch -= GetActorRotation().Pitch;
    FQuat TargetVisualMeshRotationQuat = TargetVisualMeshRotation.Quaternion()* DroneMeshOffestRotation.Quaternion()  ;
    // Interpolate smoothly
    const FQuat CurrentVisualRotation = DroneVisualMesh->GetRelativeRotation().Quaternion();
    FQuat NewVisualRotation = FMath::QInterpTo(CurrentVisualRotation, TargetVisualMeshRotationQuat, DeltaTime, 5.0f);
    DroneVisualMesh->SetRelativeRotation(NewVisualRotation);
}

void AADroneBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AADroneBase::CameraInput(FVector2D Input)
{
    CameraInputStick = Input;
}
void AADroneBase::SteerInputHandler(FVector2D Input)
{
    SteerInput = Input;
}