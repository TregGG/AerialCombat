#include "ADroneBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DronePlayerState.h"
#include "GameFramework/PlayerController.h"
#include "AircraftMovementComponent.h"

AADroneBase::AADroneBase()
{
    bReplicates = true;
    // DO NOT use SetReplicateMovement(true) because we have custom replication

    PrimaryActorTick.bCanEverTick = true;

    DroneVelocity = FVector::ZeroVector;
    SteerInput = FVector2D::ZeroVector;
    CameraInputStick = FVector2D::ZeroVector;
    CamRotation = FRotator::ZeroRotator;

    // Drone mesh
    DroneVisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneVisualMesh"));
    DroneVisualMesh->AddLocalRotation(DroneMeshOffestRotation);
    DroneVisualMesh->SetupAttachment(RootComponent);

    // Fire point
    FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
    FirePoint->SetupAttachment(RootComponent);

    UE_LOG(LogTemp, Log, TEXT("Drone Constructor: Called"));
}

void AADroneBase::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Log, TEXT("Drone BeginPlay: Called"));
}

void AADroneBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Camera rotation is purely local (not replicated)
    CamRotation += FRotator(CameraStats.CameraPitchSpeed * CameraInputStick.Y,
        CameraStats.CameraYawSpeed * CameraInputStick.X,
        0.f);
    CamRotation.Pitch = FMath::Clamp(CamRotation.Pitch, CameraStats.CameraPitchMin, CameraStats.CameraPitchMax);
    CameraBoom->SetWorldRotation(CamRotation);
}

void AADroneBase::SimulateFlightPhysics(float DeltaTime)
{
    // Get directional vectors
    FVector Forward = GetActorForwardVector();
    FVector Right = GetActorRightVector();

    // Desired movement from input
    FVector DesiredMove = (Forward * SteerInput.Y + Right * SteerInput.X).GetClampedToMaxSize(1.0f);
    FVector Acceleration = DesiredMove * DroneBuildStats.Acceleration;

    // Apply drag
    FVector CurrentVel = AircraftMovementComp->GetLinearVelocity();
    FVector Drag = -CurrentVel * DroneBuildStats.DragCoeff;

    // Update velocity
    FVector NewVelocity = CurrentVel + (Acceleration + Drag) * DeltaTime;
    NewVelocity = NewVelocity.GetClampedToMaxSize(DroneBuildStats.Speed);
    AircraftMovementComp->SetLinearVelocity(NewVelocity);
    UE_LOG(LogTemp, Log, TEXT("Setting linear Velocity- %s"), *NewVelocity.ToString())

    // Angular rotation
    FRotator DesiredRot = GetActorRotation();
    DesiredRot.Yaw = FMath::FInterpTo(DesiredRot.Yaw, CamRotation.Yaw, DeltaTime, DroneBuildStats.YawRotationSpeed);
    DesiredRot.Pitch = FMath::Clamp(CamRotation.Pitch,
        -DroneBuildStats.VerticalClimbAngleCutOff,
        DroneBuildStats.VerticalClimbAngleCutOff);

    AircraftMovementComp->SetAngularRotation(FVector(DesiredRot.Pitch, DesiredRot.Yaw, DesiredRot.Roll));

    // Cache for cosmetics
    DroneVelocity = NewVelocity;

    // Mesh tilt
    UpdateDroneVisualMeshTilt(DeltaTime, NewVelocity);
}

void AADroneBase::UpdateDroneVisualMeshTilt(float DeltaTime, const FVector& CurrVelocity)
{
    float SpeedWiseTilt = FMath::Clamp(FMath::Abs(CurrVelocity.Size()), DroneBuildStats.MinTiltAngle, 90);

    TargetVisualMeshRotation.Pitch = FMath::Clamp(-SteerInput.Y * SpeedWiseTilt * 0.1f,
        -DroneBuildStats.PitchTiltAngle,
        DroneBuildStats.PitchTiltAngle);
    TargetVisualMeshRotation.Roll = FMath::Clamp(SteerInput.X * SpeedWiseTilt * 0.1f,
        -DroneBuildStats.RollTiltAngle,
        DroneBuildStats.RollTiltAngle);

    if (SteerInput.IsNearlyZero())
        TargetVisualMeshRotation = FRotator::ZeroRotator;

    TargetVisualMeshRotation.Pitch -= GetActorRotation().Pitch;

    const FQuat TargetQuat = TargetVisualMeshRotation.Quaternion() * DroneMeshOffestRotation.Quaternion();
    const FQuat CurrentQuat = DroneVisualMesh->GetRelativeRotation().Quaternion();
    FQuat NewQuat = FMath::QInterpTo(CurrentQuat, TargetQuat, DeltaTime, 5.0f);
    DroneVisualMesh->SetRelativeRotation(NewQuat);
}

void AADroneBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    // Bind input to CameraInput/SteerInputHandler here
}

void AADroneBase::CameraInput(FVector2D Input)
{
    CameraInputStick = Input;
}

void AADroneBase::SteerInputHandler(FVector2D Input)
{
    SteerInput = Input;
}

void AADroneBase::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();
    UE_LOG(LogTemp, Log, TEXT("Drone OnRep_PlayerState: Called"));
}

void AADroneBase::InitAbilitySystemComponent()
{
    UE_LOG(LogTemp, Log, TEXT("Drone InitAbilitySystemComponent: Now handled by UAbilityManager"));
}
