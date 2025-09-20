// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyProject/GCore/Config.h"
#include "AAircraftBase.generated.h"

UCLASS()
class MYPROJECT_API AAAircraftBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAAircraftBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight")
	EFlightType FlightType = EFlightType::Aircraft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight")
	FAircraftConfig AircraftConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flight")
	FDroneConfig DroneConfig;

	virtual void CalculateAerialPhysics(float DeltaTime, FVector& OutLinearAcceleration, FVector& OutAngularVelocity);
	virtual void SetAerialInputs(float Thrust, const FVector2D& SteeringInput, float YawInput);

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:
	float CurrentThrust = 0.f;
	FVector2D SteeringInput = FVector2D::ZeroVector;
	float YawInput = 0.f;
	FVector LastTurbulence = FVector::ZeroVector;
	FEnvAirflow EnvAirflow;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
