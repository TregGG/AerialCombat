#pragma once
#include "CoreMinimal.h"
#include "FlightConfigs.generated.h"
/**
 * Flight type enum to switch configs dynamically
 */
UENUM(BlueprintType)
enum class EFlightType : uint8
{
    Aircraft UMETA(DisplayName="Aircraft"),
    Drone    UMETA(DisplayName="Drone"),
};

/**
 * Base flight settings shared by all flying vehicles
 */
USTRUCT(BlueprintType)
struct FBaseFlightConfig
{
    GENERATED_BODY()

    /** Physical mass of the vehicle (kg) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Core")
    float Mass = 1000.f;
    /** Max G-force tolerated (use for stress/blackout sim or limits) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Core")
    float MaxG = 9.f;
    /** Global gravity scale modifier */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Core")
    float GravityScale = 1.f;
    /** Camera arm distance */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Camera")
    float CameraArmLength = 800.f;
};

/**
 * Fixed-wing aircraft config
 */
USTRUCT(BlueprintType)
struct FAircraftConfig : public FBaseFlightConfig
{
    GENERATED_BODY()

    /** Target cruise speed (cm/s) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Thrust")
    float CruiseSpeed = 8000.f;
    /** Thrust power (Newtons approximation) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Thrust")
    float ThrustPower = 100000.f;
    /** Base aerodynamic drag */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Aerodynamics")
    float DragCoefficient = 0.02f;
    /** Base lift coefficient */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Aerodynamics")
    float LiftCoefficient = 0.1f;
    /** Stall angle threshold (deg) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Aerodynamics")
    float StallAngleDegrees = 45.f;
    /** Natural stability torque (restoring moment) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Stability")
    float StabilityTorque = 1000.f;
    /** Control rates (deg/s) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Controls")
    float PitchRate = 5.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Controls")
    float RollRate = 5.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Controls")
    float YawRate = 5.f;
};

/**
 * Multirotor / Drone config
 */
USTRUCT(BlueprintType)
struct FDroneConfig : public FBaseFlightConfig
{
    GENERATED_BODY()
    /** Max horizontal speed (cm/s) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Thrust")
    float MaxSpeed = 6000.f;
    /** Vertical climb rate (cm/s) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Thrust")
    float MaxClimbRate = 3000.f;
    /** Vertical descend rate (cm/s) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Thrust")
    float MaxDescendRate = 2500.f;
    /** Acceleration (cm/s²) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Thrust")
    float Acceleration = 2000.f;
    /** Simple drag coefficient */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Aerodynamics")
    float DragCoefficient = 0.1f;
    /** Control responsiveness */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Controls")
    float YawRate = 30.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Controls")
    float MaxPitchAngle = 10.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Controls")
    float MaxRollAngle = 10.f;
    /** Minimum tilt needed to generate horizontal movement */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight|Controls")
    float MinTiltAngle = 5.f;
};
