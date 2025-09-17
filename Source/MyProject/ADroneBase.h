#pragma once

#include "CoreMinimal.h"
#include "AAircraftBase.h"
#include "AbilitySystemInterface.h"
#include "ADroneBase.generated.h"

class UAbilityComponent;

USTRUCT(BlueprintType)
struct MYPROJECT_API FDroneBuild
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Speed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Acceleration = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float DragCoeff = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh") float PitchTiltAngle = 15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh") float RollTiltAngle = 15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh") float MinTiltAngle = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) float YawRotationSpeed = 90.f; // deg/s
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float VerticalClimbAngleCutOff = 30.f;
};

USTRUCT(BlueprintType)
struct MYPROJECT_API FCameraSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") float CameraYawSpeed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") float CameraPitchSpeed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") float CameraPitchMin = -80.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera") float CameraPitchMax = 80.f;
};

UCLASS()
class MYPROJECT_API AADroneBase : public AAAircraftBase
{
	GENERATED_BODY()

public:
	AADroneBase();
	virtual void BeginPlay() override;

	// ---- Drone-specific stats ----
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	FDroneBuild DroneBuildStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FCameraSettings CameraStats;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone")
	FVector DroneVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	USceneComponent* FirePoint;
	USceneComponent* GetFirePoint() const { return FirePoint; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* DroneVisualMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FRotator DroneMeshOffestRotation;

	// ---- Camera ----
	FVector2D CameraInputStick;
	FRotator  CamRotation;

	// ---- Mesh tilt ----
	FRotator TargetVisualMeshRotation;

	// ---- Overrides ----
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnRep_PlayerState() override;

protected:
	// AircraftBase calls this during Tick for physics
	virtual void SimulateFlightPhysics(float DeltaTime) override;
public:

	// Camera + mesh helpers
	void CameraInput(FVector2D Input);
	void SteerInputHandler(FVector2D Input);
	void UpdateDroneVisualMeshTilt(float DeltaTime, const FVector& CurrentVelocity);

	void InitAbilitySystemComponent();

private:
	FVector2D SteerInput;
};
