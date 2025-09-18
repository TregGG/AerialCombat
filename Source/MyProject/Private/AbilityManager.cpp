#include "AbilityManager.h"
#include "../ADroneBase.h"
#include "AbilityComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "AbilitySystemComponent.h"
#include "Engine/Engine.h"
#include "GameplayAbilityMasterComponent.h"

UAbilityManager::UAbilityManager()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicated(true);
}

void UAbilityManager::BeginPlay()
{
    Super::BeginPlay();
    
    // Get owner references
    OwnerDrone = Cast<AADroneBase>(GetOwner());
    if (OwnerDrone)
    {
        AbilityComponent = OwnerDrone->FindComponentByClass<UAbilityComponent>();
    }
    
    UE_LOG(LogTemp, Log, TEXT("AbilityManager: BeginPlay - OwnerDrone: %s, AbilityComponent: %s"), 
        OwnerDrone ? TEXT("Valid") : TEXT("Invalid"),
        AbilityComponent ? TEXT("Valid") : TEXT("Invalid"));
}

void UAbilityManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (GetOwner() && GetOwner()->GetOwner() && GetOwner()->HasAuthority())
    {
        UpdateCooldowns(DeltaTime);
        UpdateAbilityBar(DeltaTime);
    }
}

void UAbilityManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(UAbilityManager, CurrentAbilitySet);
    DOREPLIFETIME(UAbilityManager, AbilityBarFill);
    DOREPLIFETIME(UAbilityManager, bInCombatZone);
    DOREPLIFETIME(UAbilityManager, ShootCooldownTimer);
    DOREPLIFETIME(UAbilityManager, FirstAbilityCooldownTimer);
    DOREPLIFETIME(UAbilityManager, SecondAbilityCooldownTimer);
    DOREPLIFETIME(UAbilityManager, ThirdAbilityCooldownTimer);
}

// Ability Set Management
void UAbilityManager::SetAbilitySet(UAbilitySetDataAsset* NewAbilitySet)
{
    if (NewAbilitySet)
    {
        CurrentAbilitySet = NewAbilitySet;
        AbilityBarFill = 0.0f; // Reset ability bar when changing sets
        
        UE_LOG(LogTemp, Log, TEXT("AbilityManager: Set ability set to %s"), *NewAbilitySet->SetName);
        
        // Grant the new abilities
        GrantAbilitySet();
    }
}

// Ability Bar Management
void UAbilityManager::IncreaseAbilityBar(float Amount)
{
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        float MaxBar = GetMaxAbilityBar();
        AbilityBarFill = FMath::Clamp(AbilityBarFill + Amount, 0.0f, MaxBar);
        
        UE_LOG(LogTemp, Log, TEXT("AbilityManager: Increased ability bar by %f, new value: %f"), Amount, AbilityBarFill);
    }
}

void UAbilityManager::ConsumeAbilityBar(float Amount)
{
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        AbilityBarFill = FMath::Max(AbilityBarFill - Amount, 0.0f);
        
        UE_LOG(LogTemp, Log, TEXT("AbilityManager: Consumed %f ability bar, remaining: %f"), Amount, AbilityBarFill);
    }
}

float UAbilityManager::GetMaxAbilityBar() const
{
    return CurrentAbilitySet ? CurrentAbilitySet->MaxAbilityBar : 100.0f;
}

