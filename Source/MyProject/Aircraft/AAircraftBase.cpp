// Fill out your copyright notice in the Description page of Project Settings.


#include "AAircraftBase.h"

#include "MovieSceneTracksComponentTypes.h"
#include "Misc/LowLevelTestAdapter.h"

// Sets default values
AAAircraftBase::AAAircraftBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    SetAutonomousProxy(true);
    
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    ConstructPlaneMesh();
    MoveComp = CreateDefaultSubobject<UFPVMovementComponent>(TEXT("MoveComp"));

}



void AAAircraftBase::CalculateAerialPhysics(float DeltaTime, FVector& OutLinearAcceleration, FVector& OutAngularVelocity)
{
    OutLinearAcceleration = FVector::ZeroVector;
    OutAngularVelocity    = FVector::ZeroVector;

    switch (FlightType)
    {
        case EFlightType::Aircraft:
        {
            const auto& Cfg = AircraftConfig;

            FVector Forward = GetActorForwardVector();
            FVector Up      = GetActorUpVector();
            FVector Vel     = MoveComp->LastLinearVelocity;
            FVector VelDir  = Vel.GetSafeNormal();

            // --- Gravity ---
            FVector Gravity = FVector(0, 0, -980.f * Cfg.GravityScale) * Cfg.Mass;

            // --- Angle of Attack ---
            float AOA = 0.f;
            if (!VelDir.IsNearlyZero())
            {
                AOA = FMath::Acos(FVector::DotProduct(Forward, VelDir));
                if (FVector::DotProduct(Vel, Up) < 0) AOA *= -1.f;
            }

            // --- Lift ---
            FVector LiftDir = (Up - FVector::DotProduct(Up, VelDir) * VelDir).GetSafeNormal();
            FVector Lift    = 0.5f * Vel.SizeSquared() * Cfg.LiftCoefficient * LiftDir;

            // --- Drag ---
            FVector Drag = -0.5f * Vel.SizeSquared() * Cfg.DragCoefficient * VelDir;

            // --- Thrust ---
            FVector Thrust = Forward * (CurrentThrust * Cfg.ThrustPower);

            // --- Stall correction torque ---
            FVector StallTorque = FVector::ZeroVector;
            if (FMath::Abs(FMath::RadiansToDegrees(AOA)) >= Cfg.StallAngleDegrees)
            {
                FQuat TargetQuat = VelDir.ToOrientationQuat();
                FQuat DeltaQuat  = TargetQuat * GetActorQuat().Inverse();

                FVector Axis; float Angle;
                DeltaQuat.ToAxisAndAngle(Axis, Angle);
                StallTorque = Axis * Angle * (Cfg.StabilityTorque / Cfg.Mass);
            }

            // --- Environment ---
            FVector Wind       = EnvAirflow.WindDirection * EnvAirflow.WindForce;
            FVector Updraft    = EnvAirflow.UpdraftForce * Up;
            FVector Turbulence = EnvAirflow.TurbulenceStrength * LastTurbulence;

            // --- Total Forces ---
            FVector TotalForce = Gravity + Lift + Drag + Thrust + Wind + Updraft + Turbulence;
            OutLinearAcceleration = TotalForce / FMath::Max(Cfg.Mass, 1.f);

            // --- Angular motion from player + stall ---
            static FVector SmoothedAngularVelocity = FVector::ZeroVector;

            FVector DesiredAngularVelocity = FVector::ZeroVector;

                if (!FMath::IsNearlyZero(SteeringInput.Y))
                {
                    DesiredAngularVelocity.X = SteeringInput.Y * Cfg.PitchRate; // Pitch
                }

                if (!FMath::IsNearlyZero(SteeringInput.X))
                {
                    DesiredAngularVelocity.Y = SteeringInput.X * Cfg.RollRate;  // Roll
                }

                if (!FMath::IsNearlyZero(YawInput))
                {
                    DesiredAngularVelocity.Z = YawInput * Cfg.YawRate;          // Yaw
                }

                // Smooth damping (blend player input with stall torque)
                float DampingFactor = 4.f; // tweak: higher = faster stop
                SmoothedAngularVelocity = FMath::VInterpTo(
                    SmoothedAngularVelocity,
                    DesiredAngularVelocity + StallTorque,
                    DeltaTime,
                    DampingFactor
                );

                if (SmoothedAngularVelocity.SizeSquared() < KINDA_SMALL_NUMBER)
                {
                    SmoothedAngularVelocity = FVector::ZeroVector;
                }

                OutAngularVelocity = SmoothedAngularVelocity;

                break;
        }

        case EFlightType::Drone:
        {
            const auto& Cfg = DroneConfig;

            FVector Forward = GetActorForwardVector();
            FVector Up      = GetActorUpVector();
            FVector Vel     = MoveComp->LastLinearVelocity;;

            // --- Thrust ---
            FVector ForwardAccel = Forward * (CurrentThrust * Cfg.Acceleration);

            // --- Drag ---
            FVector Drag = -Vel * Cfg.DragCoefficient;

            // --- Environment ---
            FVector Wind       = EnvAirflow.WindDirection * EnvAirflow.WindForce;
            FVector Updraft    = EnvAirflow.UpdraftForce * Up;
            FVector Turbulence = EnvAirflow.TurbulenceStrength * LastTurbulence;

            OutLinearAcceleration = (ForwardAccel + Drag + Wind + Updraft + Turbulence) / FMath::Max(Cfg.Mass, 1.f);
                // --- Angular motion ---
                static FVector SmoothedAngularVelocity = FVector::ZeroVector;

                FVector DesiredAngularVelocity = FVector::ZeroVector;

                if (!FMath::IsNearlyZero(SteeringInput.Y))
                {
                    DesiredAngularVelocity.X = SteeringInput.Y * Cfg.MaxPitchAngle; // Pitch
                }

                if (!FMath::IsNearlyZero(SteeringInput.X))
                {
                    DesiredAngularVelocity.Y = SteeringInput.X * Cfg.MaxRollAngle;  // Roll
                }

                if (!FMath::IsNearlyZero(YawInput))
                {
                    DesiredAngularVelocity.Z = YawInput * Cfg.YawRate;              // Yaw
                }

                // Smooth damping towards desired angular velocity
                float DampingFactor = 6.f; // tweak this: higher = snappier stop
                SmoothedAngularVelocity = FMath::VInterpTo(
                    SmoothedAngularVelocity,
                    DesiredAngularVelocity,
                    DeltaTime,
                    DampingFactor
                );

                // Snap small values to zero so it actually stops
                if (SmoothedAngularVelocity.SizeSquared() < KINDA_SMALL_NUMBER)
                {
                    SmoothedAngularVelocity = FVector::ZeroVector;
                }

                OutAngularVelocity = SmoothedAngularVelocity;

                break;
        }
    }
}

