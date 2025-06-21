// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AAircraftBase.generated.h"

USTRUCT(BlueprintType)
struct MYPROJECT_API FAircraftBuild
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed=80.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxAltitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Unstability; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Maneuverability;  //Yaw Control
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThrustPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DragCoefficient;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PitchRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollRate;
	
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
class MYPROJECT_API AAAircraftBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAAircraftBase();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAircraftBuild BuildStats;
	FEnvironmentalAirflow EnvAirflow;
	FVector AirCraftVelocity;
	void Steer(FVector2D Direction);
	void Move(float ThrustLeverValue, float DeltaTime);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
