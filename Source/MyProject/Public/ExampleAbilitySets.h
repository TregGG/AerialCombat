#pragma once

#include "CoreMinimal.h"
#include "AbilityManager.h"
#include "Engine/DataAsset.h"
#include "ExampleAbilitySets.generated.h"

/**
 * Bloodsteal Ability Set
 * - Steals health from enemies
 * - Blood barrier for defense
 * - Blood explosion for AOE
 */
UCLASS(BlueprintType)
class MYPROJECT_API UBloodstealAbilitySet : public UAbilitySetDataAsset
{
    GENERATED_BODY()

public:
    UBloodstealAbilitySet()
    {
        SetName = TEXT("Bloodsteal Set");
        SetDescription = TEXT("A dark set that steals life from enemies");
        
        // Shoot ability - Blood bullets
        ShootAbility.AbilityName = TEXT("Blood Bullets");
        ShootAbility.Damage = 15.0f;
        ShootAbility.Range = 800.0f;
        ShootAbility.Cooldown = 0.2f;
        ShootAbility.AbilityBarCost = 0.0f; // Free to use
        ShootAbility.bRequiresAbilityBar = false;
        ShootAbility.Description = TEXT("Fires blood-infused bullets that steal health");
        
        // First ability - Blood barrier
        FirstAbility.AbilityName = TEXT("Blood Barrier");
        FirstAbility.Damage = 0.0f;
        FirstAbility.Range = 0.0f;
        FirstAbility.Cooldown = 8.0f;
        FirstAbility.AbilityBarCost = 30.0f;
        FirstAbility.bRequiresAbilityBar = true;
        FirstAbility.Description = TEXT("Creates a protective barrier of blood");
        
        // Second ability - Blood drain
        SecondAbility.AbilityName = TEXT("Blood Drain");
        SecondAbility.Damage = 25.0f;
        SecondAbility.Range = 600.0f;
        SecondAbility.Cooldown = 12.0f;
        SecondAbility.AbilityBarCost = 40.0f;
        SecondAbility.bRequiresAbilityBar = true;
        SecondAbility.Description = TEXT("Drains life from nearby enemies");
        
        // Third ability - Blood explosion
        ThirdAbility.AbilityName = TEXT("Blood Explosion");
        ThirdAbility.Damage = 50.0f;
        ThirdAbility.Range = 400.0f;
        ThirdAbility.Cooldown = 20.0f;
        ThirdAbility.AbilityBarCost = 60.0f;
        ThirdAbility.bRequiresAbilityBar = true;
        ThirdAbility.Description = TEXT("Explodes in a burst of blood, damaging all nearby enemies");
        
        // Ability bar settings
        MaxAbilityBar = 100.0f;
        BarFillRate = 8.0f;
        CombatBarFillRate = 12.0f;
        KillBarReward = 30.0f;
        ZoneCaptureBarReward = 20.0f;
    }
};

/**
 * Fire Ability Set
 * - High damage fire projectiles
 * - Fireball for single target
 * - Firestorm for AOE
 */
