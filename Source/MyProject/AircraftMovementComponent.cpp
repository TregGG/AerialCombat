#include "AircraftMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Components/PrimitiveComponent.h"
#include "Net/UnrealNetwork.h"

UAircraftMovementComponent::UAircraftMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    CurrentLinearVelocity = FVector::ZeroVector;
    CurrentAngularVelocity = FVector::ZeroVector;
    DeltaRotation = FRotator::ZeroRotator;
    SetAngularVelocityMode = false;

    // Initialize aircraft state
    AircraftVelocity = FVector::ZeroVector;
    AircraftAngularVelocity = FVector::ZeroVector;
    
    // Initialize input state
    ThrustInput = 1.5f;
    TargetPitchSpeed = 0.0f;
    TargetRollSpeed = 0.0f;
    TargetYawSpeed = 0.0f;

    // Initialize replication
    ReplicationInterval = 1.f / 20.f; // 20 Hz by default
    TimeSinceLastRep = 0.f;
    InterpTime = 0.f;
    InterpDuration = ReplicationInterval;

    SetIsReplicatedByDefault(true);
}

void UAircraftMovementComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!PawnOwner)
    {
        UE_LOG(LogTemp, Error, TEXT("UAircraftMovementComponent: Not attached to a valid Pawn!"));
        SetComponentTickEnabled(false);
        return;
    }

    if (UPrimitiveComponent* RootPrimComp = Cast<UPrimitiveComponent>(PawnOwner->GetRootComponent()))
    {
        SetUpdatedComponent(RootPrimComp);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("UAircraftMovementComponent: Owning Pawn (%s) has no PrimitiveComponent as Root!"), *GetNameSafe(PawnOwner));
        SetComponentTickEnabled(false);
    }
}

void UAircraftMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!UpdatedComponent || ShouldSkipUpdate(DeltaTime))
    {
        return;
    }

    // If we are the local controller, we use inputs + prediction
    if (PawnOwner && PawnOwner->IsLocallyControlled())
    {
        // create move and predict locally
        FAircraftMove Move = CreateMove(DeltaTime);
        SimulateMove(Move);

        // store and send to server
        UnacknowledgedMoves.Add(Move);
        
        // Limit unacknowledged moves to prevent memory issues
        if (UnacknowledgedMoves.Num() > 100)
        {
            UnacknowledgedMoves.RemoveAt(0, UnacknowledgedMoves.Num() - 100);
        }
        
        Server_SendMove(Move);
    }
    else if (PawnOwner && PawnOwner->HasAuthority())
    {
        // Server authoritative simulation - physics state should be updated by parent
        // We just handle the movement here
    }
    // Non-local clients don't simulate physics - they interpolate from server updates

    // Server authoritative replication (throttled)
    if (PawnOwner && PawnOwner->HasAuthority())
    {
        TimeSinceLastRep += DeltaTime;
        if (TimeSinceLastRep >= ReplicationInterval)
        {
            TimeSinceLastRep = 0.f;

            MoveRep.LinearVelocity = AircraftVelocity;
            MoveRep.AngularVelocity = AircraftAngularVelocity;
            MoveRep.Location = PawnOwner->GetActorLocation();
            MoveRep.Orientation = PawnOwner->GetActorRotation();
            MoveRep.TimeStamp = GetWorld()->GetTimeSeconds();

            // mark actor for replication (actor-level)
            PawnOwner->ForceNetUpdate();
        }
    }
    else if (PawnOwner && !PawnOwner->IsLocallyControlled())
    {
        // client interpolation/extrapolation between OnRep updates for non-local clients
        InterpTime += DeltaTime;
        float Alpha = (InterpDuration > 0.f) ? FMath::Clamp(InterpTime / InterpDuration, 0.f, 1.f) : 1.f;

        FVector InterpLocation = FMath::Lerp(LastLocation, TargetLocation, Alpha);
        FRotator InterpRotation = FMath::Lerp(LastRotation, TargetRotation, Alpha);

        FVector ExtrapolatedLocation = InterpLocation + ReplicatedLinearVelocity * DeltaTime;
        FRotator ExtrapolatedRotation = InterpRotation + FRotator(ReplicatedAngularVelocity.X * DeltaTime, ReplicatedAngularVelocity.Y * DeltaTime, ReplicatedAngularVelocity.Z * DeltaTime);

        PawnOwner->SetActorLocationAndRotation(ExtrapolatedLocation, ExtrapolatedRotation);
    }

    // Apply movement to the component
    FVector DeltaLocation = CurrentLinearVelocity * DeltaTime;

    DeltaRotation.Pitch = CurrentAngularVelocity.X;
    DeltaRotation.Yaw = CurrentAngularVelocity.Y;
    DeltaRotation.Roll = CurrentAngularVelocity.Z;

    FQuat DeltaRotationQuat = DeltaRotation.Quaternion();

    if (SetAngularVelocityMode)
    {
        DeltaRotationQuat = CurrentActorDirection * DeltaRotation.Quaternion() * DeltaTime;
    }

    FHitResult Hit;
    SafeMoveUpdatedComponent(DeltaLocation, DeltaRotationQuat, true, Hit);

    if (Hit.bBlockingHit)
    {
        CurrentLinearVelocity = FVector::VectorPlaneProject(CurrentLinearVelocity, Hit.Normal);
    }
}

