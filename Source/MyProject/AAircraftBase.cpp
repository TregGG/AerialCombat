// AAAircraftBase.cpp
#include "AAircraftBase.h"
#include "AAircraftBase.h"
#include "ACPlayerState.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "AbilityComponent.h"
#include "Net/UnrealNetwork.h"

AAAircraftBase::AAAircraftBase()
{
    bReplicates = true;

    // NOTE: we manage our own movement replication, so don't rely on automatic actor movement replication.
    SetReplicateMovement(false);

    PrimaryActorTick.bCanEverTick = true;

    USphereComponent* RootCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollision"));
    RootCollision->InitSphereRadius(50.0f);
    RootCollision->SetCollisionProfileName(TEXT("Pawn"));
    RootCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SetRootComponent(RootCollision);

    AircraftMovementComp = CreateDefaultSubobject<UAircraftMovementComponent>(TEXT("AircraftMovementComponent"));
    if (AircraftMovementComp)
    {
        // movement component will update the root primitive component
        AircraftMovementComp->UpdatedComponent = RootComponent;
        // ensure the component itself replicates its properties
        AircraftMovementComp->SetIsReplicated(true);
    }

    // default build stats (same as your original)
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

    AirCraftVelocity = GetActorForwardVector() * BuildStats.Speed;
    AirCraftAngularVelocity = FRotator::ZeroRotator;
    SteerInput = FVector2D::ZeroVector;
    ThrustInput = 1.5f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 1000.0f;
    CameraBoom->bUsePawnControlRotation = true;
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

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

    AbilityManager = CreateDefaultSubobject<UAbilityManager>(TEXT("AbilityManager"));
    if (AbilityManager)
    {
        UE_LOG(LogTemp, Log, TEXT("AAAircraftBase Constructor: AbilityManager created successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AAAircraftBase Constructor: Failed to create AbilityManager!"));
    }

    // Replication is now handled by UAircraftMovementComponent
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

    AACPlayerState* ACPlayerState = GetPlayerState<AACPlayerState>();
    if (!ACPlayerState)
    {
        UE_LOG(LogTemp, Error, TEXT("InitAbilitySystemComponent: PlayerState is nullptr!"));
        return;
    }

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
    
    // Update movement component with new input
    if (AircraftMovementComp)
    {
        AircraftMovementComp->SetInputs(ThrustInput, TargetPitchSpeed, TargetRollSpeed, TargetYawSpeed);
    }
}

void AAAircraftBase::processPitch(float Value)
{
    TargetPitchSpeed = Value * BuildStats.PitchRate;
    UE_LOG(LogTemp, Log, TEXT("processPitch: Value = %f, TargetPitchSpeed = %f"), Value, TargetPitchSpeed);
    
    // Update movement component with new input
    if (AircraftMovementComp)
    {
        AircraftMovementComp->SetInputs(ThrustInput, TargetPitchSpeed, TargetRollSpeed, TargetYawSpeed);
    }
}

void AAAircraftBase::processYaw(float Value)
{
    TargetYawSpeed = Value * BuildStats.YawRate;
    
    // Update movement component with new input
    if (AircraftMovementComp)
    {
        AircraftMovementComp->SetInputs(ThrustInput, TargetPitchSpeed, TargetRollSpeed, TargetYawSpeed);
    }
}