// Called when the game starts or when spawned
void AAAircraftBase::BeginPlay()
{
	Super::BeginPlay();
   
    if (IsLocallyControlled()) // only the owning client sends its inputs
    {
        FTimerHandle InputSendTimer;
        GetWorldTimerManager().SetTimer(
            InputSendTimer,
            this,
            &AAAircraftBase::SendInputsToServer, // wrapper function
            0.1f,  // every 100ms (10Hz)
            true
        );

    }

}

void AAAircraftBase::SendInputsToServer()
{
    // WRAPPER FUNCTION IS NEEDED CUZ OTHERWISE ONLY ON FIRST TIME CURRENT VALUE WOULD BE SENT, ON EACH ITTERATION WE WANT
    // THE CURRENT VALUES ON THE OWNING CLIENT TO BE RUNNING
    Server_SendInputs(CurrentThrust, SteeringInput, YawInput);
}

void AAAircraftBase::Server_SendInputs_Implementation(float OwningClientThrust, FVector2D OwningClientSteering, float OwningClientYaw)
{
    // Sanity Checking LOGIC for thrust can be applied here / but we would also have abilities
    // so keeping sanity check off for an while.
    CurrentThrust = OwningClientThrust;
    SteeringInput = OwningClientSteering;
    YawInput = OwningClientYaw;
}


