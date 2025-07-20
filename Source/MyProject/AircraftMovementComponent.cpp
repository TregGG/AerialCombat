// Fill out your copyright notice in the Description page of Project Settings.
#include "AircraftMovementComponent.h"
#include "GameFramework/Pawn.h" 
#include "Components/PrimitiveComponent.h"



UAircraftMovementComponent::UAircraftMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentLinearVelocity = FVector::ZeroVector;
	CurrentAngularVelocity = FVector::ZeroVector;
	DeltaRotation = FRotator::ZeroRotator;
	SetAngularVelocityMode = false;
};


// Called when the game starts
void UAircraftMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!PawnOwner)
	{
		UE_LOG(LogTemp, Error, TEXT("UAircraftMovementComponent: Component is not attached to a valid Pawn!"));
		SetComponentTickEnabled(false);
		return;
	}

	if (UPrimitiveComponent* RootPrimComp = Cast<UPrimitiveComponent>(PawnOwner->GetRootComponent()))
	{
		SetUpdatedComponent(RootPrimComp);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UAircraftMovementComponent: Owning Pawn (%s) has no PrimitiveComponent as its RootComponent! Cannot apply movement."), *GetNameSafe(PawnOwner));
		SetComponentTickEnabled(false); // Disable tick as movement cannot be applied
	}
}

void UAircraftMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	FVector DeltaLocation = CurrentLinearVelocity * DeltaTime;

	//DeltaRotation += CurrentAngularVelocity * DeltaTime; OLD METHOD WHILE USING FROTATOR
	DeltaRotation.Pitch = CurrentAngularVelocity.X * DeltaTime;
	DeltaRotation.Yaw = CurrentAngularVelocity.Y * DeltaTime;
	DeltaRotation.Roll = CurrentAngularVelocity.Z * DeltaTime;
	FQuat DeltaRotationQuat = DeltaRotation.Quaternion();
	if (SetAngularVelocityMode) {
		DeltaRotationQuat = CurrentActorDirection * DeltaRotation.Quaternion();

	}

	FHitResult Hit;
	SafeMoveUpdatedComponent(DeltaLocation, DeltaRotationQuat, true, Hit);

	if (Hit.bBlockingHit)
	{
		CurrentLinearVelocity = FVector::VectorPlaneProject(CurrentLinearVelocity, Hit.Normal);
	}

}

void UAircraftMovementComponent::SetLinearVelocity(FVector NewVelocity)
{
	CurrentLinearVelocity = NewVelocity;
	// Optional: Log the new velocity for debugging
	 //UE_LOG(LogTemp, Log, TEXT("SetLinearVelocity: %s"), *NewVelocity.ToString());
}


void UAircraftMovementComponent::SetAngularVelocity(FVector NewAngularVelocity,FQuat NewActorDirection)// Takes vector NOT ROTATOR
{
	SetAngularVelocityMode = true;
	CurrentAngularVelocity = NewAngularVelocity;
	CurrentActorDirection = NewActorDirection;
	//UE_LOG(LogTemp, Log, TEXT("SetAngularVelocity: %s"), *NewAngularVelocity.ToString());

}

void UAircraftMovementComponent::SetAngularRotaion(FVector NewAngularVelocity)
{
	SetAngularVelocityMode = false;
	CurrentAngularVelocity = NewAngularVelocity;
}
