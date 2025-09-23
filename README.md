<p align="center">
  <img src="branding/png/icon-h-gradient-256-128.png" alt="Henoc logo" width="128" height="128" />
</p>

# Henoc: Physics Sketch Animator

Henoc is a sophisticated physics simulation software that brings user-drawn sketches to life. By leveraging a pattern recognition agent, Henoc can interpret and animate physics sketches from a simple blackboard interface, providing a dynamic and interactive way to visualize physics principles.

## Overview

The core of Henoc is its ability to transform static, hand-drawn sketches into moving simulations. Users can draw various shapes and objects on a virtual blackboard, and Henoc's engine will apply physics rules to animate them. This makes it an excellent tool for educational purposes, rapid prototyping of physics-based scenes, or simply for creative exploration.

## What’s New (Latest Changes)

- Native ODE pipeline (dWorld + dSpace + geoms) for robust collisions and contacts.
- ENMARCA frame now acts as solid environment limits; objects can’t pass through.
- 1:1 OpenGL projection (origin at top‑left) aligned to editor pixels.
- Per‑object physics parity (mass, friction, bounce) and theta‑based colors.
- World‑level tunables: Fill Brightness Factor, Density Scale, Line Thickness preset + custom.
- Object dialog presets: Collision/Friction masks (All/None/Group 1–8/Custom) with two‑way sync.
- Per‑line thickness controls (presets + custom), live status bar readout, and context‑menu quick set.
- Branding assets added at top‑level `branding/` (+ export script and CI action for PNGs).

## Dependencies & Versions

Minimum/recommended versions to build and run Henoc:

- C++ compiler: g++/clang with C++11 support (g++ 7+ recommended).
- Qt 5 (Widgets, OpenGL, XML): Qt 5.12+ (tested with Qt 5.15.x).
- GNU Make and autotools (ODE submodule bootstrap): make, autoconf/automake/libtool.
- OpenGL headers/libs (Mesa on Linux).

Optional (branding export):

- Inkscape (preferred) or `rsvg-convert` (librsvg) or ImageMagick `convert`.

## Third‑Party Libraries

- Open Dynamics Engine (ODE)
  - Integrated as a Git submodule: `src/HenocUniverse/ode`
  - Upstream: https://bitbucket.org/odedevs/ode.git
  - The build uses the pinned submodule commit. You can check with:
    ```bash
    git submodule status src/HenocUniverse/ode
    ```

## Getting the Source (with Submodules)

```bash
git clone <this-repo-url>
cd henoc
git submodule update --init --recursive
```

## Example Dependencies

Debian/Ubuntu

```bash
sudo apt install -y \
  autoconf automake libtool build-essential \
  mesa-common-dev libglu1-mesa-dev \
  qtbase5-dev qt5-qmake qtbase5-dev-tools \
  # optional asset tools
  inkscape librsvg2-bin imagemagick
```

Fedora/RHEL

```bash
sudo dnf install -y \
  autoconf automake libtool make gcc-c++ \
  mesa-libGL-devel mesa-libGLU-devel \
  qt5-qtbase-devel qt5-qttools-devel \
  # optional asset tools
  inkscape librsvg2-tools ImageMagick
```

Arch Linux

```bash
sudo pacman -S --needed \
  autoconf automake libtool base-devel \
  mesa glu \
  qt5-base qt5-tools \
  # optional asset tools
  inkscape librsvg imagemagick
```

## Build (Autotools, recommended)

Two ways to build: without images (faster) or with images (generate PNGs from `branding/*.svg`).

Prerequisites: see Dependencies above, plus `autoconf`, `automake`, `libtool`.

Without images

1. Initialize submodules:
   ```bash
   git submodule update --init --recursive
   ```
2. Generate the build system:
   ```bash
   bash ./autogen.sh
   ```
3. Configure (Qt5 qmake auto-detected; override with `--with-qmake=/path/to/qmake-qt5` if needed):
   ```bash
   ./configure
   ```
4. Build the app (also builds ODE on first run):
   ```bash
   make
   ```
   Note: the `assets` target is disabled by default. If you need PNGs, either
   re-configure with `--enable-assets` and run `make assets`, or use the
   fallback script `branding/export.sh`.
5. Run:
   ```bash
   ./src/henoc
   ```

With images (generate PNG assets during make)

1. Initialize submodules and generate the build system (same as above).
2. Configure with assets enabled:
   ```bash
   ./configure --enable-assets
   # optional: --with-qmake=/path/to/qmake-qt5
   ```
3. Generate PNGs from `branding/*.svg` (sizes: 16/32/64/128/256):
   ```bash
   make assets
   ```
   Notes: requires one of `inkscape` (preferred), `rsvg-convert` (librsvg), or ImageMagick `convert`.
4. Build the app:
   ```bash
   make
   ```
5. Run:
   ```bash
   ./src/henoc
   ```

Tips
- Override qmake at configure time with `--with-qmake=…`, or at build time with `make QMAKE=/path/to/qmake-qt5`.
- ODE is built into `src/HenocUniverse/ode_install` by default; override with `./configure --with-ode-prefix=/abs/path`.
 - `make assets` is only available when configured with `--enable-assets`.

Quick start

