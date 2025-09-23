# FullHenoc & HenocUniverse Architecture

## 1. Scope & Context
- Focuses on the interaction between the `src/FullHenoc` (Qt front-end) and `src/HenocUniverse` (physics core) modules.
- Excludes legacy drivers (`HenocCameraDrv`) and other repository utilities.
- Highlights wiring, runtime architecture, class relationships, and user/engine workflows with ASCII diagrams for easy edits.

## 2. Runtime Architecture

### 2.1 Layer Overview
- **Presentation layer** – `CfrmHenoc` window orchestrates UI state, dialogs (`CfrmMundo`, `CfrmPeCaLi`), and the OpenGL viewport.
- **Scene editing layer** – `DiagramScene` manages `QGraphicsItem` derivatives (`HBox`, `HBall`, `HLine`) that carry `HObject` physics properties.
- **Simulation bridge** – `GLViewport` (QOpenGLWidget) delegates to `ODEBridge` for ODE setup, stepping, and drawing.
- **Physics core** – `HenocUniverse` provides reusable abstractions (`World`, `Object`, `Geometry`, collision helpers) on top of the ODE runtime.

### 2.2 Top-Level Wiring
```
Runtime structure (simplified)

+---------------------------+
| Qt Widgets Layer          |
| - CfrmHenoc               |
| - CfrmMundo / CfrmPeCaLi  |
+------------+--------------+
             |
             | owns & configures
             v
+------------+--------------+
| DiagramScene              |
| - HBox / HBall / HLine    |
| - HObject property bags   |
+------------+--------------+
             |
             | exports objects on Play()
             v
+------------+--------------+        QTimer->timeout
| GLViewport (QOpenGLWidget)|<-----------------------+
+------------+--------------+                        |
             | uses                                   |
             v                                        |
+------------+--------------+                        |
| ODEBridge namespace       |------------------------+
| - builds ODE geoms        |    StepAndDraw() draws  |
| - applies Whstc params    |                        |
+------------+--------------+                        |
             |                                       |
             | leverages                              |
             v                                        |
+------------+--------------+                        |
| HenocUniverse / ODE       |<------------------------+
| - World, Geometry, etc.   |
+---------------------------+
```

### 2.3 Key Notes
- The `Whstc` struct (defined in `CfrmHenoc.h`) captures world-level tunables passed to `ODEBridge::SetWorldParams` before every simulation run.
- Boundary detection inside `CfrmHenoc::Play` identifies "ENMARCA" frame lines and calls `ODEBridge::AddBoundaryWalls`, preventing objects from leaving the playable area.
- `GLViewport` disables depth testing and draws in a 2D orthographic projection that matches editor pixels (origin at top-left) for 1:1 visual parity.

## 3. Wiring & Data Flow

### 3.1 UI Event Wiring
- Toolbar buttons (`btnBox`, `btnLine`, `btnBall`, `btnPlay`, etc.) connect via Qt signals to `CfrmHenoc` slots, which toggle the scene mode or trigger simulation.
- `DiagramScene` emits `itemInserted` and `lineThicknessChanged`; `CfrmHenoc` uses these to reset toolbar state and update the status bar.
- A shared `QTimer` (interval `Whstc::fAnim` ms) drives continuous calls to `GLViewport::update`, keeping the OpenGL view in sync while the simulation is active.

```
Button -> Signal/Slot wiring

[User] --click--> [QPushButton] --Qt signal--> [CfrmHenoc slot]
                                            |
                                            v
                               [DiagramScene::setMode()/setItemType()]
```

### 3.2 Scene-to-Physics Handoff
- `CfrmHenoc::Play` iterates over every `QGraphicsItem` in `DiagramScene`, extracts the associated `HObject` attributes, and enqueues them through `ODEBridge::AddBox`, `AddBall`, or `AddLine`.
- `ODEBridge` converts screen units (pixels) into ODE bodies/geometries, applying densities scaled by `Whstc::densityScale` and colorizing fills according to the object's hue (`thetaColor`).

```
Play wiring (CfrmHenoc::Play)

[CfrmHenoc::Play]
   |
   | items = scene->items()
   v
[Extract HObject data]
   |
   v
[ODEBridge::AddBox/AddBall/AddLine]
   |
   v
[ODE world (g_world + g_space)]
   |
   v
[GLViewport timer repaint]
   |
   v
[ODEBridge::StepAndDraw() -> OpenGL frame]
```

### 3.3 World Tuning & Property Editing
- `changeProperties()` switches between the global world dialog (`CfrmMundo`) and the per-object dialog (`CfrmPeCaLi`) depending on the current selection count.
- `CfrmMundo` edits `Whstc` values (animation rate, gravity, ERP/CFM, fill brightness, density scaling, default line thickness). Updates take effect on the next Play invocation.
- `CfrmPeCaLi` synchronizes `HObject` fields with UI widgets and, for lines, adjusts the live `QPen` width on the selected `HLine` item.

## 4. Class Diagrams

See `CLASS_DIAGRAMS.md` for the latest diagrams.

## 5. Workflows

### 5.1 Sketch-to-Simulation
```
Sketch-to-Simulation

[User clicks Play]
        |
        v
[CfrmHenoc::Play]
        |
        v
[DiagramScene::items()]
        |
        v
[Extract HObject + item geometry]
        |
        v
[ODEBridge::AddBox/AddBall/AddLine]
        |
        v
[GLViewport timer -> update()]
        |
        v
[ODEBridge::StepAndDraw(delta)]
        |
        v
[OpenGL frame rendered to viewport]
```

### 5.2 Save & Load `.xki`
```
Save (.xki)

[CfrmHenoc::Save]
        |
        v
[Open QFile + QXmlStreamWriter]
        |
        v
[Iterate DiagramScene items]
        |
        v
[Write <Box>/<Ball>/<Line> elements with HObject data]
        |
        v
[Close file]
```

```
Load (.xki)

[CfrmHenoc::Open]
        |
        v
[QXmlStreamReader parses <Sketch>]
        |
        v
[Clear DiagramScene]
        |
        v
[Instantiate HBox/HBall/HLine]
        |
        v
[Populate HObject fields + colors]
        |
        v
[Scene ready for editing/playback]
```

### 5.3 Physics Step Loop
```
Physics update per frame

[QTimer timeout]
        |
        v
[GLViewport::update()]
        |
        v
[GLViewport::paintGL()]
        |
        v
[ODEBridge::StepAndDraw(stepDelta)]
        |
        v
[dSpaceCollide -> dWorldQuickStep]
        |
        v
[Draw g_objs via OpenGL]
```

### 5.4 World Parameter Editing
```
World parameter edit

[User triggers changeProperties() with no selection]
        |
        v
[CfrmHenoc opens CfrmMundo]
        |
        v
[CfrmMundo edits Whstc fields]
        |
        v
[Dialog Accepted -> myWorldProp updated]
        |
        v
[Next CfrmHenoc::Play -> ODEBridge::SetWorldParams(myWorldProp)]
```

## 6. Reference Map
- **Core UI**: `src/FullHenoc/CfrmHenoc.cpp`, `CfrmMundo.cpp`, `CfrmPeCaLi.cpp`.
- **Scene items**: `src/FullHenoc/diagramscene.cpp`, `HBox.cpp`, `HBall.cpp`, `HLine.cpp`, `HObject.cpp`.
- **Rendering & physics bridge**: `src/FullHenoc/glviewport.cpp`, `ode_bridge.cpp`.
- **Physics abstractions**: `src/HenocUniverse/henocUniverse.h`, `source/henocUniverse.cpp` and supporting geometry/collision files.

