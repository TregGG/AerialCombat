// Fill out your copyright notice in the Description page of Project Settings.

#include "AAircraftBase.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"

// Sets default values
AAAircraftBase::AAAircraftBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    USphereComponent* RootCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollision"));
    RootCollision->InitSphereRadius(50.0f); // Adjust size as needed for your aircraft's base collision
    RootCollision->SetCollisionProfileName(TEXT("Pawn")); // Use a suitable collision profile
    RootCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // Ensure collision is enabled
    SetRootComponent(RootCollision); // Make this the RootComponent of the Pawn

    // Now, attach other components to this RootCollision component
    AircraftMovementComp = CreateDefaultSubobject<UAircraftMovementComponent>(TEXT("AircraftMovementComponent"));
	AirCraftVelocity=GetActorForwardVector()*BuildStats.Speed;
    AirCraftAngularVelocity = FRotator::ZeroRotator; 
    SteerInput = FVector2D::ZeroVector;
    ThrustInput = 0.0f; 
    //CAM SETTINGS
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 1000.0f;
    CameraBoom->bUsePawnControlRotation = true;
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}


void AAAircraftBase::Steer(FVector2D StickInput)
{
    //float DeltaTime = GetWorld()->GetDeltaSeconds();
    ////Edit and add restriction on max roll allowed
    //FRotator DeltaRotator = FRotator(StickInput.Y*DeltaTime*BuildStats.PitchRate, 0.0f , -StickInput.X*DeltaTime*BuildStats.RollRate);
    ////AddActorLocalRotation(DeltaRotator);


    // Angular Velocity Calculations
    //FRotator DesiredAngularVelocityPerSecond = FRotator::ZeroRotator;

    //DesiredAngularVelocityPerSecond.Pitch = StickInput.Y * BuildStats.PitchRate;
    ////DesiredAngularVelocityPerSecond.Yaw = SteerInput.X * BuildStats.Maneuverability; -- we dont want this as of now.
    //DesiredAngularVelocityPerSecond.Roll = StickInput.X * BuildStats.RollRate;

    //AirCraftAngularVelocity = DesiredAngularVelocityPerSecond;

    //if (AircraftMovementComp) {
    //    AircraftMovementComp->SetAngularVelocity(AirCraftAngularVelocity);
    //    UE_LOG(LogTemp, Log, TEXT("Updating The Angular Velocity"));

    //}
}


void AAAircraftBase::processRoll(float Value)
{
    TargetRollSpeed = Value * BuildStats.RollRate;
}

void AAAircraftBase::processPitch(float Value)
{
    TargetPitchSpeed = Value * BuildStats.PitchRate;
}



// Called when the game starts or when spawned
void AAAircraftBase::BeginPlay()
{
	Super::BeginPlay();
    CameraBoom->TargetArmLength = BuildStats.CameraBoomLength;
    DesiredAngularVelocity = FRotator(0, 0, 0);

}

