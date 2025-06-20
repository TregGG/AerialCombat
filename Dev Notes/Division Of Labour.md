## 🧩 Unreal Engine Team Role Assignment (DevA to DevG)

| **Dev**  | **Primary Role**                      | **Responsibilities**                                                               | **Learning Opportunities**                                                   |
| -------- | ------------------------------------- | ---------------------------------------------------------------------------------- | ---------------------------------------------------------------------------- |
| **DevA** | Technical Lead / Multiplayer Engineer | Architecture design, multiplayer replication, session handling, core gameplay loop | Networking, session systems, RPCs, system architecture                       |
| **DevB** | Aircraft & Weapon Systems Dev         | Aircraft movement, physics control, weapon firing logic, damage system             | Aircraft dynamics, weapon component design                                   |
| **DevC** | UI & Input Systems Dev                | UMG HUDs, touch input, Enhanced Input setup, camera modes                          | HUD integration with gameplay, camera logic, responsive mobile input         |
| **DevD** | Visual Effects & Sound Integration    | Niagara systems, sound cues, UI animations, missile trails                         | Niagara systems, UE5 audio, animation blueprint logic                        |
| **DevE** | 3D Models, Assets & Optimization      | Aircraft models, LODs, weapon meshes, texture compression, mobile-friendly assets  | LOD systems, Nanite/HLOD pipeline, mobile performance testing                |
| **DevF** | AI & Gameplay Designer                | Enemy AI (if PvE), player feedback design, lock-on systems, missile behavior       | Behavior trees, EQS, AIController, gameplay feedback design                  |
| **DevG** | Level Design, Testing & Polish        | Blockout maps, lighting, post-processing, mobile testing, UX iteration             | UE lighting, QA processes, performance optimization, UI/UX improvement loops |

---

## 🧱 System-to-Developer Mapping

|**System / Feature**|**Lead Dev**|**Supporting Devs**|**Notes**|
|---|---|---|---|
|✅ Core Architecture|DevA|—|All major classes, subsystems, base actor/component logic|
|✅ Multiplayer (Sessions, RPCs)|DevA|DevC|EOS or Firebase integration for mobile|
|✅ Aircraft Movement|DevB|DevA|Smooth, mobile-friendly physics input|
|✅ Weapon Systems|DevB|DevF|Modular weapon logic, ammo, cooldowns|
|✅ Health / Damage / Death|DevB|DevC, DevF|Component-based system; replicated|
|✅ UI & HUD|DevC|DevD, DevG|UMG layout, health bars, targeting indicators|
|✅ Camera System|DevC|DevA|Dynamic follow cam, cockpit cam, zoom|
|✅ Touch Input (Mobile)|DevC|DevG|Touch joystick, fire/missile buttons|
|✅ VFX (Niagara, UI FX)|DevD|DevC|Jet trails, missile lock effects|
|✅ Sound Design|DevD|DevG|SFX for boost, lock-on, hits|
|✅ 3D Models / LODs|DevE|DevD|Aircraft, weapons, performance optimization|
|✅ AI Systems|DevF|DevA|AIController, Behavior Trees, perception|
|✅ Level Design & Polish|DevG|DevE, DevF|Mobile-ready maps, lighting, QA testing|

---

## 🎯 Suggested Communication Structure

|Role|Person|Meetings / Tasks|
|---|---|---|
|**Lead Developer**|DevA|Daily standup lead, feature reviews, Git merge approval|
|**Subsystem Leads**|DevB, DevC|Weekly subsystem sync (UI/input & gameplay)|
|**Art & UX Sync**|DevD, DevE, DevG|Twice-weekly check-in on visuals, design, and test feedback|
|**AI & Game Design**|DevF|Works closely with DevB/DevC on gameplay feel|

---

## 📚 Skill Growth Paths per Dev

|Dev|Skill Goals|
|---|---|
|**DevA**|Systems architecture, session handling, optimization, team management|
|**DevB**|Modular actor systems, weapon handling, component programming|
|**DevC**|Enhanced Input, UMG animation, touch UX, camera interpolation|
|**DevD**|Niagara emitter design, material instance logic, SFX triggering|
|**DevE**|3D pipeline, asset optimization, Unreal LOD & Nanite systems|
|**DevF**|AI perception, BT design, lock-on systems, player feedback mechanisms|
|**DevG**|UE Level Streaming, mobile UX testing, lighting, and polish workflows|

---

## ✅ Sample First Sprint (Week 1)

|Dev|Task(s)|
|---|---|
|DevA|Set up base architecture, replication testing, lobby/session system|
|DevB|Create `AAircraftBase`, `AircraftMovementComponent`, basic flight logic|
|DevC|Build UMG base HUD, link with Enhanced Input touch controls|
|DevD|Create basic VFX (engine trail), missile lock indicator, start weapon SFX|
|DevE|Import placeholder aircraft, create LODs, test for mobile draw calls|
|DevF|Begin design of missile lock behavior, AI movement ideas|
|DevG|Greybox test map, set up mobile device build pipeline, gather feedback|

---

## 🛠 Tools You Should Be Using

| Tool                | Usage                                   |
| ------------------- | --------------------------------------- |
| **GitHub/GitLab**   | Source control, issue tracking          |
| **Perforce**        | Optional for heavy asset versioning     |
| **Unreal Insights** | For performance profiling on mobile     |
| **Discord/Slack**   | Daily async updates, standup, debugging |

---