// GAS-based ability usage
void UAbilityManager::UseShoot()
{
    FAbilityData ShootAbility = GetShootAbility();
    if (ShootAbility.GameplayAbilityClass)
    {
        ActivateAbilityByClass(ShootAbility.GameplayAbilityClass);
        UE_LOG(LogTemp, Log, TEXT("AbilityManager: Activating shoot ability via GAS"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AbilityManager: No gameplay ability class set for shoot ability"));
    }
}

void UAbilityManager::UseFirstAbility()
{
    FAbilityData FirstAbility = GetFirstAbility();
    if (FirstAbility.GameplayAbilityClass)
    {
        ActivateAbilityByClass(FirstAbility.GameplayAbilityClass);
        UE_LOG(LogTemp, Log, TEXT("AbilityManager: Activating first ability via GAS: %s"), *FirstAbility.AbilityName);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AbilityManager: No gameplay ability class set for first ability"));
    }
}

void UAbilityManager::UseSecondAbility()
{
    FAbilityData SecondAbility = GetSecondAbility();
    if (SecondAbility.GameplayAbilityClass)
    {
        ActivateAbilityByClass(SecondAbility.GameplayAbilityClass);
        UE_LOG(LogTemp, Log, TEXT("AbilityManager: Activating second ability via GAS: %s"), *SecondAbility.AbilityName);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AbilityManager: No gameplay ability class set for second ability"));
    }
}

void UAbilityManager::UseThirdAbility()
{
    FAbilityData ThirdAbility = GetThirdAbility();
    if (ThirdAbility.GameplayAbilityClass)
    {
        ActivateAbilityByClass(ThirdAbility.GameplayAbilityClass);
        UE_LOG(LogTemp, Log, TEXT("AbilityManager: Activating third ability via GAS: %s"), *ThirdAbility.AbilityName);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AbilityManager: No gameplay ability class set for third ability"));
    }
}

// Combat Events
void UAbilityManager::OnKill()
{
    if (GetOwner() && GetOwner()->HasAuthority() && CurrentAbilitySet)
    {
        IncreaseAbilityBar(CurrentAbilitySet->KillBarReward);
        UE_LOG(LogTemp, Log, TEXT("AbilityManager: Kill reward - increased bar by %f"), CurrentAbilitySet->KillBarReward);
    }
}

void UAbilityManager::OnZoneCapture()
{
    if (GetOwner() && GetOwner()->HasAuthority() && CurrentAbilitySet)
    {
        IncreaseAbilityBar(CurrentAbilitySet->ZoneCaptureBarReward);
        UE_LOG(LogTemp, Log, TEXT("AbilityManager: Zone capture reward - increased bar by %f"), CurrentAbilitySet->ZoneCaptureBarReward);
    }
}

void UAbilityManager::OnEnterCombatZone()
{
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        bInCombatZone = true;
        UE_LOG(LogTemp, Log, TEXT("AbilityManager: Entered combat zone"));
    }
}

void UAbilityManager::OnExitCombatZone()
{
    if (GetOwner() && GetOwner()->HasAuthority())
    {
        bInCombatZone = false;
        UE_LOG(LogTemp, Log, TEXT("AbilityManager: Exited combat zone"));
    }
}

// Ability Data Getters
FAbilityData UAbilityManager::GetShootAbility() const
{
    return CurrentAbilitySet ? CurrentAbilitySet->ShootAbility : FAbilityData();
}

FAbilityData UAbilityManager::GetFirstAbility() const
{
    return CurrentAbilitySet ? CurrentAbilitySet->FirstAbility : FAbilityData();
}

FAbilityData UAbilityManager::GetSecondAbility() const
{
    return CurrentAbilitySet ? CurrentAbilitySet->SecondAbility : FAbilityData();
}

FAbilityData UAbilityManager::GetThirdAbility() const
{
    return CurrentAbilitySet ? CurrentAbilitySet->ThirdAbility : FAbilityData();
}

// Cooldown Management
bool UAbilityManager::CanUseShoot() const
{
    FAbilityData ShootAbility = GetShootAbility();
    return CanUseAbility(ShootAbility, ShootCooldownTimer);
}

bool UAbilityManager::CanUseFirstAbility() const
{
    FAbilityData FirstAbility = GetFirstAbility();
    return CanUseAbility(FirstAbility, FirstAbilityCooldownTimer);
}

bool UAbilityManager::CanUseSecondAbility() const
{
    FAbilityData SecondAbility = GetSecondAbility();
    return CanUseAbility(SecondAbility, SecondAbilityCooldownTimer);
}

bool UAbilityManager::CanUseThirdAbility() const
{
    FAbilityData ThirdAbility = GetThirdAbility();
    return CanUseAbility(ThirdAbility, ThirdAbilityCooldownTimer);
}

bool UAbilityManager::CanUseAbility(const FAbilityData& AbilityData, float CooldownTimer) const
{
    // Check cooldown
    if (CooldownTimer > 0.0f)
    {
        return false;
    }
    
    // Check ability bar requirement
    if (AbilityData.bRequiresAbilityBar && AbilityBarFill < AbilityData.AbilityBarCost)
    {
        return false;
    }
    
    return true;
}

// Cooldown Progress Getters (0.0 to 1.0)
float UAbilityManager::GetShootCooldownProgress() const
{
    FAbilityData ShootAbility = GetShootAbility();
    return ShootAbility.Cooldown > 0.0f ? FMath::Clamp(ShootCooldownTimer / ShootAbility.Cooldown, 0.0f, 1.0f) : 0.0f;
}

float UAbilityManager::GetFirstAbilityCooldownProgress() const
{
    FAbilityData FirstAbility = GetFirstAbility();
    return FirstAbility.Cooldown > 0.0f ? FMath::Clamp(FirstAbilityCooldownTimer / FirstAbility.Cooldown, 0.0f, 1.0f) : 0.0f;
}

float UAbilityManager::GetSecondAbilityCooldownProgress() const
{
    FAbilityData SecondAbility = GetSecondAbility();
    return SecondAbility.Cooldown > 0.0f ? FMath::Clamp(SecondAbilityCooldownTimer / SecondAbility.Cooldown, 0.0f, 1.0f) : 0.0f;
}

float UAbilityManager::GetThirdAbilityCooldownProgress() const
{
    FAbilityData ThirdAbility = GetThirdAbility();
    return ThirdAbility.Cooldown > 0.0f ? FMath::Clamp(ThirdAbilityCooldownTimer / ThirdAbility.Cooldown, 0.0f, 1.0f) : 0.0f;
}

// Private Helper Functions
void UAbilityManager::UpdateCooldowns(float DeltaTime)
{
    ShootCooldownTimer = FMath::Max(ShootCooldownTimer - DeltaTime, 0.0f);
    FirstAbilityCooldownTimer = FMath::Max(FirstAbilityCooldownTimer - DeltaTime, 0.0f);
    SecondAbilityCooldownTimer = FMath::Max(SecondAbilityCooldownTimer - DeltaTime, 0.0f);
    ThirdAbilityCooldownTimer = FMath::Max(ThirdAbilityCooldownTimer - DeltaTime, 0.0f);
}

void UAbilityManager::UpdateAbilityBar(float DeltaTime)
{
    if (!CurrentAbilitySet) return;
    
    float FillRate = bInCombatZone ? CurrentAbilitySet->CombatBarFillRate : CurrentAbilitySet->BarFillRate;
    float MaxBar = GetMaxAbilityBar();
    
    if (AbilityBarFill < MaxBar)
    {
        AbilityBarFill = FMath::Min(AbilityBarFill + FillRate * DeltaTime, MaxBar);
    }
}

// GAS Integration Functions
void UAbilityManager::GrantAbilitySet()
{
    if (!AbilityComponent || !CurrentAbilitySet)
    {
        UE_LOG(LogTemp, Warning, TEXT("AbilityManager: Cannot grant abilities - missing component or ability set"));
        return;
    }

    // Remove any existing abilities first
    RemoveCurrentAbilities();

    // Grant abilities from the current set
    TArray<FAbilityData> Abilities = {
        CurrentAbilitySet->ShootAbility,
        CurrentAbilitySet->FirstAbility,
        CurrentAbilitySet->SecondAbility,
        CurrentAbilitySet->ThirdAbility
    };

    for (const FAbilityData& Ability : Abilities)
    {
        if (Ability.GameplayAbilityClass)
        {
            // Cast to UGameplayAbility for GAS compatibility
            UClass* AbilityClass = Ability.GameplayAbilityClass.Get();
            if (AbilityClass && AbilityClass->IsChildOf(UGameplayAbility::StaticClass()))
            {
                TSubclassOf<UGameplayAbility> GameplayAbilityClass = AbilityClass;
                FGameplayAbilitySpec AbilitySpec(GameplayAbilityClass, 1);
                AbilityComponent->GiveAbility(AbilitySpec);
                UE_LOG(LogTemp, Log, TEXT("AbilityManager: Granted ability %s"), *Ability.AbilityName);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("AbilityManager: Failed to cast ability class %s to UGameplayAbility"), *Ability.GameplayAbilityClass->GetName());
            }
        }
    }
}

