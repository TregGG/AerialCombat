// Fill out your copyright notice in the Description page of Project Settings.


#include "FPVMovementComponent.h"

#include "Net/UnrealNetwork.h"

UFPVMovementComponent::UFPVMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
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
void UFPVMovementComponent::ApplyPhysicsStep(float DeltaTime, const FVector& InLinearVel, const FVector& InAngularVel)
{
	if (!PawnOwner) return;

	// Integrate locally
	SimulatedLocation += InLinearVel * DeltaTime;
	SimulatedRotation += FRotator(InAngularVel.X * DeltaTime,
								  InAngularVel.Y * DeltaTime,
								  InAngularVel.Z * DeltaTime);

	PawnOwner->SetActorLocation(SimulatedLocation);
	PawnOwner->SetActorRotation(SimulatedRotation);

	LastLinearVelocity = InLinearVel;
	LastAngularVelocity = InAngularVel;

	// If server, replicate authoritative state
	if (PawnOwner->HasAuthority())
	{
		ServerState.Location = SimulatedLocation;
		ServerState.Rotation = SimulatedRotation;
		ServerState.LinearVelocity = InLinearVel;
		ServerState.AngularVelocity = InAngularVel;
	}
}


