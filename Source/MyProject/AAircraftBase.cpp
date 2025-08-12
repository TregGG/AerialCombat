// Fill out your copyright notice in the Description page of Project Settings.

#include "AAircraftBase.h"
#include "ACPlayerState.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "AbilityComponent.h"

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
    
    // Initialize default values for BuildStats
    BuildStats.Speed = 80.0f;
    BuildStats.RollRate = 90.0f;
    BuildStats.PitchRate = 90.0f;
    BuildStats.YawRate = 45.0f;
    BuildStats.ThrustPower = 1000.0f;
    BuildStats.ThrustVelocityMultiplier = 0.1f;
    BuildStats.StallAOAThrustMultiplier = 0.5f;
    BuildStats.Mass = 1000.0f;
    BuildStats.DragCoefficientAOAMultiplier = 0.1f;
    BuildStats.DragCoefficientBase = 0.01f;
    BuildStats.MinSpeed = 10.0f;
    BuildStats.StallAngleDegrees = 45.0f;
    BuildStats.WingSpan = 10.0f;
    BuildStats.LiftCoefficientAOAMultiplier = 0.1f;
    BuildStats.MaxAllowedGForce = 9.0f;
    BuildStats.EffectiveGravity = 980.0f;
    BuildStats.ReturningTorque = 100.0f;
    BuildStats.LiftCoefficientBase = 0.1f;
    BuildStats.CameraBoomLength = 1000.0f;
    
	AirCraftVelocity=GetActorForwardVector()*BuildStats.Speed;
    AirCraftAngularVelocity = FRotator::ZeroRotator; 
    SteerInput = FVector2D::ZeroVector;
    ThrustInput = 1.5f; // Set to base value (1.5) instead of 0.0f 
    //CAM SETTINGS
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 1000.0f;
    CameraBoom->bUsePawnControlRotation = true;
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
    //ability
    AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilitySystemComponent"));
    if (AbilityComponent)
    {
        AbilityComponent->SetIsReplicated(true);
        UE_LOG(LogTemp, Log, TEXT("AAAircraftBase Constructor: AbilityComponent created successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AAAircraftBase Constructor: Failed to create AbilityComponent!"));
    }
    
    // New Ability Manager System
    AbilityManager = CreateDefaultSubobject<UAbilityManager>(TEXT("AbilityManager"));
    if (AbilityManager)
    {
        UE_LOG(LogTemp, Log, TEXT("AAAircraftBase Constructor: AbilityManager created successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AAAircraftBase Constructor: Failed to create AbilityManager!"));
    }
    
    // Initialize default values for the ability component
    if (AbilityComponent)
    {
        //AbilityComponent->AbilityBarFill = 100.0f;
        UE_LOG(LogTemp, Log, TEXT("AAAircraftBase Constructor: AbilityComponent initialized with default values"));
    }
}



void AAAircraftBase::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);
    UE_LOG(LogTemp, Log, TEXT("AAAircraftBase PossessedBy: Called"));
}

void AAAircraftBase::InitAbilitySystemComponent()
{
    UE_LOG(LogTemp, Log, TEXT("InitAbilitySystemComponent: Starting initialization..."));
    
    if (!AbilityComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("InitAbilitySystemComponent: AbilityComponent is nullptr!"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("InitAbilitySystemComponent: AbilityComponent is valid"));

    AACPlayerState* ACPlayerState = GetPlayerState<AACPlayerState>();
    if (!ACPlayerState)
    {
        UE_LOG(LogTemp, Error, TEXT("InitAbilitySystemComponent: PlayerState is nullptr!"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("InitAbilitySystemComponent: PlayerState is valid"));

    // Initialize the ability system component with the player state and this actor
    AbilityComponent->InitAbilityActorInfo(ACPlayerState, this);

    UE_LOG(LogTemp, Log, TEXT("InitAbilitySystemComponent: Initialized ASC!"));
}

void AAAircraftBase::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();
    InitAbilitySystemComponent();

}

void AAAircraftBase::GiveDefaultAbilities()
{
    if (!AbilityComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("GiveDefaultAbilities: AbilityComponent is nullptr!"));
        return;
    }

    if (!HasAuthority()) 
    {
        UE_LOG(LogTemp, Warning, TEXT("GiveDefaultAbilities: Not on authority, skipping ability granting"));
        return;
    }

    for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultAbilities) 
    {
        if (AbilityClass)
        {
            const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
            AbilityComponent->GiveAbility(AbilitySpec);
            UE_LOG(LogTemp, Log, TEXT("GiveDefaultAbilities: Granted ability %s"), *AbilityClass->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("GiveDefaultAbilities: Invalid ability class in DefaultAbilities array"));
        }
    }
}

