#include "AbilityComponent.h"
#include "Net/UnrealNetwork.h"

UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false; // No longer need ticking
	SetIsReplicatedByDefault(true);
	UE_LOG(LogTemp, Log, TEXT("UAbilityComponent Constructor: Component created"));
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("UAbilityComponent BeginPlay: Component started"));
}


//// GAS method wrappers

void UAbilityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Register which properties should replicate
	DOREPLIFETIME(UAbilityComponent, AbilityBarFill);
}
