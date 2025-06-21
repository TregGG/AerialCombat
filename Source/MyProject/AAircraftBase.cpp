// Fill out your copyright notice in the Description page of Project Settings.


#include "AAircraftBase.h"

// Sets default values
AAAircraftBase::AAAircraftBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AirCraftVelocity=GetActorForwardVector()*BuildStats.Speed;

}


void AAAircraftBase::Steer(FVector2D StickInput)
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FRotator DeltaRotator = FRotator(StickInput.Y*DeltaTime*BuildStats.PitchRate, 0.0f , -StickInput.X*DeltaTime*BuildStats.RollRate);
	AddActorLocalRotation(DeltaRotator);
}

void AAAircraftBase::Move(float ThrustLeverValue, float DeltaTime)
{
	float CurrentSpeedSquared = AirCraftVelocity.Length()*DeltaTime;
	CurrentSpeedSquared *= CurrentSpeedSquared;
	AirCraftVelocity+=EnvAirflow.WindDirection*EnvAirflow.WindForce;
	AirCraftVelocity+=BuildStats.ThrustPower*ThrustLeverValue*GetActorForwardVector()*DeltaTime;
	AirCraftVelocity-=BuildStats.DragCoefficient*CurrentSpeedSquared*GetActorForwardVector()*DeltaTime;
	AirCraftVelocity+= EnvAirflow.TurbulenceStrength*FMath::VRand()* BuildStats.Unstability*DeltaTime;
	AirCraftVelocity+=EnvAirflow.UpdraftForce*GetActorUpVector()*DeltaTime;
	// We do Yaw through HORIZONTAL LIFT COMPONENT CALCULATIONS!
	float LiftCoffecient = BuildStats.Maneuverability*CurrentSpeedSquared;
	float LiftForceMagnitude= FMath::Sin(FMath::DegreesToRadians(GetActorRotation().Roll));
	FVector CentrifugalForceDirection = FVector::CrossProduct(GetActorUpVector(),GetActorUpVector());
	AirCraftVelocity+=LiftCoffecient*LiftForceMagnitude*CentrifugalForceDirection;
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