void AAAircraftBase::processRoll(float Value)
{
    TargetRollSpeed = Value * BuildStats.RollRate;
    UE_LOG(LogTemp, Log, TEXT("processRoll: Value = %f, TargetRollSpeed = %f"), Value, TargetRollSpeed);
}

void AAAircraftBase::processPitch(float Value)
{
    TargetPitchSpeed = Value * BuildStats.PitchRate;
    UE_LOG(LogTemp, Log, TEXT("processPitch: Value = %f, TargetPitchSpeed = %f"), Value, TargetPitchSpeed);
}

void AAAircraftBase::processYaw(float Value)
{
    TargetYawSpeed = Value * BuildStats.YawRate;
}



// Called when the game starts or when spawned
void AAAircraftBase::BeginPlay()
{
	Super::BeginPlay();
    CameraBoom->TargetArmLength = BuildStats.CameraBoomLength;

    // Ensure the ability component is properly initialized
    if (AbilityComponent)
    {
        UE_LOG(LogTemp, Log, TEXT("BeginPlay: AbilityComponent is valid"));
        
        // Initialize the ability system component
        InitAbilitySystemComponent();
        GiveDefaultAbilities();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("BeginPlay: AbilityComponent is nullptr!"));
    }
    
    // Initialize the new ability manager system
    if (AbilityManager)
    {
        UE_LOG(LogTemp, Log, TEXT("BeginPlay: AbilityManager is valid"));
        
        // Set the default ability set if one is assigned
        if (DefaultAbilitySet)
        {
            AbilityManager->SetAbilitySet(DefaultAbilitySet);
            UE_LOG(LogTemp, Log, TEXT("BeginPlay: Set default ability set: %s"), *DefaultAbilitySet->SetName);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("BeginPlay: No default ability set assigned"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("BeginPlay: AbilityManager is nullptr!"));
    }
}

