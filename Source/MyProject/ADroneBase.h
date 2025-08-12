// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AAircraftBase.h"
#include "AbilitySystemInterface.h"
//#include "Public/AbilityComponent.h"

#include "ADroneBase.generated.h"
class UAbilityComponent;

/**
 * 
 */
USTRUCT(BlueprintType)
struct MYPROJECT_API FDroneBuild
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Acceleration = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DragCoeff = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Mesh")
	float PitchTiltAngle=15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Mesh")
	float RollTiltAngle=15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	float MinTiltAngle = 15;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float YawRotationSpeed = 90.f; // degrees per second
 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VerticalClimbAngleCutOff = 30.f; // degrees
};
USTRUCT(BlueprintType)
struct MYPROJECT_API FCameraSettings {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraYawSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraPitchSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraPitchMin = -80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraPitchMax = 80.f;
};

UCLASS()
class MYPROJECT_API AADroneBase : public AAAircraftBase
{
	GENERATED_BODY()

public:
	AADroneBase();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone")
	FDroneBuild DroneBuildStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	FCameraSettings CameraStats;

	FVector2D CameraInputStick;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone")
	FVector DroneVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	USceneComponent* FirePoint;
	USceneComponent* GetFirePoint() const { return FirePoint; }
	// Your Drone Class (APawn or AActor)
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* DroneVisualMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FRotator DroneMeshOffestRotation;
	
	// Ability System inherited from AAircraftBase
	// UAbilityManager* AbilityManager; // Inherited from AAircraftBase
	// UAbilitySetDataAsset* DefaultAbilitySet; // Inherited from AAircraftBase




	FRotator CamRotation;
	FRotator TargetVisualMeshRotation;


	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnRep_PlayerState() override;
	void CameraInput(FVector2D Input);
	void SteerInputHandler(FVector2D Input);
	void InitAbilitySystemComponent();

};