// Movement interface methods
void UAircraftMovementComponent::SetLinearVelocity(FVector NewVelocity)
{
    CurrentLinearVelocity = NewVelocity;
}

void UAircraftMovementComponent::SetAngularVelocity(FVector NewAngularVelocity, FQuat NewActorDirection)
{
    SetAngularVelocityMode = true;
    CurrentAngularVelocity = NewAngularVelocity;
    CurrentActorDirection = NewActorDirection;
}

void UAircraftMovementComponent::SetAngularRotation(FVector NewAngularVelocity)
{
    SetAngularVelocityMode = false;
    CurrentAngularVelocity = NewAngularVelocity;
}

// Physics simulation interface - called by parent actor
void UAircraftMovementComponent::UpdatePhysicsState(const FVector& NewAircraftVelocity, const FVector& NewAngularVelocity)
{
    AircraftVelocity = NewAircraftVelocity;
    AircraftAngularVelocity = NewAngularVelocity;
    
    // Update movement component velocities
    CurrentLinearVelocity = AircraftVelocity;
    CurrentAngularVelocity = AircraftAngularVelocity;
}

// Input interface - called by parent actor
void UAircraftMovementComponent::SetInputs(float Throttle, float Pitch, float Roll, float Yaw)
{
    ThrustInput = Throttle;
    TargetPitchSpeed = Pitch;
    TargetRollSpeed = Roll;
    TargetYawSpeed = Yaw;
}

// Replication methods
FAircraftMove UAircraftMovementComponent::CreateMove(float DeltaTime) const
{
    FAircraftMove Move;
    Move.ThrottleInput = ThrustInput;
    Move.PitchInput = TargetPitchSpeed;
    Move.RollInput = TargetRollSpeed;
    Move.YawInput = TargetYawSpeed;
    Move.DeltaTime = DeltaTime;
    Move.TimeStamp = GetWorld()->GetTimeSeconds();
    return Move;
}

void UAircraftMovementComponent::SimulateMove(const FAircraftMove& Move)
{
    // Apply input values for prediction/replay
    ThrustInput = Move.ThrottleInput;
    TargetPitchSpeed = Move.PitchInput;
    TargetRollSpeed = Move.RollInput;
    TargetYawSpeed = Move.YawInput;

    // Call physics simulation on parent actor
    OnPhysicsSimulation.Broadcast(Move.DeltaTime);
}

void UAircraftMovementComponent::Server_SendMove_Implementation(const FAircraftMove& MoveIn)
{
    // Validate the move timestamp to prevent cheating
    if (MoveIn.TimeStamp < GetWorld()->GetTimeSeconds() - 1.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Server_SendMove: Rejecting old move (timestamp: %f, current: %f)"), 
               MoveIn.TimeStamp, GetWorld()->GetTimeSeconds());
        return;
    }

    // Validate delta time to prevent extreme values
    if (MoveIn.DeltaTime <= 0.0f || MoveIn.DeltaTime > 0.1f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Server_SendMove: Rejecting move with invalid delta time: %f"), MoveIn.DeltaTime);
        return;
    }

    // On the server: re-simulate the move (authoritative)
    SimulateMove(MoveIn);

    // update authoritative Move state for replication
    MoveRep.LinearVelocity = AircraftVelocity;
    MoveRep.AngularVelocity = AircraftAngularVelocity;
    MoveRep.Location = PawnOwner->GetActorLocation();
    MoveRep.Orientation = PawnOwner->GetActorRotation();
    MoveRep.TimeStamp = MoveIn.TimeStamp;
}

void UAircraftMovementComponent::OnRep_ServerState()
{
    // called on clients when 'MoveRep' is replicated from server
    UE_LOG(LogTemp, Log, TEXT("OnRep_ServerState: Received server update at timestamp %f"), MoveRep.TimeStamp);
    
    // Save client's current transform to interpolate from
    LastLocation = PawnOwner->GetActorLocation();
    LastRotation = PawnOwner->GetActorRotation();

    // populate target values from server
    TargetLocation = MoveRep.Location;
    TargetRotation = MoveRep.Orientation;
    ReplicatedLinearVelocity = MoveRep.LinearVelocity;
    ReplicatedAngularVelocity = MoveRep.AngularVelocity;

    InterpTime = 0.f;
    InterpDuration = ReplicationInterval;

    // If this is the local owning client, perform reconciliation:
    if (PawnOwner && PawnOwner->IsLocallyControlled())
    {
        // Update our local state to match server
        AircraftVelocity = ReplicatedLinearVelocity;
        AircraftAngularVelocity = ReplicatedAngularVelocity;

        // Snap to server authoritative state
        PawnOwner->SetActorLocationAndRotation(TargetLocation, TargetRotation);

        // Update movement component with server state
        SetLinearVelocity(ReplicatedLinearVelocity);
        SetAngularVelocity(ReplicatedAngularVelocity, PawnOwner->GetActorQuat());

        // Reapply unacknowledged moves that occurred after the server timestamp
        TArray<FAircraftMove> NewMoves;
        for (const FAircraftMove& M : UnacknowledgedMoves)
        {
            if (M.TimeStamp > MoveRep.TimeStamp)
            {
                SimulateMove(M);
                NewMoves.Add(M);
            }
        }
        UnacknowledgedMoves = NewMoves;
    }
    else
    {
        // Non-owning clients: we already set TargetLocation/Rotation above, interpolation uses these
    }
}

void UAircraftMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UAircraftMovementComponent, MoveRep);
}