void UAbilityManager::RemoveCurrentAbilities()
{
    if (!AbilityComponent)
    {
        return;
    }

    // Remove all abilities from the ability system component
    TArray<FGameplayAbilitySpecHandle> AbilityHandles;
    AbilityComponent->GetAllAbilities(AbilityHandles);
    
    for (FGameplayAbilitySpecHandle Handle : AbilityHandles)
    {
        AbilityComponent->ClearAbility(Handle);
    }
    
    UE_LOG(LogTemp, Log, TEXT("AbilityManager: Removed all current abilities"));
}

void UAbilityManager::ActivateAbilityByClass(TSubclassOf<UGameplayAbilityMasterComponent> AbilityClass)
{
    if (!AbilityComponent || !AbilityClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("AbilityManager: Cannot activate ability - missing component or class"));
        return;
    }

    // Cast to UGameplayAbility for GAS compatibility
    UClass* AbilityClassPtr = AbilityClass.Get();
    if (!AbilityClassPtr || !AbilityClassPtr->IsChildOf(UGameplayAbility::StaticClass()))
    {
        UE_LOG(LogTemp, Warning, TEXT("AbilityManager: Failed to cast ability class %s to UGameplayAbility"), *AbilityClass->GetName());
        return;
    }
    
    TSubclassOf<UGameplayAbility> GameplayAbilityClass = AbilityClassPtr;
    
    // Try to activate the ability through GAS
    FGameplayAbilitySpec* AbilitySpec = AbilityComponent->FindAbilitySpecFromClass(GameplayAbilityClass);
    if (AbilitySpec)
    {
        AbilityComponent->TryActivateAbility(AbilitySpec->Handle);
        UE_LOG(LogTemp, Log, TEXT("AbilityManager: Activated ability class %s"), *AbilityClass->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AbilityManager: Could not find ability handle for class %s"), *AbilityClass->GetName());
    }
}

void UAbilityManager::UseAbility(const FAbilityData& AbilityData)
{
    // This function is now deprecated since we're using GAS
    // Keep it for backward compatibility but log a warning
    UE_LOG(LogTemp, Warning, TEXT("AbilityManager: UseAbility called - this should be replaced with GAS activation"));
}
