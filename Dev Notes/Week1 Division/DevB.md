# ✅ Week 1 Tasks for DevB: **Aircraft Movement & Flight System**

### 🎯 Role Focus
DevB is responsible for getting the **aircraft flying**, feeling good, and handling player input on PC and mobile. This is the _core moment-to-moment gameplay_.

## 🎮 Features to Build (Week 1)

|**Feature**|**File/Class**|**Status**|**Notes**|
|---|---|---|---|
|Aircraft base pawn|`AAircraftBase`|🛠️|Handles movement, rotation, and replication|
|Aircraft movement component|`UAircraftMovementComponent`|🛠️|Modular movement logic, throttle, pitch, yaw, roll|
|Player aircraft class|`APlayerAircraft` → child of `AAircraftBase`|🛠️|For player-specific input/camera logic|
|Input bindings|`InputAction_Fly`, `InputMappingContext` (DevC later)|🔜 (DevC)|DevC will connect Enhanced Input next|
|Blueprint exposure|`UPROPERTY()` / `BlueprintCallable`|✅|Expose throttle/speed vars to Blueprint|
|Dummy flight test map|`TestFlightMap.umap`|✅|Spawn aircraft, fly around, tweak physics|

## 🛠️ How to Structure This (Architecture Summary)

You’ll create:

### 1. `AAircraftBase` (C++) → Base Aircraft

> Base aircraft for both players and AI. Doesn't handle direct input.

```cpp
class AAircraftBase : public APawn
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UAircraftMovementComponent* MovementComp;

    virtual void BeginPlay() override;
};
```

---

### 2. `UAircraftMovementComponent` (C++) → Movement Logic

> Handles forward thrust, rotation (yaw/pitch/roll), boost, drag, max speed, etc.

```cpp
class UAircraftMovementComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    void AddThrottle(float Value);
    void AddPitch(float Value);
    void AddYaw(float Value);
    void AddRoll(float Value);
    
    void TickComponent(...); // Move aircraft
};
```

---

### 3. `APlayerAircraft` (C++) → Controlled by Human Players

> Inherits from `AAircraftBase`. Binds input. Calls movement functions.

```cpp
class APlayerAircraft : public AAircraftBase
{
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
```

---

### ✅ Task Checklist

|Task|Goal|Time Estimate|
|---|---|---|
|Create `AAircraftBase` & setup mesh|Base class and visual representation|3–4 hrs|
|Implement `UAircraftMovementComponent`|Modular movement for reuse|1–2 days|
|Create `APlayerAircraft` class|Player-specific subclass|3 hrs|
|Add basic controls via dummy input|Bind throttle, pitch, yaw|1 day|
|Set up test level for flight|Visual flight test|1 hr|
|Tweak movement values for feel|Speed, drag, rotation rate|1 day|

---

## 🤔 WHY SO COMPLICATED? (Why Not Just Do This in One Blueprint?)

Newer devs often ask:

> “Why not just do this all in one Blueprint with Tick nodes and input events?”

Here’s why the **modular, C++-driven architecture is better** — especially on a 3.5-week multiplayer project.

### 🔥 The Pitch to Newer Devs

|Problem (All-in-One BP)|Solution (Modular Architecture)|
|---|---|
|Hard to reuse or maintain|Each part (movement, input, weapons) is **isolated, reusable**, testable|
|Difficult to debug logic|Each system is in its own class → easier to find bugs|
|Tick-heavy BPs kill performance (esp. mobile)|C++ components are **faster**, no Blueprint overhead|
|Can’t scale to 4–8 players flying at once|Optimized movement, replication is **necessary** for multiplayer|
|Pain to add new features (e.g., boost, wind)|Add to the **MovementComponent** only — no rewiring big spaghetti BPs|
|No learning about real-world game dev|This is how real studios like Ubisoft, Respawn, Riot structure systems|

> 🎯 **We’re not overengineering. We’re setting up simple, reusable components that scale** — so we can add weapons, bots, team logic, and UI without tripping over our own work.

---

## 🧪 Example Scenario

> Want to make an **AI aircraft** use the same movement system?

With this system:

- Just create `AEnemyAircraft` → subclass of `AAircraftBase`
    
- Let AI Controller call `MovementComp->AddYaw()` just like the player
    
- ✅ Done. No copy-pasting, no hacking Blueprint logic.
    

---

## 🗂 Folder Structure (For DevB's Work)

```plaintext
Source/AerialCombat/
└── Aircraft/
    ├── AAircraftBase.cpp/.h
    ├── APlayerAircraft.cpp/.h
    └── Components/
        └── AircraftMovementComponent.cpp/.h
```

---

## 🧠 Tips for DevB

- Use **`UE_LOG(LogTemp, Warning, TEXT(...))`** to debug throttle/speed values.
    
- Use a **Blueprint child** of `APlayerAircraft` to tweak flight settings live.
    
- Ask DevC for the **input mapping asset** (they’ll do Enhanced Input later).
    
- Focus on _getting the "feel"_ of flight right by end of Week 1.
    

---

## ✅ Deliverables for End of Week 1 (DevB)

|Deliverable|Description|
|---|---|
|`AAircraftBase` & `APlayerAircraft`|Spawnable aircraft with correct mesh/component setup|
|`UAircraftMovementComponent`|Modular movement logic for pitch/yaw/roll/throttle|
|Test map|Aircraft flies in test level, responds to input|
|Basic tuning|Sensible flight controls for a mobile combat feel|

---

Would you like me to:
- Provide a **C++ template/stub** for each class ready to paste into UE?
- Generate a **Blueprint test aircraft** to validate your C++ work visually?
Let’s start strong — this movement system will carry the **entire game feel.**