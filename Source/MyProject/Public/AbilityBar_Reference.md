# Ability Bar Quick Reference

## Bar Fill Rates by Set

| Set | Base Rate | Combat Rate | Kill Reward | Zone Reward |
|-----|-----------|-------------|-------------|-------------|
| 🔥 Fire | 6.0/s | 10.0/s | 25 | 15 |
| ⚡ Lightning | 7.0/s | 11.0/s | 28 | 18 |
| 🩸 Bloodsteal | 8.0/s | 12.0/s | 30 | 20 |
| ❄️ Ice | 5.0/s | 8.0/s | 22 | 12 |
| 🌑 Shadow | 9.0/s | 13.0/s | 35 | 25 |
| 🤖 Tech | 6.5/s | 10.5/s | 27 | 17 |

## Bar Costs by Ability Type

| Ability | Cost Range | Typical Cost |
|---------|------------|--------------|
| Shoot | 0 | Always Free |
| 1st | 20-35 | 25-30 |
| 2nd | 30-45 | 35-40 |
| 3rd | 50-75 | 60-70 |

## Combat Events

### Bar Fill Events
- **Kill**: +22 to +35 bar (varies by set)
- **Zone Capture**: +12 to +25 bar (varies by set)
- **Combat Zone**: 1.5x-1.8x fill rate multiplier

### Bar Drain Events
- **1st Ability Use**: -20 to -35 bar
- **2nd Ability Use**: -30 to -45 bar
- **3rd Ability Use**: -50 to -75 bar

## Implementation Notes

### Bar Fill Logic
```cpp
// In UAbilityManager::Tick()
float DeltaFill = CurrentAbilitySet->BarFillRate * DeltaTime;

// Apply combat multiplier if in combat zone
if (bInCombatZone)
{
    DeltaFill *= CurrentAbilitySet->CombatBarFillRate / CurrentAbilitySet->BarFillRate;
}

ModifyAbilityBar(DeltaFill);
```

### Event Handlers
```cpp
// On kill
void OnKill()
{
    float KillReward = CurrentAbilitySet->KillBarReward;
    ModifyAbilityBar(KillReward);
}

// On zone capture
void OnZoneCapture()
{
    float ZoneReward = CurrentAbilitySet->ZoneCaptureBarReward;
    ModifyAbilityBar(ZoneReward);
}
```

## Balance Guidelines

### Fill Rate Balance
- **Fast Sets** (Shadow, Bloodsteal): 8-9/s base
- **Medium Sets** (Fire, Tech, Lightning): 6-7/s base
- **Slow Sets** (Ice): 5/s base

### Cost Balance
- **1st Ability**: Should be usable every 3-5 seconds
- **2nd Ability**: Should be usable every 5-8 seconds
- **3rd Ability**: Should be usable every 8-12 seconds

### Reward Balance
- **Kill Reward**: Should allow 1-2 ability uses
- **Zone Reward**: Should allow 1 ability use
- **Combat Multiplier**: 1.5x-1.8x for engagement incentive