```bash
git submodule update --init --recursive
bash ./autogen.sh
./configure            # or: ./configure --enable-assets
make assets            # only if configured with --enable-assets
make
./src/henoc
```

Configure options

- `--with-qmake=/path/to/qmake-qt5`: force a specific Qt5 qmake.
- `--with-ode-prefix=/abs/path`: where ODE will be installed/built.
- `--enable-assets`: enable Makefile-based SVG→PNG generation.

## Building the Project (legacy src-only)

The project also includes a legacy `src/Makefile` build that automates compiling the ODE submodule (Open Dynamics Engine) and the Henoc application.

To build the Henoc application, follow these steps:

1.  **Navigate to the source directory:**

    ```bash
    cd src
    ```

2.  **Run the make command:**

    ```bash
    make
    ```

This compiles ODE (if needed) and the Henoc app, producing the `henoc` executable in `src/`.

Notes:
- If you see autotools warnings while bootstrapping ODE, they are generally harmless.
- On first build, ODE may be configured/built automatically; subsequent builds will be faster.

## Running Henoc

Once the build is complete, you can run the application from the `src` directory:

```bash
./henoc
```

## Cleaning the Project

- Top-level (Autotools):
  ```bash
  make clean        # delegates cleaning into src/
  make distclean    # also removes configure results
  ```
- Legacy (src-only):
  ```bash
  cd src && make clean
  ```

## Branding Assets

SVG sources and export live at the repository top level:

```text
branding/
  export.sh            # exports all SVGs to PNG (16/32/64/128/256) into branding/png/
  README.md            # asset documentation
  *.svg                # icon/wordmark/lockups
```

Export locally (two options):

1) Via Makefile (recommended when configured with assets):
```bash
./configure --enable-assets
make assets
```

2) Via script (fallback when not using --enable-assets):
```bash
cd branding && ./export.sh
```
 
Note: If you run `make assets` without `--enable-assets`, it will fail with a
message explaining how to enable it or use `branding/export.sh`.

CI (GitHub Actions) automatically exports PNGs on push and uploads them as artifacts.

## Building With Different Qt Versions

Henoc targets Qt 5. If your system defaults to Qt 6, install Qt 5 side‑by‑side and ensure `qmake`/headers point to Qt 5 (e.g., `qtchooser` or distro‑specific packages).

## Troubleshooting (Qt 5 only)

- Qt 6 picked instead of Qt 5
  - Symptom: build errors about missing Qt 5 modules or incompatible headers.
  - Fix: install Qt 5 dev packages and ensure the Qt 5 `qmake` is used (e.g., `qmake-qt5`). On Debian/Ubuntu, `qt5-qmake` installs `qmake`. On Fedora, use `qt5-qtbase-devel` and `qmake-qt5`.
  - Check: `qmake -v` should report Qt version 5.x.

- Missing Qt 5 modules (widgets/opengl/xml)
  - Symptom: compile fails finding `<QtWidgets/...>` or OpenGL headers.
  - Fix (Debian/Ubuntu):
    ```bash
    sudo apt install qtbase5-dev qtbase5-dev-tools qttools5-dev-tools libglu1-mesa-dev mesa-common-dev
    ```
    Fix (Fedora):
    ```bash
    sudo dnf install qt5-qtbase-devel mesa-libGLU-devel
    ```

- ODE submodule not initialized
  - Symptom: missing sources under `src/HenocUniverse/ode/` or submodule build errors.
  - Fix:
    ```bash
    git submodule update --init --recursive
    ```

- ODE autotools warnings/errors during bootstrap
  - Symptom: messages about obsolete macros or libtool warnings.
  - Notes: warnings are typically harmless. If bootstrap fails, install autotools:
    ```bash
    # Debian/Ubuntu
    sudo apt install autoconf automake libtool
    # Fedora
    sudo dnf install autoconf automake libtool
    ```

- OpenGL headers not found
  - Symptom: missing GL/GLU headers.
  - Fix (Debian/Ubuntu): `sudo apt install mesa-common-dev libglu1-mesa-dev`
  - Fix (Fedora): `sudo dnf install mesa-libGL-devel mesa-libGLU-devel`

- Branding export script fails
  - Symptom: `export.sh` cannot find inkscape/rsvg/convert.
  - Fix:
    ```bash
    # Choose one of the following exporters
    sudo apt install inkscape
    # or
    sudo apt install librsvg2-bin   # provides rsvg-convert
    # or
    sudo apt install imagemagick
    ```
  - Also ensure it is executable: `chmod +x branding/export.sh`.

- Runtime black screen or unexpected physics
  - Ensure ENMARCA (frame) exists if you expect solid boundaries. The app auto‑adds ODE boundary walls when the frame lines are detected at the scene edges.
  - Verify world settings (gravity defaults to 9.8). Adjust Fill Brightness and Density Scale in the World dialog as needed.

## Authors

*   Román Cisneros José Eduardo
*   Soto Escobar Álvaro Antonio
*   Valdez Guzmán Rubén
*   Villanueva Cortez Eder

[![SonarQube Cloud](https://sonarcloud.io/images/project_badges/sonarcloud-light.svg)](https://sonarcloud.io/summary/new_code?id=opensource-latinamerica_henoc)
