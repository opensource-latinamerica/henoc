# TROUBLESHOOTING & QT NOTES

## Building With Different Qt Versions

Henoc targets Qt 5. If your system defaults to Qt 6, install Qt 5 side-by-side and ensure `qmake` and headers point to Qt 5 (e.g., via `qtchooser` or distro-specific packages).

## Troubleshooting (Qt 5 Only)

- **Qt 6 picked instead of Qt 5**
  - *Symptom:* build errors about missing Qt 5 modules or incompatible headers.
  - *Fix:* install Qt 5 development packages and ensure the Qt 5 `qmake` is used (e.g., `qmake-qt5`). On Debian/Ubuntu, `qt5-qmake` installs `qmake`. On Fedora, use `qt5-qtbase-devel` and `qmake-qt5`.
  - *Check:* `qmake -v` should report Qt version 5.x.

- **Missing Qt 5 modules (widgets/opengl/xml)**
  - *Symptom:* compilation fails finding `<QtWidgets/...>` or OpenGL headers.
  - *Fix (Debian/Ubuntu):*
    ```bash
    sudo apt install qtbase5-dev qtbase5-dev-tools qttools5-dev-tools libglu1-mesa-dev mesa-common-dev
    ```
  - *Fix (Fedora):*
    ```bash
    sudo dnf install qt5-qtbase-devel mesa-libGLU-devel
    ```

- **ODE submodule not initialized**
  - *Symptom:* missing sources under `src/HenocUniverse/ode/` or submodule build errors.
  - *Fix:*
    ```bash
    git submodule update --init --recursive
    ```

- **ODE autotools warnings/errors during bootstrap**
  - *Symptom:* messages about obsolete macros or libtool warnings.
  - *Notes:* warnings are typically harmless. If bootstrap fails, install autotools:
    ```bash
    # Debian/Ubuntu
    sudo apt install autoconf automake libtool
    # Fedora
    sudo dnf install autoconf automake libtool
    ```

- **OpenGL headers not found**
  - *Symptom:* missing GL/GLU headers.
  - *Fix (Debian/Ubuntu):* `sudo apt install mesa-common-dev libglu1-mesa-dev`
  - *Fix (Fedora):* `sudo dnf install mesa-libGL-devel mesa-libGLU-devel`

- **Branding export script fails**
  - *Symptom:* `branding/export.sh` cannot find inkscape/rsvg/convert.
  - *Fix:*
    ```bash
    # Choose one of the following exporters
    sudo apt install inkscape
    # or
    sudo apt install librsvg2-bin   # provides rsvg-convert
    # or
    sudo apt install imagemagick
    ```
  - Ensure the script is executable: `chmod +x branding/export.sh`.

- **Runtime black screen or unexpected physics**
  - Ensure ENMARCA (frame) exists if you expect solid boundaries; Henoc auto-adds ODE boundary walls when frame lines are detected at the scene edges.
  - Verify world settings (gravity defaults to 9.8). Adjust Fill Brightness and Density Scale in the World dialog as needed.

## Additional Help
- Consult the `README.md` for installation and build steps.
- Refer to `docs/USER_GUIDE.md` for UI walkthroughs and general troubleshooting tips.