// Called every frame
void AAAircraftBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    currentRollSpeed = FMath::FInterpTo(currentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 1.0f);
    CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 1.0f);
    
    // Debug logging for movement
    static float DebugTimer = 0.0f;
    DebugTimer += DeltaTime;
    if (DebugTimer >= 1.0f) // Log every second
    {
        DebugTimer = 0.0f;
        UE_LOG(LogTemp, Log, TEXT("Aircraft Tick: ThrustInput=%f, AirCraftVelocity=%s, Position=%s"), 
            ThrustInput, *AirCraftVelocity.ToString(), *GetActorLocation().ToString());
    }
    FRotator currentRotation = GetActorRotation();
    FVector VelocityDir = CurrentVelocity.GetSafeNormal();
    float RollAngleRadians = FMath::DegreesToRadians(currentRotation.Roll);


    //UE_LOG(LogTemp, Log, TEXT("RollAngleRadians: %f"), RollAngleRadians);

    FVector LocalAngularVelocity(CurrentPitchSpeed, 0.0f, currentRollSpeed); // Pitch, Yaw, Roll

    

    //UE_LOG(LogTemp, Log, TEXT("CurrentPitchSpeed: %f, CurrentRollSpeed: %f"), CurrentPitchSpeed,currentRollSpeed);

    FVector CurrentForwardVector = GetActorForwardVector(); 
    //UE_LOG(LogTemp, Log, TEXT("currentForwardVector: %s"), *CurrentForwardVector.ToString());

    //// Linear Velocity Calculations
    //UE_LOG(LogTemp, Log, TEXT("currentVelocity: %s"), *CurrentVelocity.ToString());

    float AngleOfAttackRad = FMath::Acos(FVector::DotProduct(CurrentForwardVector, VelocityDir));
    float Sign = FVector::DotProduct(CurrentVelocity,GetActorUpVector()) < 0 ? 1.0f : -1.0f;//sign of AOA
    AngleOfAttackRad *= Sign;

    //UE_LOG(LogTemp, Log, TEXT("AngleOfAttack: %f"), AngleOfAttackRad);
    // //Calculating G-Force
    float MaxGForce = (FVector::CrossProduct(FVector(FMath::Cos(RollAngleRadians) * BuildStats.PitchRate, FMath::Sin(RollAngleRadians) * BuildStats.PitchRate, BuildStats.RollRate), AirCraftVelocity).Size());//GForce Calculation
    if (MaxGForce >= BuildStats.MaxAllowedGForce) {
        LocalAngularVelocity *= BuildStats.MaxAllowedGForce / MaxGForce;//Relative scaling of the input so it feels more REAL
        //UE_LOG(LogTemp, Log, TEXT("GMAXFORCEs: %f"), MaxGForce);

    }
    ////Making Angular velocity also dependent on the AOA
    LocalAngularVelocity *= FMath::Cos(AngleOfAttackRad);


    float EffectiveAOAMultiplier = 1.0f;
    //stalling
    if (AngleOfAttackRad >= FMath::DegreesToRadians(BuildStats.StallAngleDegrees)) {
        //torque that makes the plane return to the direction of velocity
        FQuat CurrentQuat = GetActorQuat();
        FQuat TargetQuat = VelocityDir.ToOrientationQuat();
        FQuat DeltaQuat = TargetQuat * CurrentQuat.Inverse();

        // Convert delta rotation to axis-angle
        FVector Axis;
        float AngleRad;
        DeltaQuat.ToAxisAndAngle(Axis, AngleRad);

        // Clamp angle if it's too large
        AngleRad = FMath::Clamp(AngleRad, -PI, PI);

        // Calculate returning angular velocity/torque
        FVector StallReturningTorque = Axis * AngleRad * BuildStats.ReturningTorque;
        //UE_LOG(LogTemp, Log, TEXT("ReturningTorque: %s"), *StallReturningTorque.ToString());
        EffectiveAOAMultiplier = BuildStats.StallAOAThrustMultiplier;
        ////Assuming that the inertia and the mass of the craft is same
        LocalAngularVelocity += StallReturningTorque / BuildStats.Mass;
    }
    
    


    //Velocity Calculations
    



    //Gravity
    FVector GravityForce = FVector(0, 0, -1) * BuildStats.EffectiveGravity*BuildStats.Mass;
    //appling Lift
    float LiftForceAgaisntGravity = GravityForce.Size();
    float RealLiftCoeff = BuildStats.LiftCoefficientAOAMultiplier * FMath::Sin(2 * AngleOfAttackRad) * BuildStats.WingSpan+BuildStats.LiftCoefficientBase;
    //calculating lift direction
    FVector LiftDirection;
    if (CurrentVelocity.IsNearlyZero())
    {
        LiftDirection = FVector::UpVector; // Default to world up if no velocity, or handle as desired
    }
    else
    {
        LiftDirection = GetActorUpVector() - (GetActorUpVector().Dot(VelocityDir)) * VelocityDir;
        LiftDirection.Normalize(); // Normalize to ensure it's a unit vector
    }

    FVector LiftForce = (0.5f * CurrentVelocity.SizeSquared() * RealLiftCoeff) * LiftDirection;
    //FVector LiftForce = (0.5f * CurrentVelocity.SizeSquared() * RealLiftCoeff) * (FVector::CrossProduct(VelocityDir, FVector::CrossProduct(GetActorUpVector(), VelocityDir)));//Applies One Lift Constantly
    //UE_LOG(LogTemp, Log, TEXT("LiftForce: %s"), *LiftForce.ToString());

    //Appling Drag
    float RealDragCoeff = FMath::Abs(BuildStats.DragCoefficientAOAMultiplier * (FMath::Sin(AngleOfAttackRad) * BuildStats.WingSpan))+BuildStats.DragCoefficientBase;
    FVector DragForce = 0.5f * CurrentVelocity.SizeSquared() * RealDragCoeff * (-VelocityDir);
    //UE_LOG(LogTemp, Log, TEXT("DragForce: %s"), *DragForce.ToString());
    //EffectiveAOA
    EffectiveAOAMultiplier *= FMath::Cos(AngleOfAttackRad);
    //UE_LOG(LogTemp, Log, TEXT("EffectiveAOAMultiplier: %f"), EffectiveAOAMultiplier);

    FVector ThrustForce = CurrentForwardVector * (ThrustInput * BuildStats.ThrustPower+ CurrentVelocity.Size()*BuildStats.ThrustVelocityMultiplier*EffectiveAOAMultiplier); //Thrust Input-> Acc. level value E[0.5,2.5]
    //UE_LOG(LogTemp, Log, TEXT("ThrustForce: %s"), *ThrustForce.ToString());

    



    

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

    if (!AirCraftVelocity.IsNearlyZero())
    {
        FRotator TargetRot = AirCraftVelocity.ToOrientationRotator();
        CameraBoom->SetWorldRotation(TargetRot);
    }
     if (AircraftMovementComp)
        {
    //        AircraftMovementComp->SetAngularVelocity(AirCraftAngularVelocity);
         CurrentVelocity = AirCraftVelocity;
         //AircraftMovementComp->SetAngularVelocity(DesiredAngularVelocity);
         AircraftMovementComp->SetAngularVelocity(LocalAngularVelocity,GetActorQuat());
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

UAbilitySystemComponent* AAAircraftBase::GetAbilitySystemComponent() const
{
    return AbilityComponent;
}