// Called every frame
void AAAircraftBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    check(MoveComp)
    
    if ((HasAuthority() || IsLocallyControlled()))
    {
        FVector LinearAccel;
        FVector AngularVel;

        // Always compute physics based on inputs/environment
        CalculateAerialPhysics(DeltaTime, LinearAccel, AngularVel);
              
        // Owning client + server both call ApplyPhysicsStep
        MoveComp->ApplyPhysicsStep(DeltaTime, LinearAccel, AngularVel);
    }

}


void AAAircraftBase::ConstructPlaneMesh()
{
    PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh"));
    PlaneMesh->SetupAttachment(RootComponent);
    PlaneMesh->SetWorldScale3D(FVector(1.f));
    if (PlaneMeshAsset)
    {
        PlaneMesh->SetStaticMesh(PlaneMeshAsset);
    }
    
}

void AAAircraftBase::UpdatePlaneMeshAppearance()
{
    if (PlaneMeshAsset && PlaneMesh)
    {
        PlaneMesh->SetStaticMesh(PlaneMeshAsset);
    }
}

void AAAircraftBase::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    UpdatePlaneMeshAppearance();
}

void AAAircraftBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    UpdatePlaneMeshAppearance();
}

void AAAircraftBase::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (AACPlayerController* PC = Cast<AACPlayerController>(GetController()))
    {
        // Bind once per possession
        PC->OnSteerInput.AddDynamic(this, &AAAircraftBase::HandleSteerInput);
        PC->OnYawInput.AddDynamic(this, &AAAircraftBase::HandleYawInput);
        PC->OnThrustInput.AddDynamic(this, &AAAircraftBase::HandleThrustInput);
        PRINTSCREEN("BINDING SUCCESSFULL FOR STEER YAW THRUST SERVER")

    }

    
}

void AAAircraftBase::OnRep_Controller()
{
    Super::OnRep_Controller();
    if (!IsLocallyControlled()) {return;}
    if (AACPlayerController* PC = Cast<AACPlayerController>(GetController()))
    {
        // Client side binding when Controller gets replicated
        PC->OnSteerInput.AddDynamic(this, &AAAircraftBase::HandleSteerInput);
        PC->OnYawInput.AddDynamic(this, &AAAircraftBase::HandleYawInput);
        PC->OnThrustInput.AddDynamic(this, &AAAircraftBase::HandleThrustInput);
        PRINTSCREEN("BINDING SUCCESSFULL FOR STEER YAW THRUST CLIENT")

    }
}



void AAAircraftBase::UnPossessed()
{
    UE_LOG(LogTemp, Log, TEXT("UnPossessed() called"));

    if (AACPlayerController* PC = Cast<AACPlayerController>(GetController()))
    {
        // Unbind to prevent dangling references
        PC->OnSteerInput.RemoveDynamic(this, &AAAircraftBase::HandleSteerInput);
        PC->OnYawInput.RemoveDynamic(this, &AAAircraftBase::HandleYawInput);
        PC->OnThrustInput.RemoveDynamic(this, &AAAircraftBase::HandleThrustInput);
    }

    Super::UnPossessed();
}

// Called to bind functionality to input
void AAAircraftBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    
}


void AAAircraftBase::SetAerialInputs(float Thrust, const FVector2D& InSteeringInput, float InYawInput)
{
   
    CurrentThrust   = FMath::Clamp(Thrust, 0.f, 1.f);
    SteeringInput   = InSteeringInput;
    YawInput        = InYawInput;
}

void AAAircraftBase::HandleSteerInput(const FVector2D InSteeringInput)
{
    PRINTSCREEN("Steering Input Handle CALLED by the controller");
    SteeringInput   = InSteeringInput;
}

void AAAircraftBase::HandleYawInput(float InYawInput)
{
    PRINTSCREEN("YAW Input Handle CALLED by the controller");
    YawInput        = InYawInput;
}

void AAAircraftBase::HandleThrustInput(float InThrustInput)
{
    PRINTSCREEN("THRUST Input Handle CALLED by the controller");

    CurrentThrust   = FMath::Clamp(InThrustInput, 0.f, 1.f);
}


