Absolutely — let’s lock in **Week 1 tasks for DevA**, which is _you_, the **Lead Developer & Technical Architect**.

---

# 👤 DevA – Week 1 Focus: **Project Architecture, Multiplayer Foundation, and Team Enablement**

> 🎯 **Goal:** Set up the technical backbone of the project — clean class structure, game framework classes, and multiplayer session support.  
> You're not just coding — you're **designing systems** that empower every other dev to work cleanly and confidently.

---

## 🛠️ What DevA Will Work On (Week 1)

|System/Area|What You’ll Build|
|---|---|
|**Core Game Framework**|`GameMode`, `GameState`, `PlayerController`, `PlayerState`, `HUD` setup|
|**Multiplayer Session Base**|Basic session creation/join flow using EOS or Subsystem (placeholder)|
|**Project File & Module Setup**|Organize folders, target files, startup map, asset rules|
|**Replication Rules**|Design and document how each class/variable replicates|
|**Code Standards / Docs**|Shared macros, naming rules, Git structure, starter readme|

---

## ✅ Week 1 Task Breakdown

|**Task**|Class/File|Description|Est. Time|
|---|---|---|---|
|Create core gameplay classes|`ACGameMode`, `ACGameState`, etc.|Empty but structured classes for rest of team to build on|1 day|
|Define spawn rules & player controller logic|`PostLogin()`, `HandleStartingNewPlayer()`|Setup spawning logic to support MP flow|1 day|
|Implement basic session system (placeholder)|`ACMultiplayerSubsystem`, `CreateSession()`, etc.|Begin EOS or null subsystem flow|1 day|
|Set replication defaults|`bReplicates`, `SetIsReplicatedByDefault()` in key classes|Guide DevB/F with pattern|0.5 day|
|Project structure pass|Create folders, `.Target.cs`, fix includes, precompile blueprint base classes|0.5 day||
|Write team-facing doc|`README.md`, `CodingGuidelines.md`|How to name classes, how to use Git branches, how to add new components|0.5 day|

---

## 📦 Core Classes to Set Up

```cpp
Source/AerialCombat/Core/
├── ACGameMode.cpp/.h
├── ACGameState.cpp/.h
├── ACPlayerController.cpp/.h
├── ACPlayerState.cpp/.h
├── ACHUD.cpp/.h
```

|Class|Base Class|Purpose|
|---|---|---|
|`ACGameMode`|`AGameModeBase`|Controls match state, spawning, server-only logic|
|`ACGameState`|`AGameStateBase`|Tracks shared match data (score, time, etc.)|
|`ACPlayerController`|`APlayerController`|Input, HUD, session join/leave RPCs|
|`ACPlayerState`|`APlayerState`|Holds replicated data per player (kills, team, etc.)|
|`ACHUD`|`AHUD`|Manages UMG widgets|

---

## 🔁 EOS/Online Subsystem Setup (Stub)

You won’t fully build the system yet — just **stub the interface** so other devs (DevF, DevC) can later hook into joining/hosting logic.

Create:

```cpp
Source/AerialCombat/Multiplayer/
├── ACMultiplayerSubsystem.cpp/.h
```

Start basic function signatures:

```cpp
UCLASS()
class UACMultiplayerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    void CreateSession(int32 MaxPlayers);
    void FindSessions();
    void JoinSession(const FOnlineSessionSearchResult& Result);
};
```

🧠 **Tip:** Stub these to log to console first. Implement actual flow in Week 2.

---

## 🧠 WHY WE’RE DOING THIS (Selling the Architecture to the Team)

> "Why create all these separate classes? Why not just throw logic into one big controller?"

|Problem (All-in-One Blueprint Mess)|Solution (Modular Framework Classes)|
|---|---|
|No separation of server vs client logic|`GameMode` = server only, `Controller` = client only — Unreal is designed this way|
|UI logic mixed with game rules|`HUD` handles UI, `GameMode` handles rules — each has its own responsibility|
|Replication chaos later|`PlayerState` replicates automatically → cleaner multiplayer|
|Hard to debug player behavior|Splitting data by class makes debugging and testing easier|
|No scalability|This system works for 2 players or 200|

> 🛠️ You're not just coding. You're **engineering a gameplay platform** the rest of the team can plug into.

---

## 🧩 Suggested Class Responsibility Summary (For Internal Docs)

|Class|Owned By|Purpose|
|---|---|---|
|`ACGameMode`|DevA|Match control, spawning, server-side rules|
|`ACGameState`|DevA|Replicated match state (score, time, phase)|
|`ACPlayerController`|DevA|Input, UI hookup, session logic|
|`ACPlayerState`|DevA|Stores kill count, team, display name|
|`ACHUD`|DevA|Adds UMG widgets, references `MainHUDWidget`|
|`ACMultiplayerSubsystem`|DevA|Manages session creation/join/find logic|

---

## 📘 README/Dev Docs Contents

```markdown
# Aerial Combat – Dev Quick Start

## Git Workflow
- Branch per feature: `devB/aircraft`, `devF/lockon`
- Pull main every morning
- PRs must compile + not break map

## Project Architecture
- Core classes in `/Core/`
- Aircraft in `/Aircraft/`, each has modular components
- Input mappings in `/Input/`, using Enhanced Input
- UMG Widgets in `/UI/`

## Naming
- C++: `AClassName`, `UClassName`, `EEnumName`
- BP: `BP_Weapon_Missile`, `WBP_HUD_Main`
```

---

## ✅ DevA Week 1 Summary

|Deliverable|Description|
|---|---|
|All core framework classes|C++ base classes for GameMode, PlayerController, etc.|
|Multiplayer session stubs|Session creation/join method placeholders|
|Clean project folder structure|Standardized `Source/`, `Content/`, and naming conventions|
|Replication guide|Where to replicate vars/components — for DevB, DevF, DevC|
|Dev onboarding doc|README, code patterns, Git rules for team to follow|

---

## 📘 Bonus: Example Class Setup Snippet

```cpp
// ACPlayerController.cpp
void AACPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (IsLocalController())
    {
        ClientMessage(TEXT("Welcome to the sky!"));
    }
}
```

```cpp
// ACGameMode.cpp
void AACGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    UE_LOG(LogTemp, Warning, TEXT("Player joined: %s"), *NewPlayer->GetName());
}
```

---
