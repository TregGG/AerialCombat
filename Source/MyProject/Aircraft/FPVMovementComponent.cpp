// Fill out your copyright notice in the Description page of Project Settings.


#include "FPVMovementComponent.h"

#include "Net/UnrealNetwork.h"

UFPVMovementComponent::UFPVMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	LastLinearVelocity = FVector::ZeroVector;
}

void UFPVMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFPVMovementComponent, ServerState);

}



void UFPVMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	if (PawnOwner)
	{
		SimulatedLocation = PawnOwner->GetActorLocation();
		SimulatedRotation = PawnOwner->GetActorRotation();
		ServerState.Location = GetActorLocation();
		LastLinearVelocity = PawnOwner->GetActorForwardVector()*100;
		ServerState.LinearVelocity = ServerState.Rotation.RotateVector(FVector(0, 0, 0));
		ServerState.AngularVelocity = ServerState.Rotation.RotateVector(FVector(0, 0, 0));
	}
}

void UFPVMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!PawnOwner) return;

	// SERVER & OWNING CLIENT ARE ALREADY BEING called to be MOVED!!
	// Simulated proxies smooth to ServerState
	if (!PawnOwner->IsLocallyControlled() && !PawnOwner->HasAuthority())
	{
		FVector Goal = ServerState.Location;
		FRotator GoalRot = ServerState.Rotation;

		float Dist = FVector::Dist(SimulatedLocation, Goal);
		if (Dist > TeleportThreshold)
		{
			SimulatedLocation = Goal;
			SimulatedRotation = GoalRot;
		}
		else
		{
			SimulatedLocation = FMath::VInterpTo(SimulatedLocation, Goal, DeltaTime, 10.f);
			SimulatedRotation = FMath::RInterpTo(SimulatedRotation, GoalRot, DeltaTime, 10.f);
		}

		PawnOwner->SetActorLocation(SimulatedLocation);
		PawnOwner->SetActorRotation(SimulatedRotation);
	}

	if (PawnOwner->IsLocallyControlled())
	{
		Server_SyncTrasnform(PawnOwner->GetActorLocation(), PawnOwner->GetActorRotation(), DeltaTime);
	}
}

// ONLY FOR SERVER & IT'S MULTICASTING!!
void UFPVMovementComponent::OnRep_ServerState()
{
	// On simulated proxies, update target
	if (!PawnOwner || PawnOwner->IsLocallyControlled()) return;

	SimulatedLocation = PawnOwner->GetActorLocation(); // start point for interp
	SimulatedRotation = PawnOwner->GetActorRotation();
}

void UFPVMovementComponent::Server_SyncTrasnform_Implementation(FVector OwningClientLocation,
	FRotator OwningClientRotation, float DeltaTime)
{
	const FVector ServerLoc = PawnOwner->GetActorLocation();
	const FRotator ServerRot = PawnOwner->GetActorRotation();

	if (!ServerLoc.Equals(OwningClientLocation, 5.f))
	{
		PawnOwner->SetActorLocation(FMath::VInterpTo(ServerLoc, OwningClientLocation, DeltaTime, 10.f));
	}

	if (!ServerRot.Equals(OwningClientRotation, 1.f))
	{
		PawnOwner->SetActorRotation(FMath::RInterpTo(ServerRot, OwningClientRotation, DeltaTime, 10.f));
	}

}

// ONLY PAWN OWNER & SERVER DO THE PHYSICS CALCULATION
void UFPVMovementComponent::ApplyPhysicsStep(float DeltaTime, const FVector& InLinearAcel, const FVector& InAngularVel)
{
	if (!PawnOwner) return;
	
	// Integrate locally

	// --- Integrate acceleration into velocity ---
	LastLinearVelocity += InLinearAcel * DeltaTime;

	// --- Integrate velocity into position ---
	SimulatedLocation += LastLinearVelocity * DeltaTime;
	
	// --- Smooth angular velocity (damped) ---
	float DampingFactor = 10.f; // higher = faster stop
	LastAngularVelocity = FMath::VInterpTo(LastAngularVelocity, InAngularVel, DeltaTime, DampingFactor);

	// Snap to zero if nearly zero
	if (LastAngularVelocity.SizeSquared() < KINDA_SMALL_NUMBER)
	{
		LastAngularVelocity = FVector::ZeroVector;
	}

	// --- Quaternion rotation integration ---
	// Convert angular velocity (deg/sec) → quaternion delta
	FVector AngularVelocityRad = FMath::DegreesToRadians(LastAngularVelocity) * DeltaTime;

	// Axis-angle: rotation around angular velocity vector
	float Angle = AngularVelocityRad.Size();
	if (Angle > KINDA_SMALL_NUMBER)
	{
		FVector Axis = AngularVelocityRad / Angle; // normalize
		FQuat DeltaQuat(Axis, Angle);

		// Apply delta quaternion to current rotation
		FQuat CurrentQuat = PawnOwner->GetActorQuat();
		FQuat NewQuat = DeltaQuat * CurrentQuat;
		NewQuat.Normalize();

		SimulatedRotation = NewQuat.Rotator();
		PawnOwner->SetActorRotation(NewQuat);
	}
	else
	{
		// No angular movement
		SimulatedRotation = PawnOwner->GetActorRotation();
	}

	

	PawnOwner->SetActorLocation(SimulatedLocation);
	PawnOwner->SetActorRotation(SimulatedRotation);

	LastAngularVelocity = LastAngularVelocity;
	LastLinearVelocity = LastLinearVelocity;
	// If server, replicate authoritative state
	if (PawnOwner->HasAuthority())
	{
		ServerState.Location = SimulatedLocation;
		ServerState.Rotation = SimulatedRotation;
		ServerState.LinearVelocity = LastLinearVelocity;
		ServerState.AngularVelocity = LastAngularVelocity;
	}

	// --- Debug logging once per second ---
	static float TimeAccumulator = 0.f;
	TimeAccumulator += DeltaTime;

	if (TimeAccumulator >= 10.0f)
	{
		UE_LOG(LogTemp, Log, TEXT("[FPVMovement] LinearVel: %s | AngularVel: %s"),
			*LastLinearVelocity.ToString(),
			*InAngularVel.ToString());

		TimeAccumulator = 0.f;
	}

}


