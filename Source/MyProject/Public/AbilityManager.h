#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataAsset.h"
#include "AbilityManager.generated.h"

// Forward declarations
class UAbilityComponent;
class AADroneBase;
class UGameplayAbilityMasterComponent;

/**
 * Individual ability data structure
 */
USTRUCT(BlueprintType)
struct MYPROJECT_API FAbilityData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    FString AbilityName = TEXT("Default Ability");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    float Damage = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    float Range = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    float Cooldown = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    float AbilityBarCost = 25.0f; // How much of the ability bar this ability uses

    	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
	TSubclassOf<UGameplayAbilityMasterComponent> GameplayAbilityClass = nullptr; // GAS ability class

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    bool bRequiresAbilityBar = true; // Whether this ability requires ability bar to be filled

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
    FString Description = TEXT("Default ability description");
};

/**
 * Ability Set Data Asset - Contains all abilities for a specific set
 */
UCLASS(BlueprintType)
class MYPROJECT_API UAbilitySetDataAsset : public UDataAsset
{   
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Set")
    FString SetName = TEXT("Default Set");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Set")
    FString SetDescription = TEXT("Default ability set description");

    // Shoot ability (always available)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot Ability")
    FAbilityData ShootAbility;

    // Special abilities (1st, 2nd, 3rd)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Abilities")
    FAbilityData FirstAbility;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Abilities")
    FAbilityData SecondAbility;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Special Abilities")
    FAbilityData ThirdAbility;

    // Ability bar settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Bar")
    float MaxAbilityBar = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Bar")
    float BarFillRate = 5.0f; // Points per second when not in combat

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Bar")
    float CombatBarFillRate = 10.0f; // Points per second when in combat

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Bar")
    float KillBarReward = 25.0f; // Bar points gained per kill

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Bar")
    float ZoneCaptureBarReward = 15.0f; // Bar points gained per zone capture
};

/**
 * Main Ability Manager Component
 */
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UAbilityManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UAbilityManager();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // Ability Set Management
    UFUNCTION(BlueprintCallable, Category = "Ability Manager")
    void SetAbilitySet(UAbilitySetDataAsset* NewAbilitySet);

    UFUNCTION(BlueprintPure, Category = "Ability Manager")
    UAbilitySetDataAsset* GetCurrentAbilitySet() const { return CurrentAbilitySet; }

    // Ability Bar Management
    UFUNCTION(BlueprintCallable, Category = "Ability Bar")
    void IncreaseAbilityBar(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Ability Bar")
    void ConsumeAbilityBar(float Amount);

    UFUNCTION(BlueprintPure, Category = "Ability Bar")
    float GetAbilityBarFill() const { return AbilityBarFill; }

    UFUNCTION(BlueprintPure, Category = "Ability Bar")
    float GetNormalizedAbilityBar() const { return AbilityBarFill / GetMaxAbilityBar(); }

    UFUNCTION(BlueprintPure, Category = "Ability Bar")
    float GetMaxAbilityBar() const;

    // Ability Usage (Server-side) - Now using GAS
    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void UseShoot();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void UseFirstAbility();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void UseSecondAbility();

    UFUNCTION(BlueprintCallable, Category = "Abilities")
    void UseThirdAbility();

    // Combat Events (for ability bar rewards)
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void OnKill();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void OnZoneCapture();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void OnEnterCombatZone();

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void OnExitCombatZone();

    // Getters for ability data
    UFUNCTION(BlueprintPure, Category = "Abilities")
    FAbilityData GetShootAbility() const;

    UFUNCTION(BlueprintPure, Category = "Abilities")
    FAbilityData GetFirstAbility() const;

    UFUNCTION(BlueprintPure, Category = "Abilities")
    FAbilityData GetSecondAbility() const;

    UFUNCTION(BlueprintPure, Category = "Abilities")
    FAbilityData GetThirdAbility() const;

    // Cooldown management
    UFUNCTION(BlueprintPure, Category = "Abilities")
    bool CanUseShoot() const;

    UFUNCTION(BlueprintPure, Category = "Abilities")
    bool CanUseFirstAbility() const;

    UFUNCTION(BlueprintPure, Category = "Abilities")
    bool CanUseSecondAbility() const;

    UFUNCTION(BlueprintPure, Category = "Abilities")
    bool CanUseThirdAbility() const;

    // Cooldown progress (0.0 to 1.0)
    UFUNCTION(BlueprintPure, Category = "Abilities")
    float GetShootCooldownProgress() const;

    UFUNCTION(BlueprintPure, Category = "Abilities")
    float GetFirstAbilityCooldownProgress() const;

    UFUNCTION(BlueprintPure, Category = "Abilities")
    float GetSecondAbilityCooldownProgress() const;

    UFUNCTION(BlueprintPure, Category = "Abilities")
    float GetThirdAbilityCooldownProgress() const;

    // GAS Integration Functions
    UFUNCTION(BlueprintCallable, Category = "GAS")
    void GrantAbilitySet();

    UFUNCTION(BlueprintCallable, Category = "GAS")
    void RemoveCurrentAbilities();

    UFUNCTION(BlueprintCallable, Category = "GAS")
    void ActivateAbilityByClass(TSubclassOf<UGameplayAbilityMasterComponent> AbilityClass);

protected:
    // Current ability set
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Ability Manager")
    UAbilitySetDataAsset* CurrentAbilitySet;

    // Ability bar
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Ability Bar")
    float AbilityBarFill = 0.0f;

    // Combat state
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Combat")
    bool bInCombatZone = false;

    // Cooldown timers
    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooldowns")
    float ShootCooldownTimer = 0.0f;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooldowns")
    float FirstAbilityCooldownTimer = 0.0f;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooldowns")
    float SecondAbilityCooldownTimer = 0.0f;

    UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Cooldowns")
    float ThirdAbilityCooldownTimer = 0.0f;

private:
    // Helper functions
    void UpdateCooldowns(float DeltaTime);
    void UpdateAbilityBar(float DeltaTime);
    bool CanUseAbility(const FAbilityData& AbilityData, float CooldownTimer) const;
    void UseAbility(const FAbilityData& AbilityData);

    // Owner references
    UPROPERTY()
    AADroneBase* OwnerDrone = nullptr;

    UPROPERTY()
    UAbilityComponent* AbilityComponent = nullptr;
};