UCLASS(BlueprintType)
class MYPROJECT_API UFireAbilitySet : public UAbilitySetDataAsset
{
    GENERATED_BODY()

public:
    UFireAbilitySet()
    {
        SetName = TEXT("Fire Set");
        SetDescription = TEXT("A destructive set focused on fire damage");
        
        // Shoot ability - Fire bullets
        ShootAbility.AbilityName = TEXT("Fire Bullets");
        ShootAbility.Damage = 20.0f;
        ShootAbility.Range = 1000.0f;
        ShootAbility.Cooldown = 0.15f;
        ShootAbility.AbilityBarCost = 0.0f;
        ShootAbility.bRequiresAbilityBar = false;
        ShootAbility.Description = TEXT("Fires incendiary bullets that burn enemies");
        
        // First ability - Fireball
        FirstAbility.AbilityName = TEXT("Fireball");
        FirstAbility.Damage = 35.0f;
        FirstAbility.Range = 800.0f;
        FirstAbility.Cooldown = 6.0f;
        FirstAbility.AbilityBarCost = 25.0f;
        FirstAbility.bRequiresAbilityBar = true;
        FirstAbility.Description = TEXT("Launches a powerful fireball at enemies");
        
        // Second ability - Flame burst
        SecondAbility.AbilityName = TEXT("Flame Burst");
        SecondAbility.Damage = 30.0f;
        SecondAbility.Range = 300.0f;
        SecondAbility.Cooldown = 10.0f;
        SecondAbility.AbilityBarCost = 35.0f;
        SecondAbility.bRequiresAbilityBar = true;
        SecondAbility.Description = TEXT("Releases a burst of flames in all directions");
        
        // Third ability - Firestorm
        ThirdAbility.AbilityName = TEXT("Firestorm");
        ThirdAbility.Damage = 75.0f;
        ThirdAbility.Range = 500.0f;
        ThirdAbility.Cooldown = 25.0f;
        ThirdAbility.AbilityBarCost = 70.0f;
        ThirdAbility.bRequiresAbilityBar = true;
        ThirdAbility.Description = TEXT("Creates a devastating firestorm that burns all enemies");
        
        // Ability bar settings
        MaxAbilityBar = 100.0f;
        BarFillRate = 6.0f;
        CombatBarFillRate = 10.0f;
        KillBarReward = 25.0f;
        ZoneCaptureBarReward = 15.0f;
    }
};

/**
 * Lightning Ability Set
 * - Fast electric projectiles
 * - Chain lightning
 * - Thunderstorm
 */
UCLASS(BlueprintType)
class MYPROJECT_API ULightningAbilitySet : public UAbilitySetDataAsset
{
    GENERATED_BODY()

public:
    ULightningAbilitySet()
    {
        SetName = TEXT("Lightning Set");
        SetDescription = TEXT("A fast-paced set focused on lightning and electricity");
        
        // Shoot ability - Lightning bolts
        ShootAbility.AbilityName = TEXT("Lightning Bolts");
        ShootAbility.Damage = 12.0f;
        ShootAbility.Range = 1200.0f;
        ShootAbility.Cooldown = 0.1f;
        ShootAbility.AbilityBarCost = 0.0f;
        ShootAbility.bRequiresAbilityBar = false;
        ShootAbility.Description = TEXT("Fires rapid lightning bolts at enemies");
        
        // First ability - Chain lightning
        FirstAbility.AbilityName = TEXT("Chain Lightning");
        FirstAbility.Damage = 25.0f;
        FirstAbility.Range = 600.0f;
        FirstAbility.Cooldown = 5.0f;
        FirstAbility.AbilityBarCost = 20.0f;
        FirstAbility.bRequiresAbilityBar = true;
        FirstAbility.Description = TEXT("Lightning that chains between multiple enemies");
        
        // Second ability - Electric field
        SecondAbility.AbilityName = TEXT("Electric Field");
        SecondAbility.Damage = 20.0f;
        SecondAbility.Range = 400.0f;
        SecondAbility.Cooldown = 8.0f;
        SecondAbility.AbilityBarCost = 30.0f;
        SecondAbility.bRequiresAbilityBar = true;
        SecondAbility.Description = TEXT("Creates an electric field that damages nearby enemies");
        
        // Third ability - Thunderstorm
        ThirdAbility.AbilityName = TEXT("Thunderstorm");
        ThirdAbility.Damage = 60.0f;
        ThirdAbility.Range = 600.0f;
        ThirdAbility.Cooldown = 18.0f;
        ThirdAbility.AbilityBarCost = 50.0f;
        ThirdAbility.bRequiresAbilityBar = true;
        ThirdAbility.Description = TEXT("Summons a thunderstorm that strikes all enemies in range");
        
        // Ability bar settings
        MaxAbilityBar = 100.0f;
        BarFillRate = 7.0f;
        CombatBarFillRate = 11.0f;
        KillBarReward = 28.0f;
        ZoneCaptureBarReward = 18.0f;
    }
};