void AAAircraftBase::BeginPlay()
{
    Super::BeginPlay();
    CameraBoom->TargetArmLength = BuildStats.CameraBoomLength;

    // Bind to movement component's physics simulation delegate
    if (AircraftMovementComp)
    {
        AircraftMovementComp->OnPhysicsSimulation.AddDynamic(this, &AAAircraftBase::SimulateFlightPhysics);
        UE_LOG(LogTemp, Log, TEXT("BeginPlay: Bound to movement component physics simulation"));
    }

    if (AbilityComponent)
    {
        UE_LOG(LogTemp, Log, TEXT("BeginPlay: AbilityComponent is valid"));
        InitAbilitySystemComponent();
        GiveDefaultAbilities();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("BeginPlay: AbilityComponent is nullptr!"));
    }

    if (AbilityManager)
    {
        UE_LOG(LogTemp, Log, TEXT("BeginPlay: AbilityManager is valid"));
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

//////////////////////////////////////////
// Core physics simulation (extracted)
//////////////////////////////////////////
void AAAircraftBase::SimulateFlightPhysics(float DeltaTime)
{
    // (This function is a near-direct MoveRep of your previous Tick physics code)
    currentRollSpeed = FMath::FInterpTo(currentRollSpeed, TargetRollSpeed, DeltaTime, 1.0f);
    CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, DeltaTime, 1.0f);

    FVector LocalAngularVelocity(CurrentPitchSpeed, 0.0f, currentRollSpeed); // Pitch, Yaw, Roll

    FVector CurrentForwardVector = GetActorForwardVector();
    FVector VelocityDir = CurrentVelocity.GetSafeNormal();

    // Handle AOA
    float AngleOfAttackRad = 0.f;
    if (!VelocityDir.IsNearlyZero())
    {
        AngleOfAttackRad = FMath::Acos(FVector::DotProduct(CurrentForwardVector, VelocityDir));
        float Sign = FVector::DotProduct(CurrentVelocity, GetActorUpVector()) < 0 ? 1.0f : -1.0f;
        AngleOfAttackRad *= Sign;
    }

    float RollAngleRadians = FMath::DegreesToRadians(GetActorRotation().Roll);

    // G-Force limiting
    float MaxGForce = (FVector::CrossProduct(FVector(FMath::Cos(RollAngleRadians) * BuildStats.PitchRate, FMath::Sin(RollAngleRadians) * BuildStats.PitchRate, BuildStats.RollRate), AirCraftVelocity).Size());
    if (MaxGForce >= BuildStats.MaxAllowedGForce)
    {
        LocalAngularVelocity *= BuildStats.MaxAllowedGForce / MaxGForce;
    }

    LocalAngularVelocity *= FMath::Cos(AngleOfAttackRad);

    float EffectiveAOAMultiplier = 1.0f;

    if (AngleOfAttackRad >= FMath::DegreesToRadians(BuildStats.StallAngleDegrees))
    {
        FQuat CurrentQuat = GetActorQuat();
        FQuat TargetQuat = VelocityDir.ToOrientationQuat();
        FQuat DeltaQuat = TargetQuat * CurrentQuat.Inverse();

        FVector Axis;
        float AngleRad;
        DeltaQuat.ToAxisAndAngle(Axis, AngleRad);
        AngleRad = FMath::Clamp(AngleRad, -PI, PI);

        FVector StallReturningTorque = Axis * AngleRad * BuildStats.ReturningTorque;
        EffectiveAOAMultiplier = BuildStats.StallAOAThrustMultiplier;
        LocalAngularVelocity += StallReturningTorque / BuildStats.Mass;
    }

    // Linear forces
    FVector GravityForce = FVector(0, 0, -1) * BuildStats.EffectiveGravity * BuildStats.Mass;
    float RealLiftCoeff = BuildStats.LiftCoefficientAOAMultiplier * FMath::Sin(2 * AngleOfAttackRad) * BuildStats.WingSpan + BuildStats.LiftCoefficientBase;

    FVector LiftDirection;
    if (CurrentVelocity.IsNearlyZero())
    {
        LiftDirection = FVector::UpVector;
    }
    else
    {
        LiftDirection = GetActorUpVector() - (GetActorUpVector().Dot(VelocityDir)) * VelocityDir;
        LiftDirection.Normalize();
    }

    FVector LiftForce = (0.5f * CurrentVelocity.SizeSquared() * RealLiftCoeff) * LiftDirection;

    float RealDragCoeff = FMath::Abs(BuildStats.DragCoefficientAOAMultiplier * (FMath::Sin(AngleOfAttackRad) * BuildStats.WingSpan)) + BuildStats.DragCoefficientBase;
    FVector DragForce = 0.5f * CurrentVelocity.SizeSquared() * RealDragCoeff * (-VelocityDir);

    EffectiveAOAMultiplier *= FMath::Cos(AngleOfAttackRad);

    FVector ThrustForce = CurrentForwardVector * (ThrustInput * BuildStats.ThrustPower + CurrentVelocity.Size() * BuildStats.ThrustVelocityMultiplier * EffectiveAOAMultiplier);

    FVector TotalLinearForce = ThrustForce + DragForce + LiftForce + GravityForce;
    TotalLinearForce += EnvAirflow.WindDirection * EnvAirflow.WindForce;
    TotalLinearForce += EnvAirflow.UpdraftForce * GetActorUpVector();
    TotalLinearForce += EnvAirflow.TurbulenceStrength * FMath::VRand() * BuildStats.Unstability;

    float EffectiveMass = FMath::Max(BuildStats.Mass, 0.001f);
    FVector LinearAcceleration = TotalLinearForce / EffectiveMass;
    AirCraftVelocity += LinearAcceleration * DeltaTime;

    float MaxHorizontalSpeed = BuildStats.Speed;
    if (AirCraftVelocity.SizeSquared() > FMath::Square(MaxHorizontalSpeed))
    {
        AirCraftVelocity = AirCraftVelocity.GetSafeNormal() * MaxHorizontalSpeed;
    }

    if (!AirCraftVelocity.IsNearlyZero())
    {
        FRotator TargetRot = AirCraftVelocity.ToOrientationRotator();
        CameraBoom->SetWorldRotation(TargetRot);
    }

    // update movement component with physics results
    if (AircraftMovementComp)
    {
        CurrentVelocity = AirCraftVelocity;
        AircraftMovementComp->UpdatePhysicsState(AirCraftVelocity, LocalAngularVelocity);
        AircraftMovementComp->SetAngularVelocity(LocalAngularVelocity, GetActorQuat());
        AircraftMovementComp->SetLinearVelocity(AirCraftVelocity);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("not able to get AircraftMovemnentComp"));
    }
}

//////////////////////////////////////////
// Tick, replication and prediction
//////////////////////////////////////////
void AAAircraftBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update movement component with current inputs
    if (AircraftMovementComp)
    {
        AircraftMovementComp->SetInputs(ThrustInput, TargetPitchSpeed, TargetRollSpeed, TargetYawSpeed);
    }

    // Simulate physics - this will be called by the movement component for prediction/replay
    // and directly for server authoritative simulation
    if (IsLocallyControlled() || HasAuthority())
    {
        SimulateFlightPhysics(DeltaTime);
    }
    // Non-local clients don't simulate physics - they interpolate from server updates via movement component
}

// Replication methods moved to UAircraftMovementComponent for reusability

void AAAircraftBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bind your input axes to the helper functions if you want
    // Example (you may already bind these elsewhere):
    // PlayerInputComponent->BindAxis("Roll", this, &AAAircraftBase::processRoll);
    // PlayerInputComponent->BindAxis("Pitch", this, &AAAircraftBase::processPitch);
    // PlayerInputComponent->BindAxis("Yaw", this, &AAAircraftBase::processYaw);
}

UAbilitySystemComponent* AAAircraftBase::GetAbilitySystemComponent() const
{
    return AbilityComponent;
}

// GetLifetimeReplicatedProps moved to UAircraftMovementComponent for reusability
void AAAircraftBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    // No actor-level replication here; movement/state replication handled by UAircraftMovementComponent
}
