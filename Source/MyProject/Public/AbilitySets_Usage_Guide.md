# Ability Sets Usage Guide

## Overview
The ability system provides 6 different ability sets, each with unique gameplay styles and mechanics. Each set contains 4 abilities: Shoot (free), 1st, 2nd, and 3rd (cost ability bar).

## Available Ability Sets

### 🔥 Fire Set
**Theme**: High damage, aggressive playstyle
- **Shoot**: Fire Bullets (20 damage, 1000 range, 0.15s cooldown)
- **1st**: Fireball (35 damage, 800 range, 25 bar cost)
- **2nd**: Flame Burst (30 damage, 300 range, 35 bar cost)
- **3rd**: Firestorm (75 damage, 500 range, 70 bar cost)
- **Bar Fill Rate**: 6.0/s (10.0/s in combat)

### ⚡ Lightning Set
**Theme**: Fast-paced, chain damage
- **Shoot**: Lightning Bolts (12 damage, 1200 range, 0.1s cooldown)
- **1st**: Chain Lightning (25 damage, 600 range, 20 bar cost)
- **2nd**: Electric Field (20 damage, 400 range, 30 bar cost)
- **3rd**: Thunderstorm (60 damage, 600 range, 50 bar cost)
- **Bar Fill Rate**: 7.0/s (11.0/s in combat)

### 🩸 Bloodsteal Set
**Theme**: Life steal, defensive
- **Shoot**: Blood Bullets (15 damage, 800 range, 0.2s cooldown)
- **1st**: Blood Barrier (0 damage, defensive, 30 bar cost)
- **2nd**: Blood Drain (25 damage, 600 range, 40 bar cost)
- **3rd**: Blood Explosion (50 damage, 400 range, 60 bar cost)
- **Bar Fill Rate**: 8.0/s (12.0/s in combat)

### ❄️ Ice Set
**Theme**: Control, slowing effects
- **Shoot**: Ice Shards (18 damage, 900 range, 0.25s cooldown)
- **1st**: Ice Wall (0 damage, defensive, 35 bar cost)
- **2nd**: Frost Nova (30 damage, 350 range, 40 bar cost)
- **3rd**: Blizzard (45 damage, 600 range, 65 bar cost)
- **Bar Fill Rate**: 5.0/s (8.0/s in combat)

### 🌑 Shadow Set
**Theme**: Stealth, mobility
- **Shoot**: Shadow Bolts (16 damage, 1100 range, 0.2s cooldown)
- **1st**: Shadow Step (teleport, 25 bar cost)
- **2nd**: Shadow Clone (20 damage, 300 range, 45 bar cost)
- **3rd**: Void Explosion (80 damage, 400 range, 75 bar cost)
- **Bar Fill Rate**: 9.0/s (13.0/s in combat)

### 🤖 Tech Set
**Theme**: High-tech, gadgets
- **Shoot**: Plasma Rifle (22 damage, 1200 range, 0.12s cooldown)
- **1st**: Combat Drone (15 damage, 800 range, 30 bar cost)
- **2nd**: Shield Generator (0 damage, defensive, 40 bar cost)
- **3rd**: EMP Blast (60 damage, 500 range, 70 bar cost)
- **Bar Fill Rate**: 6.5/s (10.5/s in combat)

## Ability Bar Mechanics

### Bar Fill Sources
- **Passive**: Each set has different base fill rates
- **Combat**: Increased fill rate when in combat zones
- **Kills**: Bonus bar fill (varies by set: 22-35)
- **Zone Capture**: Bonus bar fill (varies by set: 12-25)

### Bar Costs
- **Shoot**: Always free (0 cost)
- **1st Ability**: 20-35 bar cost
- **2nd Ability**: 30-45 bar cost  
- **3rd Ability**: 50-75 bar cost

## Setup Instructions

### 1. Create Ability Set Data Assets
1. Right-click in Content Browser
2. Select "Miscellaneous" → "Data Asset"
3. Choose the desired ability set class (e.g., `FireAbilitySet`)
4. Name and save the asset

### 2. Assign to Drone/Aircraft
1. Open your drone or aircraft Blueprint
2. Find the "Default Ability Set" property
3. Assign your created ability set data asset

### 3. Create Ability Classes (TODO)
Each ability set needs corresponding `UGameplayAbility` classes:
- Create child classes of `UGameplayAbilityMasterComponent`
- Implement the actual ability logic in `ActivateAbility()`
- Assign the classes to the data asset

## Example Ability Class Structure

```cpp
UCLASS()
class MYPROJECT_API UFireballAbility : public UGameplayAbilityMasterComponent
{
    GENERATED_BODY()

protected:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
                                const FGameplayAbilityActorInfo* ActorInfo, 
                                const FGameplayAbilityActivationInfo ActivationInfo, 
                                const FGameplayEventData* TriggerEventData) override
    {
        // Implement fireball logic here
        // - Spawn projectile
        // - Apply damage
        // - Visual effects
    }
};
```

## Balance Considerations

### Damage Scaling
- **Shoot**: 12-22 damage (fast, low damage)
- **1st**: 0-35 damage (utility or moderate damage)
- **2nd**: 20-30 damage (moderate damage, utility)
- **3rd**: 45-80 damage (high damage, long cooldown)

### Range Scaling
- **Shoot**: 800-1200 range
- **1st**: 200-800 range
- **2nd**: 300-400 range
- **3rd**: 400-600 range

### Cooldown Scaling
- **Shoot**: 0.1-0.25s (very fast)
- **1st**: 5-20s (moderate)
- **2nd**: 8-18s (moderate)
- **3rd**: 18-30s (long)

## Tips for Implementation

1. **Start Simple**: Begin with just the shoot ability
2. **Test Balance**: Adjust damage, cooldowns, and bar costs
3. **Visual Feedback**: Add particle effects and sounds
4. **Network Replication**: Ensure abilities work in multiplayer
5. **UI Integration**: Show ability bar, cooldowns, and costs

## Next Steps

1. Create the individual ability classes
2. Implement projectile spawning and damage
3. Add visual and audio effects
4. Create UI elements for ability bar
5. Test and balance all sets
