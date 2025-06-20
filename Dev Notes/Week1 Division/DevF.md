Great — now let’s plan **Week 1 for DevF: AI & Gameplay Designer**.

---

# 👤 DevF – Week 1 Focus: **Missile Lock-On System & Combat Feedback Design**

> 🎯 **Goal:** Lay the foundation for **targeting and combat feel**, especially **missile lock-on logic**, player feedback systems, and prep for basic AI (week 2+).  
> You’re focused on systems that make combat feel **tactical and responsive**, giving players satisfying lock-on and firing mechanics — even before full AI is added.

---

## 🛠️ What DevF Will Work On (Week 1)

|System|What DevF Does|
|---|---|
|**Missile Lock-On System**|Logic to select and lock onto nearby enemy targets (players or bots)|
|**Lock-On Feedback Design**|Define how players know a lock is active — HUD icon, sound, glow effect|
|**Targeting Component**|Optional `UTargetingComponent` to encapsulate lock logic|
|**Basic Enemy Placeholder**|Add `AEnemyAircraft` or dummy pawn to test targeting|
|**Collaborate on Projectile Logic**|With DevB to define targeting → missile firing flow|

---

## ✅ Week 1 Task Breakdown

|**Task**|File/Class|Description|Est. Time|
|---|---|---|---|
|Design lock-on targeting logic|`UTargetingComponent` or in `APlayerAircraft`|Find nearby enemies within range and FOV|1.5 days|
|Implement lock-on timer & state|Enum state (e.g. Unlocked → Locking → Locked)|Smooth lock over time logic|1 day|
|Setup test target in level|Dummy actor tagged as “Enemy”|Used to test lock-on targeting system|0.5 day|
|Communicate with DevC for HUD feedback|Lock-on reticle, progress bar|Trigger UMG updates when locking/locked|0.5 day|
|Trigger VFX & sound with DevD|Notify when lock acquired/lost|Pulse VFX and lock sound|0.5 day|
|Prep `AEnemyAircraft` base|Inherits from `AAircraftBase`|Stub only — just enough to test targeting|0.5 day|

---

## 🧩 File/Class Structure

```plaintext
Source/AerialCombat/
├── Aircraft/
│   ├── AEnemyAircraft.cpp/.h      // Dummy test target
│   └── Components/
│       └── TargetingComponent.cpp/.h  // Optional
```

---

## 🎮 How Target Lock Works (Game Logic Overview)

### Lock-On Flow:

```plaintext
→ Player presses “Target” button
   → Find nearby valid enemies
     → If one is in FOV and range
        → Start lock timer (2 sec default)
           → On success: Lock achieved
              → Fire missile (homing on target)
```

|Concept|Details|
|---|---|
|**Max Lock Range**|~2000–3000 units|
|**FOV Cone Angle**|~45–60 degrees in front of aircraft|
|**Lock Time**|~2.0 seconds, can vary by weapon|
|**Reset Conditions**|Target leaves FOV, gets destroyed, player switches target|

---

## 🔥 WHY NOT JUST DO THIS IN A SINGLE BP?

> “Why not use a simple overlap sphere and call it a day?”

### 🔥 The Pitch: Selling Smart Combat Systems

|Problem (Naive BP-only Targeting)|Solution (Modular Lock System)|
|---|---|
|Can’t reuse lock-on for AI/bots later|Component-based = works for AI or player aircraft|
|No scalability for multiple target types|Add filtering (e.g., only lock missiles, not friendlies)|
|Hard to customize lock behavior|Component handles lock timer, FOV, input logic cleanly|
|Messy Blueprint dependencies|Logic is encapsulated in one place, clean interface|

> ✅ **You’re building a reusable system** — DevB and even AI aircraft can use the same targeting logic later.

---

## 🎯 Deliverables for End of Week 1 (DevF)

|Deliverable|Description|
|---|---|
|Lock-on logic|Can find and lock onto an enemy in front of the player|
|Feedback triggers|Communicates lock state to DevC (UI) and DevD (FX/Sound)|
|Target test actor|Dummy “enemy” aircraft that can be locked onto|
|Basic interaction flow|Player can aim → lock → fire → get confirmation|
|Optional component|TargetingComponent that wraps up lock-on logic for reuse|

---

## 🧪 Suggested Code Concepts (DevF)

### Target Filtering:

```cpp
TArray<AActor*> Candidates;
UGameplayStatics::GetAllActorsWithTag(World, FName("Enemy"), Candidates);

// Filter by FOV and distance
```

### Lock State Enum:

```cpp
enum class ETargetLockState
{
    Unlocked,
    Locking,
    Locked
};
```

### TargetingComponent Interface:

```cpp
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UTargetingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    void StartTargeting();
    void TickTargeting(float DeltaTime);
    AActor* GetCurrentTarget() const;
};
```

---

## 🔁 Collaboration Touchpoints

|Collaborator|What You Need From Them|What You Provide|
|---|---|---|
|**DevB**|Aircraft/Weapon interface (call missile fire)|Target actor or lock-on state|
|**DevC**|UI widget that shows lock bar or reticle|Send lock progress and status|
|**DevD**|VFX trigger point (ring or pulse)|Call event when lock is achieved|

---

## 💡 Tips for DevF

- Use `DrawDebugCone()` to visualize FOV in testing
    
- Tag enemy aircraft with `"Enemy"` tag to simplify filtering
    
- Use a `TWeakObjectPtr<AActor>` to track lock target safely (avoid crashes if destroyed)
    
- Expose `UFUNCTION(BlueprintCallable)` for UI to read lock status
    

---

## ✅ DevF Summary

By end of Week 1, you’ll have:

- A **working missile lock-on system**
    
- Visual and audio **feedback hooks**
    
- A **modular base** for future AI aircraft and lock-based weapons
    

---

Would you like:

- A helper C++ targeting cone snippet?
    
- A test map setup with target dummy aircraft?
    
- A lock-on state diagram or flowchart to visualize logic?
    

Let me know, and I’ll prep the tools for you next.