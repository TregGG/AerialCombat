// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "AircraftMovementComponent.generated.h"

UCLASS()
class MYPROJECT_API UAircraftMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:

	UAircraftMovementComponent();

protected:

	virtual void BeginPlay() override;


	UPROPERTY(Transient)
	FVector CurrentLinearVelocity;

	UPROPERTY(Transient)
	FRotator CurrentAngularVelocity;

	FRotator DeltaRotation;


	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	UFUNCTION(BlueprintCallable, Category = "Aircraft Movement")
	void SetLinearVelocity(FVector NewVelocity);

	UFUNCTION(BlueprintCallable, Category = "Aircraft Movement")
	void SetAngularVelocity(FRotator NewAngularVelocity);

};
