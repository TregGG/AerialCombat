# 👤 DevD – Week 1 Focus: **VFX & Audio Foundations**

> 🎯 Goal: Set up the **base visual and audio feedback** systems — this includes engine trails, missile lock effects, basic explosion VFX, and placeholder sounds.  
> This brings the game world **to life**, making movement and combat **feel powerful and responsive**.

---

## 🛠️ What DevD Will Work On (Week 1)

|System|What DevD Does|
|---|---|
|**Engine VFX**|Jet trail or smoke from aircraft engines (looping, Niagara)|
|**Missile Lock FX**|Circular target lock HUD effect or mesh glow effect (optional)|
|**Impact VFX**|Placeholder explosion when a projectile hits|
|**Basic Audio Setup**|Add placeholder SFX (engine hum, weapon fire, explosion)|
|**Audio/VFX Triggers**|Hook VFX/SFX into gameplay events via `BlueprintImplementableEvent` or Notify|

---

## ✅ Week 1 Task Breakdown

|**Task**|File/Asset|Description|Est. Time|
|---|---|---|---|
|Create **Engine Trail VFX**|`NiagaraSystem_EngineTrail`|Looping Niagara effect (smoke trail, glow, afterburner)|1 day|
|Add VFX component to aircraft|Add to `AAircraftBase`|Attach Niagara component to socket or offset|0.5 day|
|Create **Missile Lock VFX**|`NiagaraSystem_LockOn`|Circular pulse or hologram effect to show locked target|1 day|
|Create **Projectile Impact FX**|`NiagaraSystem_Impact`|Quick, fiery explosion VFX for when a missile hits|0.5 day|
|Import placeholder **SFX assets**|`SFX_Engine`, `SFX_Fire`, etc.|Get from freesound.org or similar for testing|1 hr|
|Setup **SFX playback**|Add `USoundCue` triggers|Add to aircraft for engine hum, to weapon for fire/explosion|1 day|

---

## 🎥 Visual Implementation Locations

|VFX/SFX|Attached To|Triggered By|
|---|---|---|
|Engine Trail|`AAircraftBase`|Looping effect on movement start|
|Missile Lock VFX|`AWeaponBase` or HUD|Triggered when missile acquires lock|
|Projectile Impact VFX|`AProjectile`|On `OnHit()` or `OnExplode()`|
|Engine Sound|`AAircraftBase`|Loop based on throttle input|
|Weapon Fire Sound|`AWeaponBase`|On Fire (DevB may call a function/event)|
|Explosion Sound|`AProjectile`|On impact|

---

## 🧠 WHY NOT JUST DO VFX IN BLUEPRINT ACTORS?

> "Why go through Niagara and sound cues? Just play particle system in Blueprint."

|Problem (Quick Blueprint FX)|Solution (Niagara + Modular Audio)|
|---|---|
|Poor performance & control|Niagara can be pooled, optimized for mobile|
|Not reusable|FX becomes **assets**, not logic spaghetti|
|Hard to tweak per aircraft/weapon|Each aircraft/weapon can have different FX slots|
|Difficult to sync with events|Blueprint events can trigger FX cleanly (via Notify/Event)|
|Not network-aware|Niagara and audio handled **server/client safe** when modular|
|Pain to scale (e.g., fire trail, contrails)|Just add more Niagara emitters to component or actor structure|

> 🔊 **Sound & VFX aren't just decoration — they're feedback systems.**  
> When you fly, you should _feel the heat_, _hear the speed_, and _see the burn._

---

## 🔁 Collaboration Touchpoints

|Collaborator|What They Provide|What You Do With It|
|---|---|---|
|**DevB**|Aircraft & weapon C++ base classes|Attach VFX/SFX components|
|**DevF**|Projectile/lock-on logic|Trigger effects for missile lock or hit|
|**DevC**|HUD widgets|Sync HUD indicators with missile targeting|

---

## ✅ Deliverables for End of Week 1 (DevD)

|Deliverable|Description|
|---|---|
|`NiagaraSystem_EngineTrail`|Looped jet trail (or placeholder smoke) on aircraft|
|`NiagaraSystem_Impact`|Simple explosion VFX for hits|
|`NiagaraSystem_LockOn`|Pulse or ring to indicate missile target lock|
|Placeholder audio cues|SFX for engine, weapon fire, and explosions|
|VFX/SFX triggers setup|Hooked to `OnFire`, `OnHit`, `OnLock` via Blueprint events|
|VFX actor prefab (optional)|A modular actor for attaching and previewing FX|

---

## 🧰 Example – How to Trigger VFX in C++

```cpp
// AProjectile.cpp
if (ImpactFX)
{
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactFX, GetActorLocation());
}
```

```cpp
// AWeaponBase.cpp
UFUNCTION(BlueprintImplementableEvent)
void OnWeaponFired(); // DevD can bind to this in Blueprint
```

---

## 📂 Folder Structure for DevD's Work

```plaintext
Content/VFX/
├── NiagaraSystem_EngineTrail
├── NiagaraSystem_LockOn
├── NiagaraSystem_Impact

Content/Audio/
├── SFX_Engine
├── SFX_MissileFire
├── SFX_Explosion
```

---

## 💡 Tips for DevD

- Use **Niagara templates** to speed up effect creation (UE5 provides jet trails, explosions).
    
- For sound, wrap `.wav` into **SoundCue** assets for volume/pitch randomization.
    
- Ask DevB for movement values to modulate trail FX (e.g., boost = stronger trail).
    
- Niagara supports **LOD**, so it's mobile-friendly when used correctly.
    

---

## ✅ Summary: DevD Week 1 Recap

|Goal|Done When…|
|---|---|
|Aircraft has visual engine effect|Aircraft trails visible while flying|
|Weapon fire has a bang!|Explosion + sound when firing or hitting|
|Lock-on system shows feedback|Target pulse or reticle glows when locked|
|Game _feels alive_ with sound|Hearing movement, speed, fire improves immersion|

---

Would you like:

- **Pre-made Niagara templates** (starter packs)?
    
- Suggestions for **free, license-safe SFX**?
    
- A **VFX performance checklist** for mobile optimization?
    

Let me know and I’ll package the tools up!