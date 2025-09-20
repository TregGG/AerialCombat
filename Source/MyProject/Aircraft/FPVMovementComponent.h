// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "FPVMovementComponent.generated.h"

USTRUCT()
struct FServerState
{
	GENERATED_BODY()

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;

	UPROPERTY()
	FVector LinearVelocity;

	UPROPERTY()
	FVector AngularVelocity;
};

UCLASS()
class MYPROJECT_API UFPVMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	UFPVMovementComponent();

	void ApplyPhysicsStep(float DeltaTime, const FVector& InLinearVel, const FVector& InAngularVel);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual auto GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const -> void override;

	UPROPERTY(ReplicatedUsing=OnRep_ServerState)
	FServerState ServerState;

	UFUNCTION()
	void OnRep_ServerState();

	UFUNCTION(Server, Reliable)
	void Server_SyncTrasnform(FVector OwningClientLocation, FRotator OwningClienRotation, float DeltaTime);

	
private:
	FVector SimulatedLocation;
	FRotator SimulatedRotation;

	FVector LastLinearVelocity;
	FVector LastAngularVelocity;

	UPROPERTY(EditAnywhere)
	float TeleportThreshold = 1000.f;
};
