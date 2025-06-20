Awesome — let’s wrap up Week 1 with the plan for **DevG: Level Design, Testing & UX Polish**.
# 👤 DevG – Week 1 Focus: **Test Map, UX Touchpoints, and QA Setup**

> 🎯 **Goal:** Build a test environment where other devs can fly, fight, and test systems in one place.  
> You’ll also start exploring how the **game feels** on mobile, and help connect early HUD/UI designs to feedback flow (health, lock-on, damage, etc.).

This role is a hybrid between **level prototyping**, **QA**, and **user experience validation**.

---

## 🛠️ What DevG Will Work On (Week 1)

|System/Area|What DevG Does|
|---|---|
|**Test Map (Greybox)**|Design an open-air test map with spawns, navigation space, simple obstacles|
|**Player Spawn Points**|Place PlayerStart actors for multiplayer & testing|
|**Mobile QA Setup**|Run build on Android/iOS test device, report performance or UI bugs|
|**UI/UX Testing**|Check HUD readability, button sizes, joystick layout on screen|
|**Lighting/PostFX**|Add basic directional light, skybox, and test post-processing|
|**Feedback Sync**|Help test health feedback, lock-on indicators, and fire reaction loops|

---

## ✅ Week 1 Task Breakdown

|**Task**|Location/Asset|Description|Est. Time|
|---|---|---|---|
|Create **TestFlightMap**|`/Maps/TestFlightMap.umap`|Large, open map with some terrain variation & obstacles|1 day|
|Place **PlayerStart** + dummy enemies|In `TestFlightMap`|At least 2 spawn points and 1 enemy pawn for lock-on & fire tests|0.5 day|
|Add basic **lighting/post-process**|DirectionalLight, SkySphere|Ensure level looks clear and consistent on mobile|0.5 day|
|Hook up test **HUD** with DevC|Check if widgets display correctly on phone|1 day||
|Run mobile build & test input|On real device (if available)|Validate touch input, button size, responsiveness|1 day|
|Report UX issues|Share screenshots & written notes|E.g., joystick too small, button overlaps, text too hard to read|0.5 day|

---

## 📂 File Structure for DevG

```plaintext
Content/
├── Maps/
│   └── TestFlightMap.umap
├── QA/
│   └── MobileTestNotes_Week1.md
├── UI/
│   └── FeedbackScreenshots/ (HUD visibility issues, reticle problems, etc.)
```

---

## 💡 WHY NOT JUST TEST IN “Minimal Default” MAP?

> “Why not just let everyone test in the default level? Why build another map?”

### 🔥 The Pitch: Why DevG’s Work Matters (Especially to Newer Devs)

|Problem (No Level or UX Testing)|Solution (Greybox Test + UX Testing)|
|---|---|
|Team wastes time debugging in unprepared maps|TestMap lets everyone spawn, fly, shoot quickly|
|Mobile UI doesn't scale/test properly|DevG ensures mobile buttons feel good and are placed correctly|
|No baseline visual feedback|Lighting and post FX give a clean feel that matches expected game tone|
|Bugs go unnoticed until late|DevG can report UI/VFX/sound bugs early through builds and screenshots|

> ✨ DevG is the **first person to feel the actual game as a player**, and tell the team:  
> “Here’s what’s working. Here’s what’s confusing. Here’s what’s awesome.”

---

## 🎯 Deliverables for End of Week 1 (DevG)

|Deliverable|Description|
|---|---|
|`TestFlightMap.umap`|A basic test level where players can spawn, fly, lock, and shoot|
|Lighting setup|Sky, sunlight, and fog post-process volume for clarity|
|Mobile test report|Document with screenshots, input feedback, UI sizing notes|
|QA feedback (optional)|Notes for DevC/DevD: "Lock icon is too small", "Joystick overlaps HUD"|
|Enemy dummy in map|So DevF can test lock-on logic visually|

---

## 🔁 Collaboration Touchpoints

|Collaborator|What They Provide|What You Test/Validate|
|---|---|---|
|**DevB**|Aircraft & movement systems|Flight feel, physics clarity|
|**DevC**|Touch input & HUD|UI scale, input feedback, visibility|
|**DevD**|VFX/SFX cues|Do effects trigger clearly and read well visually|
|**DevF**|Lock-on mechanics|Target feedback, lock timers, visuals|

---

## 📘 Example Test Case Notes

```markdown
## Mobile UI Test – June 17
- Joystick button is too small on Galaxy S10 → resize to 140x140
- Fire button overlaps lock icon → move lock to top-center
- Lock-on FX is not visible in bright light → suggest blue outline
- Game runs at 45–50fps on Android (good)
```

---

## ✅ DevG Week 1 Summary

|✅ Outcome|Description|
|---|---|
|Test map built|Aircraft and weapons can be tested by everyone|
|Mobile UX feedback provided|Early warnings about UI, controls, or visibility|
|Real gameplay feel established|Level design gives a sense of space, direction, and usability|
|Shared notes with team|Everyone gets insights from a player perspective|

---

## 💡 Tips for DevG

- Use mobile build + "Launch on Device" to test real UI performance
    
- Use `HighResShot 1920x1080` or `Take Screenshot` for documentation
    
- Use console command `Stat FPS`, `Stat Unit` to check mobile perf
    

---

Would you like:
- A test map template to start with?
- A checklist for “Mobile UX Do’s and Don’ts”?
- A bug reporting template for feedback consistency?
Let me know — I can deliver a **QA toolkit** ready to drop into your project folder.