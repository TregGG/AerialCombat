// Fill out your copyright notice in the Description page of Project Settings.


#include "AAircraftBase.h"

// Sets default values
AAAircraftBase::AAAircraftBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
            FVector Vel     = GetVelocity();
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
            OutAngularVelocity = FVector(
                SteeringInput.Y * Cfg.PitchRate,
                SteeringInput.X * Cfg.RollRate,
                YawInput * Cfg.YawRate
            ) + StallTorque;

            break;
        }

        case EFlightType::Drone:
        {
            const auto& Cfg = DroneConfig;

            FVector Forward = GetActorForwardVector();
            FVector Up      = GetActorUpVector();
            FVector Vel     = GetVelocity();

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
            OutAngularVelocity = FVector(
                SteeringInput.Y * Cfg.MaxPitchAngle,
                SteeringInput.X * Cfg.MaxRollAngle,
                YawInput * Cfg.YawRate
            );

            break;
        }
    }
}

void AAAircraftBase::SetAerialInputs(float Thrust, const FVector2D& InSteeringInput, float InYawInput)
{
    CurrentThrust   = FMath::Clamp(Thrust, 0.f, 1.f);
    SteeringInput   = InSteeringInput;
    YawInput        = InYawInput;
}


// Called when the game starts or when spawned
void AAAircraftBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAAircraftBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAAircraftBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