// Called every frame
void AAAircraftBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    currentRollSpeed = FMath::FInterpTo(currentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 1.0f);
    CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 1.0f);
    FRotator currentRotation = GetActorRotation();
    float RollAngleRadians = FMath::DegreesToRadians(currentRotation.Roll);
    //float RollAngleRadians = (currentRotation.Roll);
    DesiredAngularVelocity.Pitch = FMath::Cos(RollAngleRadians)* CurrentPitchSpeed;
    DesiredAngularVelocity.Yaw = FMath::Sin(RollAngleRadians)*CurrentPitchSpeed;
    //DesiredAngularVelocity.Pitch = CurrentPitchSpeed;
    DesiredAngularVelocity.Roll = currentRollSpeed;

    FVector CurrentForwardVector = GetActorForwardVector(); 
    //UE_LOG(LogTemp, Log, TEXT("currentForwardVector: %s"), *CurrentForwardVector.ToString());

    //// Linear Velocity Calculations
    //UE_LOG(LogTemp, Log, TEXT("currentVelocity: %s"), *CurrentVelocity.ToString());

    float AngleOfAttackRad = FMath::Acos(FVector::DotProduct(CurrentForwardVector, CurrentVelocity.GetSafeNormal()));
    float Sign = FVector::DotProduct(CurrentVelocity,GetActorUpVector()) < 0 ? 1.0f : -1.0f;//sign of AOA
    AngleOfAttackRad *= Sign;

    UE_LOG(LogTemp, Log, TEXT("AngleOfAttack: %f"), AngleOfAttackRad);


    //Gravity
    FVector GravityForce = FVector(0, 0, -1) * BuildStats.EffectiveGravity*BuildStats.Mass;
    //appling Lift
    float LiftForceAgaisntGravity = GravityForce.Size();
    float RealLiftCoeff = BuildStats.LiftCoefficient * FMath::Sin(2 * AngleOfAttackRad) * BuildStats.WingSpan;
    FVector LiftForce = (0.5f * CurrentVelocity.SizeSquared() * RealLiftCoeff+LiftForceAgaisntGravity) * (FVector::CrossProduct(CurrentVelocity.GetSafeNormal(), FVector::CrossProduct(GetActorUpVector(), CurrentVelocity.GetSafeNormal())));//Applies One Lift Constantly
    UE_LOG(LogTemp, Log, TEXT("LiftForce: %s"), *LiftForce.ToString());

    //Appling Drag
    float RealDragCoeff = FMath::Abs(BuildStats.DragCoefficient * (FMath::Sin(AngleOfAttackRad * BuildStats.WingSpan) + 1 ));
    FVector DragForce = 0.5f * CurrentVelocity.SizeSquared() * RealDragCoeff * (-CurrentVelocity.GetSafeNormal());
    //UE_LOG(LogTemp, Log, TEXT("DragForce: %s"), *DragForce.ToString());

    FVector ThrustForce = CurrentForwardVector * (ThrustInput * BuildStats.ThrustPower); //Thrust Input-> Acc. level value E[0.5,2.5]
    //UE_LOG(LogTemp, Log, TEXT("ThrustForce: %s"), *ThrustForce.ToString());

    //Calculating G-Force
    float MaxGForce = (FVector::CrossProduct(FVector(FMath::Cos(RollAngleRadians) * BuildStats.PitchRate, FMath::Sin(RollAngleRadians) * BuildStats.PitchRate, BuildStats.RollRate), AirCraftVelocity).Size());//GForce Calculation
    if (MaxGForce >= BuildStats.MaxAllowedGForce) {
        DesiredAngularVelocity *= BuildStats.MaxAllowedGForce / MaxGForce;//Relative scaling of the input so it feels more REAL
        //UE_LOG(LogTemp, Log, TEXT("GMAXFORCEs: %f"), MaxGForce);

    }


    //FVector DragForce = FVector::ZeroVector; // DRAG
    //float CurrentSpeedSquared = AirCraftVelocity.SizeSquared();
    //float DragMagnitude = BuildStats.DragCoefficient * CurrentSpeedSquared; 
    //DragForce = -AirCraftVelocity.GetSafeNormal() * DragMagnitude; 
    ////

    FVector TotalLinearForce = ThrustForce + DragForce+LiftForce+GravityForce; 
    TotalLinearForce += EnvAirflow.WindDirection * EnvAirflow.WindForce; // Apply wind force
    TotalLinearForce += EnvAirflow.UpdraftForce * GetActorUpVector(); //  Apply updraft force
    TotalLinearForce += EnvAirflow.TurbulenceStrength * FMath::VRand() * BuildStats.Unstability; // TURBULENCE

    float EffectiveMass = FMath::Max(BuildStats.Mass, 0.001f); // Prevent division by zero
    FVector LinearAcceleration = TotalLinearForce / EffectiveMass;
    AirCraftVelocity += LinearAcceleration * DeltaTime;

    float MaxHorizontalSpeed = BuildStats.Speed; 
    if (AirCraftVelocity.SizeSquared() > FMath::Square(MaxHorizontalSpeed))
    {

        AirCraftVelocity = AirCraftVelocity.GetSafeNormal() * MaxHorizontalSpeed;

    }

    //// Yaw through Horizontal Lift Component Calculations (Centrifugal Force from Roll)
    //float LiftCoefficient = BuildStats.Maneuverability; // Re-purpose Maneuverability for lift coefficient
    //float LiftForceMagnitude = FMath::Sin(FMath::DegreesToRadians(GetActorRotation().Roll));
    //FVector CentrifugalForceDirection = FVector::CrossProduct(GetActorUpVector(), AirCraftVelocity).GetSafeNormal(); 
    //AirCraftVelocity += LiftCoefficient * LiftForceMagnitude * CentrifugalForceDirection * DeltaTime; 

    //// Angular Velocity Calculations
    ////FRotator DesiredAngularVelocityPerSecond = FRotator::ZeroRotator; 

    ////DesiredAngularVelocityPerSecond.Pitch = SteerInput.Y * BuildStats.PitchRate; 
    ////DesiredAngularVelocityPerSecond.Yaw = SteerInput.X * BuildStats.Maneuverability;
    ////DesiredAngularVelocityPerSecond.Roll = SteerInput.X * BuildStats.RollRate;

    ////AirCraftAngularVelocity = DesiredAngularVelocityPerSecond;
    if (!AirCraftVelocity.IsNearlyZero())
    {
        FRotator TargetRot = AirCraftVelocity.ToOrientationRotator();
        CameraBoom->SetWorldRotation(TargetRot);
    }
     if (AircraftMovementComp)
        {
    //        AircraftMovementComp->SetAngularVelocity(AirCraftAngularVelocity);
         CurrentVelocity = AirCraftVelocity;
            AircraftMovementComp->SetAngularVelocity(DesiredAngularVelocity);
            AircraftMovementComp->SetLinearVelocity(AirCraftVelocity);
            //UE_LOG(LogTemp, Log, TEXT("Updating The Velocity"));
            //UE_LOG(LogTemp, Log, TEXT("currentForwardVector: %s"), *CurrentForwardVector.ToString());

        }
     else {
            UE_LOG(LogTemp, Warning, TEXT("not able to get AircraftMovemnentComp"));

        }
        //UE_LOG(LogTemp,Log,TEXT("CurrentForwardDirection ;: %s", CurrentForwardVector.ToString()))
}

// Called to bind functionality to input
void AAAircraftBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);

}

