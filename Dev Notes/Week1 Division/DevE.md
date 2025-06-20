# 👤 DevE – Week 1 Focus: **3D Asset Integration, LODs & Mobile Optimization**

> 🎯 Goal: Create and integrate basic placeholder aircraft and weapons, build a streamlined import pipeline, and set up **LODs/Nanite-friendly workflows** that keep mobile performance in check.  
> This lays the groundwork for all visuals in the game — from player aircraft to projectiles.

---

## 🛠️ What DevE Will Work On (Week 1)

|System|What DevE Does|
|---|---|
|**Placeholder Aircraft**|Model or import basic aircraft mesh and set up in UE|
|**Weapon Meshes**|Basic missile or gun barrel mesh for attachment|
|**Import Pipeline**|Define folder structure, FBX naming, scale/axis convention|
|**LODs Setup**|Add LODs to aircraft mesh for mobile-friendly rendering|
|**Material Standards**|Assign basic materials, prep for customizable skins (later weeks)|
|**Nanite Check/Test**|Decide which models can/should use Nanite — fallback to LODs if not|

---

## ✅ Week 1 Task Breakdown

|**Task**|File/Folder|Description|Est. Time|
|---|---|---|---|
|Create/import placeholder aircraft|`/Art/Meshes/Aircraft/JetA.fbx`|Simple jet/plane model, clean pivot, game scale|1 day|
|Create weapon mesh|`/Art/Meshes/Weapons/MissileA.fbx`|Simple missile, 1–2K polycount|0.5 day|
|Define import pipeline doc|`AssetImportGuide.md`|Scale (1cm = 1cm), naming (`SM_JetA`), materials, smoothing groups|0.5 day|
|Create & test LODs manually|Aircraft mesh in engine|LOD0–LOD3 with auto or custom decimation|1 day|
|Assign basic material instance|`/Art/Materials/MI_JetBasic`|Gray box mat with parameter slots|0.5 day|
|Test Nanite compatibility|Use console: `r.Nanite 1`|See if aircraft/weapon can use Nanite or fallback to LODs|0.5 day|

---

## 📂 File Structure for DevE

```plaintext
Content/
├── Art/
│   ├── Meshes/
│   │   ├── Aircraft/
│   │   │   └── SM_JetA.fbx
│   │   └── Weapons/
│   │       └── SM_MissileA.fbx
│   └── Materials/
│       ├── M_JetBase (Master)
│       └── MI_JetBasic (Instance)
├── Docs/
│   └── AssetImportGuide.md
```

---

## 💡 WHY THIS MATTERS (Selling the Workflow to Newer Devs)

> “Why not just drag in random .fbx files and assign a material? Why care about LODs or Nanite?”

### 🔥 The Pitch: Why Asset Discipline = Performance & Speed

|Problem (Quick-and-dirty workflow)|Solution (Clean Pipeline)|
|---|---|
|Inconsistent scales → broken collisions/cameras|Standardized 1cm scale for all assets|
|High-poly meshes → laggy performance on mobile|Use LODs or Nanite where possible for auto-scaling detail|
|No naming convention → chaos in content browser|SM_ prefix, clean folder structure → fast lookup & debugging|
|Models import weirdly rotated/upside-down|Clean forward axis + up vector set in export & import|
|Materials get duplicated everywhere|Master materials + instances reduce draw calls|

> 🔧 You’re not just importing assets — you’re building a **pipeline** that lets artists, designers, and gameplay devs move faster without fear of breaking things.

This is what real studios do to scale from 10 to 1000+ assets.

---

## 🎯 Deliverables for End of Week 1 (DevE)

|Deliverable|Description|
|---|---|
|Placeholder aircraft mesh|Single, usable low-poly aircraft model in-engine with clean pivot|
|Missile mesh|Imported and assigned to a `StaticMeshComponent` in weapon class|
|LODs set up|Either manually or UE-generated (LOD0-LOD3) for aircraft|
|Basic materials|One master material + instance for tweaking color/emissive|
|Asset import guide|Short doc for team describing asset prep, FBX naming, and folder rules|
|Nanite testing complete|You know which meshes use Nanite vs LOD|

---

## 🔁 Collaboration Touchpoints

|Collaborator|What They Provide|What You Do With It|
|---|---|---|
|**DevB**|Aircraft base actor + component sockets|Attach meshes to correct sockets|
|**DevD**|VFX trail location|Ensure pivot/sockets support correct placement|
|**DevG**|Level design setup|Confirm lighting and performance are holding up|

---

## 📘 Example: FBX Export Checklist (Include in Guide)

|Setting|Value|
|---|---|
|**Scale**|1 cm = 1 Unreal unit|
|**Up Axis**|Z+ (Maya/Blender)|
|**Forward Axis**|Y+ or X+|
|**Smoothing Groups**|ON|
|**Include Animation**|OFF (for static meshes)|
|**Name Convention**|`SM_JetA`, `SM_MissileA`|

---

## 📊 Optimization Tip

|Setting|Recommendation|
|---|---|
|Triangle count (LOD0)|Aircraft: 10K–15K poly max; Missile: 1K–3K|
|LODs|At least 3 → LOD1 = 50%, LOD2 = 25%, LOD3 = 10%|
|Material slots|1–2 per mesh, avoid more unless justified|
|Nanite fallback for mobile|Only if you're NOT supporting Vulkan 1.0 or low-end Android|

---

## ✅ DevE Wrap-Up

> By end of Week 1, you’ll have given the team:

- **Clean placeholder models**
    
- **A reusable pipeline for artists**
    
- **Optimized geometry for mobile**
    
- And the **confidence** that “visuals won’t tank our framerate.”
    

---

Would you like:

- A downloadable `.fbx` placeholder aircraft/missile to start from?
    
- A `.uasset` example of LOD mesh setup?
    
- A PDF version of the Asset Import Guide?
    

Just say the word — I can build that asset starter kit for you!