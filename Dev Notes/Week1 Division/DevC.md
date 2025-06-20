
## 👤 DevC – Week 1 Focus: **UI & Input Systems**

> 🎯 Goal: Build the **foundation of player interaction** through **HUD and touch input** using **UMG + Enhanced Input**.  
> These systems allow players to control aircraft and get visual feedback — critical for mobile playability and user experience.

---

## 🛠️ What DevC Will Work On (Week 1)

|System|What DevC Does|
|---|---|
|**Enhanced Input Setup**|Create modular input actions (Throttle, Pitch, Yaw, Fire) for player control|
|**Input Mapping Context**|Design mobile-friendly bindings: joystick, fire button, etc.|
|**Touch Input UI**|Create a UMG widget for on-screen joystick and buttons|
|**Basic HUD**|Design and implement a minimal HUD (health, throttle meter, ammo count)|
|**HUD Integration**|Ensure the `ACHUD` class can add the UI to screen and update dynamically|

---

## 🎮 Week 1 Objectives – Task Breakdown

|**Task**|Class/File|Notes|Est. Time|
|---|---|---|---|
|Set up Enhanced Input plugin & asset structure|`InputConfigDataAsset`, `InputMappingContext`, InputActions|Will drive all game controls (futureproof)|0.5 day|
|Create Input Actions|`InputAction_Pitch`, `InputAction_Throttle`, etc.|Separate input actions for axis & button inputs|0.5 day|
|Build `TouchInputWidget` in UMG|`/UI/TouchInputWidget`|Joystick on left, Fire/Boost buttons on right|1 day|
|Implement Enhanced Input in `APlayerAircraft`|In collaboration with DevB|Bind movement input to movement functions|0.5 day|
|Create `MainHUDWidget` base UI|`/UI/MainHUDWidget`|Add health bar, speed, reticle, weapon icon|1 day|
|Update `ACHUD` to add HUD widget|`ACHUD.cpp/.h`|Spawns and attaches `MainHUDWidget`|0.5 day|
|Connect UMG values to variables|Use `BindWidget`, Update from C++|e.g., Health % → ProgressBar, Speed → Text|1 day|

---

## 🧩 File/Asset Structure

```plaintext
Source/AerialCombat/
├── UI/
│   ├── TouchInputWidget.uasset         (UMG)
│   ├── MainHUDWidget.uasset            (UMG)
│   └── AircraftStatusWidget.uasset     (UMG - optional detail panel)
├── Input/
│   ├── InputMappingContext_Flight.uasset
│   ├── InputAction_Pitch.uasset
│   ├── InputAction_Throttle.uasset
│   └── InputAction_Fire.uasset
Core/
├── ACHUD.cpp/.h
```

---

## 🧠 WHY ARE WE DOING IT THIS WAY?

### 🤔 "Why not just use regular input events in the player Blueprint?"

**Good question!** Newer devs often ask this. Here's why we’re doing it modularly using **Enhanced Input System** and **separate UMG widgets**:


| Problem (Old One BlueprintOnly Method)   | Solution (Modern Modular Approach)                                        |
| ---------------------------------------- | ------------------------------------------------------------------------- |
| Only one control scheme (no flexibility) | Easily supports **multiple input contexts** (touch/gamepad/keyboard)      |
| Hard to reuse input logic                | **Centralized system** of `InputActions` that can be reused/extended      |
| Blueprint spaghetti logic                | Clean separation between UI (UMG), logic (HUD class), and data            |
| Touch buttons delay or misfire           | Enhanced Input supports **digital, analog, and custom gestures**          |
| UI gets bloated & unreadable             | Using **modular widgets** makes each HUD piece easy to maintain           |
| Pain to integrate for mobile             | Blueprint Joystick input → Enhanced Input handles touch gestures natively |

> ✨ **Enhanced Input + Modular UMG = Cross-platform ready, scalable, and maintainable UI and controls.**

This is **how Fortnite does it**, and most studios building for mobile.

---

## 💡 Learning Opportunities for DevC

|Topic|What You’ll Learn|
|---|---|
|**Enhanced Input**|UE5’s new input system — modular, scalable, and futureproof|
|**UMG Binding**|How to bind C++ variables (e.g., speed, ammo) to UI widgets|
|**C++ ↔ Blueprint communication**|Use `UFUNCTION(BlueprintCallable)` to let UI talk to game code|
|**HUD Class Structure**|How `ACHUD` creates and controls screen widgets|

## 🧪 DevC & DevB Collaboration Touchpoints

|System|DevC Responsibilities|DevB Responsibilities|
|---|---|---|
|Input Binding|Create input actions & UMG elements|Implement input handling in Aircraft class|
|Movement Feedback (HUD)|Show throttle/speed on UI|Send speed info via C++ variables|
|Health/Ammo HUD|Build progress bars & counters|Hook up C++ events to trigger UI updates|

---

## ✅ Deliverables for End of Week 1 (DevC)

|Deliverable|Description|
|---|---|
|`InputMappingContext`|Input config asset with mappings for all aircraft controls|
|`TouchInputWidget`|UMG widget with joystick + buttons, working on test map|
|`MainHUDWidget`|Health bar, throttle, reticle, weapon icon — functional & clean|
|`ACHUD` update|C++ HUD class creates and adds HUD/Touch widgets|
|Input system hookup|Connected to `APlayerAircraft` (via DevB)|

---

## 🧰 Tips for DevC

- Use `BindWidget` in your `UUserWidget` to connect C++ and Blueprint
    
- Add debug print logs to test Enhanced Input bindings (`UE_LOG`)
    
- Ask DevB for exposed speed/health variables to test HUD bindings
    
- Keep widget styles minimal — DevD will polish visuals later
    

---

### ✅ Bonus: C++ Helper Example for ACHUD

```cpp
// ACHUD.cpp
void AACHUD::BeginPlay()
{
    Super::BeginPlay();

    if (MainHUDClass)
    {
        UMainHUDWidget* HUD = CreateWidget<UMainHUDWidget>(GetWorld(), MainHUDClass);
        if (HUD)
        {
            HUD->AddToViewport();
        }
    }
}
```

---

## 🎯 End Goal for DevC (Week 1)

> “Player can touch a joystick + fire button and fly around while seeing a basic HUD showing health and speed.”

**Everything will evolve — but this is your foundation.**

---

Would you like:

- A UMG wireframe design for the HUD?
    
- Pre-made placeholder assets for joysticks/buttons?
    

Let me know, and I can spin those up next to keep DevC moving efficiently!