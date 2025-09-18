// AAAircraftBase.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AircraftMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"
#include <AbilitySystemInterface.h>
#include "Public/AbilityComponent.h"
#include "Public/AbilityManager.h"

#include "AAircraftBase.generated.h"

// Replication structs moved to UAircraftMovementComponent for reusability

USTRUCT(BlueprintType)
struct MYPROJECT_API FAircraftBuild
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Speed = 80.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxAltitude;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Unstability;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Maneuverability;  //Yaw Control
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ThrustPower;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ThrustVelocityMultiplier;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StallAOAThrustMultiplier;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Mass;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DragCoefficientAOAMultiplier;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DragCoefficientBase;//For Drag Force for zero AOA
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PitchRate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RollRate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float YawRate;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinSpeed;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StallAngleDegrees = 45;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WingSpan;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LiftCoefficientAOAMultiplier;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxAllowedGForce;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EffectiveGravity;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReturningTorque; // for making aircraft face the velocity direction
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LiftCoefficientBase;//For lift force for Zero AOA
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
    float CameraBoomLength;
};

USTRUCT(BlueprintType)
struct FEnvironmentalAirflow
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector WindDirection = FVector::ZeroVector;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WindForce = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TurbulenceStrength = 0.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float UpdraftForce = 0.f;
};

UCLASS()
class MYPROJECT_API AAAircraftBase : public APawn, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    AAAircraftBase();

    // flight state (exposed as before)
    float CurrentSpeed;
    float CurrentYawSpeed;
    float currentRollSpeed;
    float CurrentPitchSpeed;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    float TargetRollSpeed;
    float TargetPitchSpeed;
    float TargetYawSpeed;
    FVector CurrentVelocity;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FAircraftBuild BuildStats;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FEnvironmentalAirflow EnvAirflow;
    FVector AirCraftVelocity;

    // Deprecated shim to satisfy existing assets/redirects after moving replication to UAircraftMovementComponent
    // This should not be used at runtime.
    UPROPERTY()
    FCraftRepMovement MoveRep; // DEPRECATED: moved to UAircraftMovementComponent

    // Abilities (unchanged)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System", meta = (AllowPrivateAccess = "true"))
    UAbilityComponent* AbilityComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability System", meta = (AllowPrivateAccess = "true"))
    UAbilityManager* AbilityManager;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability System")
    UAbilitySetDataAsset* DefaultAbilitySet;

    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;
    UPROPERTY(EditAnywhere, Category = "Ability")
    TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
    void GiveDefaultAbilities();

    // Inputs & movement
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Aircraft State")
    FRotator AirCraftAngularVelocity;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Input")
    FVector2D SteerInput;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Input")
    float ThrustInput; // IMPORTANT :- BASE Value -> 1.5, Full throttle forward Value -> 2.5, Full rev Value->0.5
    void processRoll(float Value);
    void processPitch(float Value);
    void processYaw(float Value);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aircraft Movement")
    UAircraftMovementComponent* AircraftMovementComp;

    void InitAbilitySystemComponent();

    // Note: All replication and movement logic is now handled by UAircraftMovementComponent
protected:
    // physics extracted from Tick - called both for normal tick and for replay
    virtual void SimulateFlightPhysics(float DeltaTime);

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
