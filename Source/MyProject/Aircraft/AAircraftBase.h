// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyProject/GCore/Config.h"
#include "MyProject/Player/ACPlayerController.h"
#include "AAircraftBase.generated.h"

UCLASS()
class MYPROJECT_API AAAircraftBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAAircraftBase();

	// SETUP MOVEMENTS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight")
	EFlightType FlightType = EFlightType::Aircraft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight")
	FAircraftConfig AircraftConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight")
	FDroneConfig DroneConfig;
protected:
	float CurrentThrust = 0.f;
	FVector2D SteeringInput = FVector2D::ZeroVector;
	float YawInput = 0.f;
	FVector LastTurbulence = FVector::ZeroVector;
	FEnvAirflow EnvAirflow;
public:
	virtual void CalculateAerialPhysics(float DeltaTime, FVector& OutLinearAcceleration, FVector& OutAngularVelocity);
	virtual void SetAerialInputs(float Thrust, const FVector2D& SteeringInput, float YawInput);
	UFUNCTION()
	virtual void HandleSteerInput(const FVector2D SteeringInput);
	UFUNCTION()
	virtual void HandleYawInput(float YawInput);
	UFUNCTION()
	virtual void HandleThrustInput(float Thrust);
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// SETUP VISUAL MESH
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Airplane", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlaneMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Airplane")
	UStaticMesh* PlaneMeshAsset;
	void ConstructPlaneMesh();
	void UpdatePlaneMeshAppearance();
public:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	// SETUP INPUT
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;


};
