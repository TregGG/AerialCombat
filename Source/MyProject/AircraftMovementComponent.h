#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "AircraftMovementComponent.generated.h"

USTRUCT()
struct MYPROJECT_API FAircraftMove
{
    GENERATED_BODY()

    UPROPERTY()
    float ThrottleInput;

    UPROPERTY()
    float PitchInput;

    UPROPERTY()
    float RollInput;

    UPROPERTY()
    float YawInput;

    UPROPERTY()
    float DeltaTime;

    UPROPERTY()
    float TimeStamp;
};

USTRUCT()
struct MYPROJECT_API FCraftRepMovement
{
    GENERATED_BODY()

    UPROPERTY()
    FVector LinearVelocity;

    UPROPERTY()
    FVector AngularVelocity;

    UPROPERTY()
    FVector Location;

    UPROPERTY()
    FRotator Orientation;

    UPROPERTY()
    float TimeStamp = 0.f;
};

UCLASS()
class MYPROJECT_API UAircraftMovementComponent : public UPawnMovementComponent
{
    GENERATED_BODY()

public:

    UAircraftMovementComponent();

protected:

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Local simulation state
    UPROPERTY(Transient)
    FVector CurrentLinearVelocity;

    UPROPERTY(Transient)
    FQuat CurrentActorDirection;

    UPROPERTY(Transient)
    FVector CurrentAngularVelocity;

    UPROPERTY(Transient)
    bool SetAngularVelocityMode = false;

    FRotator DeltaRotation;

    // ---------------- Networked movement members ----------------
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Replication")
    float ReplicationInterval;

    // Authoritative server state - replicated to clients
    UPROPERTY(ReplicatedUsing = OnRep_ServerState)
    FCraftRepMovement MoveRep;

    // client interpolation/extrapolation state
    FVector LastLocation;
    FRotator LastRotation;
    FVector TargetLocation;
    FRotator TargetRotation;
    FVector ReplicatedLinearVelocity;
    FVector ReplicatedAngularVelocity;
    float InterpTime;
    float InterpDuration;

    // server-side accumulator
    float TimeSinceLastRep;

    // prediction
    TArray<FAircraftMove> UnacknowledgedMoves;

    // RPC for sending moves to server
    UFUNCTION(Server, Reliable, WithValidation)
    void Server_SendMove(const FAircraftMove& Move);
    bool Server_SendMove_Validate(const FAircraftMove& Move) { return true; }
    void Server_SendMove_Implementation(const FAircraftMove& Move);

    // helper to create + simulate moves for client prediction and server replay
    FAircraftMove CreateMove(float DeltaTime) const;
    void SimulateMove(const FAircraftMove& Move);

    // called on clients when server updates Move
    UFUNCTION()
    void OnRep_ServerState();

public:

    // Movement interface
    UFUNCTION(BlueprintCallable, Category = "Aircraft Movement")
    void SetLinearVelocity(FVector NewVelocity);

    UFUNCTION(BlueprintCallable, Category = "Aircraft Movement")
    void SetAngularVelocity(FVector NewAngularVelocity, FQuat NewActorDirection);

    UFUNCTION(BlueprintCallable, Category = "Aircraft Movement")
    void SetAngularRotation(FVector NewAngularVelocity);

    // Physics simulation interface - called by parent actor
    UFUNCTION(BlueprintCallable, Category = "Aircraft Physics")
    void UpdatePhysicsState(const FVector& NewAircraftVelocity, const FVector& NewAngularVelocity);

    // Delegate for physics simulation
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhysicsSimulation, float, DeltaTime);
    UPROPERTY(BlueprintAssignable, Category = "Aircraft Physics")
    FOnPhysicsSimulation OnPhysicsSimulation;

    // Input interface - called by parent actor
    UFUNCTION(BlueprintCallable, Category = "Aircraft Input")
    void SetInputs(float Throttle, float Pitch, float Roll, float Yaw);

    // Getters
    const FVector& GetLinearVelocity() const { return CurrentLinearVelocity; }
    const FVector& GetAngularVelocity() const { return CurrentAngularVelocity; }
    const FVector& GetAircraftVelocity() const { return AircraftVelocity; }
    
    // Replication
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    // Current aircraft state (set by parent)
    FVector AircraftVelocity;
    FVector AircraftAngularVelocity;
    
    // Current input state
    float ThrustInput;
    float TargetPitchSpeed;
    float TargetRollSpeed;
    float TargetYawSpeed;
};
