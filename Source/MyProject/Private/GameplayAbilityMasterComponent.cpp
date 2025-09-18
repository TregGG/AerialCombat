


// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityMasterComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "../ADroneBase.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

UGameplayAbilityMasterComponent::UGameplayAbilityMasterComponent()
{
    // Set default values for the ability
    RateOfFire = 0.2f;
    MaxAmmo = 100.0f;
    ClipAmmo = 30.0f;
    MaxClipAmmo = 30.0f;
    ShootRange = 1000.0f;
    ReloadTime = 2.0f;
    AbilityBarFill = 0.0f;
    MaxBarFill = 100.0f;
    BacklashStartDelay = 5.0f;
    BacklashTickInterval = 1.0f;
    BacklashDamagePerTick = 5.0f;
    PassiveEffectScale = 1.0f;
    
    // Set GAS-specific defaults
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
    ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
}

/*void UGameplayAbilityMasterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeSinceLastAbilityUse += DeltaTime;
	BacklashTickTimer += DeltaTime;
	TimeSinceLastShoot += DeltaTime;

	HandleBacklash(DeltaTime);
	HandlePassiveScaling();

}*/

void UGameplayAbilityMasterComponent::ModifyAbilityBar(float Delta)
{
	AbilityBarFill = FMath::Clamp(AbilityBarFill + Delta, 0.0f, MaxBarFill);
}


float UGameplayAbilityMasterComponent::GetAbilityBarFill() const
{
	return AbilityBarFill;
}

//void UGameplayAbilityMasterComponent::Server_UseShoot_Implementation()
//{
//	if (TimeSinceLastShoot >= 0 && MaxAmmo > 0 && ClipAmmo > 0) {
//		OnShoot();
//		TimeSinceLastShoot = -RateOfFire;
//		MaxAmmo -= 1;
//		ClipAmmo -= 1;
//	}
//	else if (TimeSinceLastShoot >= 0 && MaxAmmo > 0 && ClipAmmo <= 0) {
//		OnReload();
//	}
//}
//
//void UGameplayAbilityMasterComponent::Server_UseAbilityPrimary_Implementation()
//{
//	OnAbilityPrimary();
//	TimeSinceLastAbilityUse = 0.0f;
//}
//
//void UGameplayAbilityMasterComponent::Server_UseAbilitySecondary_Implementation()
//{
//	OnAbilitySecondary();
//	TimeSinceLastAbilityUse = 0.0f;
//}

//void UGameplayAbilityMasterComponent::Server_UseAbilityTertiary_Implementation()
//{
//	OnAbilityTertiary();
//	TimeSinceLastAbilityUse = 0.0f;
//}

void UGameplayAbilityMasterComponent::HandleBacklash(float DeltaTime)
{
    // Base implementation - can be overridden by child classes
    BacklashTickTimer += DeltaTime;
    if (BacklashTickTimer >= BacklashTickInterval)
	{
        BacklashTickTimer = 0.0f;
		OnBacklashTick(GetNormalizedBar());
	}
}

void UGameplayAbilityMasterComponent::HandlePassiveScaling()
{
    // Base implementation - can be overridden by child classes
	OnPassiveEffectTick(GetNormalizedBar());
}

void UGameplayAbilityMasterComponent::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    // Call parent implementation first
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
    
    // Base implementation - child classes should override this
    UE_LOG(LogTemp, Log, TEXT("GameplayAbilityMasterComponent: ActivateAbility called"));
    
    // Call the appropriate ability function based on the ability type
    // This can be overridden by child classes for specific behavior
    OnShoot();
}

void UGameplayAbilityMasterComponent::OnShoot()
{
    // Base implementation - can be overridden by child classes
    UE_LOG(LogTemp, Log, TEXT("GameplayAbilityMasterComponent: OnShoot called"));
}

void UGameplayAbilityMasterComponent::OnReload()
{
    // Base implementation - can be overridden by child classes
    UE_LOG(LogTemp, Log, TEXT("GameplayAbilityMasterComponent: OnReload called"));
}

// Default no-ops for override
void UGameplayAbilityMasterComponent::OnAbilityPrimary()
{
    // Base implementation - can be overridden by child classes
    UE_LOG(LogTemp, Log, TEXT("GameplayAbilityMasterComponent: OnAbilityPrimary called"));
}
void UGameplayAbilityMasterComponent::OnAbilitySecondary()
{
    // Base implementation - can be overridden by child classes
    UE_LOG(LogTemp, Log, TEXT("GameplayAbilityMasterComponent: OnAbilitySecondary called"));
}
void UGameplayAbilityMasterComponent::OnAbilityTertiary()
{
    // Base implementation - can be overridden by child classes
    UE_LOG(LogTemp, Log, TEXT("GameplayAbilityMasterComponent: OnAbilityTertiary called"));
}
void UGameplayAbilityMasterComponent::OnPassiveEffectTick(float NormalizedBar)
{
    // Base implementation - can be overridden by child classes
    UE_LOG(LogTemp, Log, TEXT("GameplayAbilityMasterComponent: OnPassiveEffectTick called with bar: %f"), NormalizedBar);
}
void UGameplayAbilityMasterComponent::OnBacklashTick(float NormalizedBar)
{
    // Base implementation - can be overridden by child classes
    UE_LOG(LogTemp, Log, TEXT("GameplayAbilityMasterComponent: OnBacklashTick called with bar: %f"), NormalizedBar);
}

void UGameplayAbilityMasterComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UGameplayAbilityMasterComponent, AbilityBarFill);
}
