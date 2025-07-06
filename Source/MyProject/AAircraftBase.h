// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AircraftMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

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
	float DragCoefficientAOAMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DragCoefficientBase;//For Drag Force for zero AOA
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PitchRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WingSpan;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LiftCoefficientAOAMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxAllowedGForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectiveGravity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LiftCoefficientBase;//For lift force for Zero AOA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float CameraBoomLength;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThrustInput;
	*/
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
	float CurrentSpeed;
	float CurrentYawSpeed;
	float currentRollSpeed;
	float CurrentPitchSpeed;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category= Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	float TargetRollSpeed;
	float TargetPitchSpeed;
	FVector CurrentVelocity;



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAircraftBuild BuildStats;
	FEnvironmentalAirflow EnvAirflow;
	FVector AirCraftVelocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Aircraft State")
	FRotator AirCraftAngularVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Input")
	FVector2D SteerInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aircraft Input")
	float ThrustInput; // IMPORTANT :- BASE Value -> 1.5, Full throttle forward Value -> 2.5, Full rev Value->0.5
	void processRoll(float Value);
	void processPitch(float Value);
	//void Move(float ThrustLeverValue, float DeltaTime);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aircraft Movement")
	UAircraftMovementComponent* AircraftMovementComp;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
