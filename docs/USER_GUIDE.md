# Henoc User Guide

## 1. Getting Started

### 1.1 Launching the Application
- **Autotools build:** After running `make`, launch the editor with `./src/henoc` from the repository root.
- **Legacy build:** If you built via `make -C src`, run `./henoc` from inside `src/`.
- On first launch, Henoc opens the diagram editor ("Lienzo") ready for sketching.

### 1.2 Interface Overview
```
+---------------------------------------------------------------+
| Toolbar: Box | Ball | Line | Catapult | Play | Stop | ...     |
+---------------------------------------------------------------+
| Canvas (QGraphicsView hosting DiagramScene)                   |
| - Draw and manipulate boxes, balls, lines                     |
| - Context menu on lines for thickness presets                 |
+---------------------------------------------------------------+
| Status bar → shows selection info (e.g., line thickness)      |
+---------------------------------------------------------------+
| Page tabs: Editor | Simulation (OpenGL viewport)              |
+---------------------------------------------------------------+
```

Key components:
- **Toolbar** – toggles insertion modes and open/save actions.
- **Canvas** – displays the editable sketch (per-object physics lives in each item).
- **Status bar** – reflects current selection and dialog hints.
- **Simulation tab** – displays the ODE-powered animation via OpenGL.

## 2. Core Features

### 2.1 Drawing Objects
- Use the **Box**, **Ball**, or **Line** buttons to enter insertion mode.
- Click-and-drag to size the object; release to insert.
- Lines support context menu thickness presets (1/2/4/8 px) or custom widths.
- Automatically assigned colors help differentiate objects.

### 2.2 Editing Physics Properties
- Select an object and click **Propiedades** to open the per-object dialog (`CfrmPeCaLi`).
- Adjustable settings include:
  - Mass, friction, bounce factor, bounce velocity.
  - Collision and friction masks (presets for groups 1–8 plus custom integer).
  - Rotation (for boxes) or radius (for balls).
  - Line thickness (when editing an `HLine`).
- Changes update the underlying `HObject` and, for lines, the scene's pen width.

### 2.3 World Configuration
- With no selection, clicking **Propiedades** opens the world dialog (`CfrmMundo`).
- Tunables:
  - Animation frequency (`fAnim`) and simulation delta time.
  - Contact and constraint parameters (ERP, CFM, max corrective velocity, surface layer).
  - Gravity, auto-disable thresholds, density scale, fill brightness, default line thickness.
- Updates are stored in the `Whstc` struct and applied on the next simulation run.

### 2.4 Running Simulations
- Press **Play** to transfer the current scene into the ODE simulation.
- The editor switches to the OpenGL viewport; the `QTimer` drives frame updates.
- Press **Stop** to return to the editor and continue editing.
- Boundary lines along the scene rectangle are auto-detected and converted into solid walls.

### 2.5 Saving & Loading
- **Guardar XKI…** saves the sketch to an XML-based `.xki` file.
- **Abrir XKI…** loads existing sketches from disk.
- The repository provides sample files in `examples/` for experimentation.

## 3. Configuration Dialogs

### 3.1 Object Properties (`CfrmPeCaLi`)
```
[ Tabs: General | Advanced (varies) ]
Mass / Friction / Bounce parameters
Collision mask presets + spin box
Friction mask presets + spin box
Box: width, height, rotation
Ball: radius
Line: thickness presets + custom spin box
```
- Presets synchronize with integer spin boxes (Custom selects manual values).
- Tooltips explain mask logic and thickness options.

### 3.2 World Settings (`CfrmMundo`)
```
Animation interval (ms)
Simulation delta (s)
Gravity
Contact settings: ERP, CFM, max correcting velocity, surface layer, auto-disable thresholds
Brightness and density scale
Default line thickness presets + custom value
```
- Sliders/spin boxes map directly to `Whstc` members.
- Density scale blends object mass with pixel area density for more realistic behavior.

### 3.3 Context Menu Enhancements
- Right-click a line item to access thickness presets and custom thickness dialog.
- Status bar automatically reflects the selected line's width for quick feedback.

## 4. Working with Examples

```
examples/
  balls01.xki       # elastic collisions
  catapulta.xki     # basic projectile launcher
  domino3.xki       # domino chain reactions
  equilibrio.xki    # balancing blocks
```
- Load any `.xki`, run the simulation, and adjust individual object properties to observe physics changes.
- Combine presets (e.g., collision groups) to stage more complex interactions.

## 5. Troubleshooting

### 5.1 Common Issues
- **Qt 6 picked instead of Qt 5** – Ensure Qt 5 development packages are installed; run `qmake -v` to verify version 5.x. Override with `./configure --with-qmake=/path/to/qmake-qt5` if necessary.
- **Missing headers (Qt/OpenGL)** – Install the required dev packages (`qtbase5-dev`, `libglu1-mesa-dev`, etc.) per README instructions.
- **ODE rebuild needed** – Delete `src/HenocUniverse/ode_install` and rebuild if the physics library appears corrupted.
- **Simulation does not play** – Confirm at least one object exists, and check status bar for error hints. Lines that form the boundary are skipped but still enforce walls.

### 5.2 Debugging Tips
- Run the application from a terminal to capture Qt warnings and ODE diagnostics.
- Use the legacy `src/` Makefile for faster rebuilds during debugging (`make -C src`).
- Inspect `bw-output/compile_commands.json` (generated during CI builds) for compilation flags if integrating with IDEs or static analysis tools.

## 6. Additional Resources
- **Architecture notes:** `docs/ARCHITECTURE.md`
- **Class diagrams:** `docs/CLASS_DIAGRAMS.md`
- **Branding assets:** `branding/README.md`
- **Examples:** `examples/` directory (load via Henoc's Open dialog)

Feedback and bug reports are welcome via GitHub issues. Contributions should follow the guidelines in the project README.
