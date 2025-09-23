# Usage Examples & Tutorials

### Load a Sample Scene
1. Build Henoc and launch it with `./src/henoc` (Autotools build) or `./henoc` (legacy `src/` build).
2. Choose `Archivo` → `Abrir XKI…` (or press `Ctrl+O`).
3. Navigate to `examples/` and select a scene such as `balls01.xki` or `catapulta.xki`.
4. Inspect the sketch in the editor, then click **Play** to watch the Open Dynamics Engine simulation.
5. Use **Stop** to return to the editor, tweak objects (e.g., edit mass, friction, or line thickness), and replay to see the changes.

### Draw Your First Sketch
1. Start Henoc and click the **Box**, **Ball**, or **Line** toolbar buttons to enter insertion mode.
2. Click-and-drag on the canvas to size each item. Right-click a line to adjust its thickness if desired.
3. Select an object and press **Propiedades** to modify physics parameters such as mass or collision masks.
4. When the scene is ready, press **Play**. Henoc embeds the sketch into the ODE world and animates it in the OpenGL viewport.
5. Save your work with **Guardar XKI…**; files are stored in the XML-based `.xki` format and can be reloaded later.

### Quick Catapult Tutorial
1. Load `examples/catapulta.xki`.
2. Review the boundary lines that form the ENMARCA frame—objects stay within these limits.
3. In **Propiedades**, adjust world gravity (e.g., set it to `6.0`) and play the scene to see a slower arc.
4. Edit the projectile ball: increase its mass to `12`, friction to `0.2`, and bounce factor to `0.6` for livelier rebounds.
5. Re-run the simulation and observe how global and per-object settings combine to produce new behavior.

### CLI Tips
- Use the `--enable-assets` configure flag when you want freshly generated PNG icons for the UI.
- To rebuild quickly during development: `make -C src` reuses the legacy Makefile and skips Autotools regeneration.
- `make distclean` removes the configured build tree—helpful before packaging or running Sonar checks.
