## ==== THIS PROJECT IS STILL BEING MIGRATED TO LATEST LIBRARY VERSIONS ====
## ====               CURRENTLY DOES NOT RUN PROPERLY                   ==== 
## ====         WE WILL DROP THESE LINES ONCE THIS CHANGES              ====

# Henoc: Physics Sketch Animator

Henoc is a sophisticated physics simulation software that brings user-drawn sketches to life. By leveraging a pattern recognition agent, Henoc can interpret and animate physics sketches from a simple blackboard interface, providing a dynamic and interactive way to visualize physics principles.

## Overview

The core of Henoc is its ability to transform static, hand-drawn sketches into moving simulations. Users can draw various shapes and objects on a virtual blackboard, and Henoc's engine will apply physics rules to animate them. This makes it an excellent tool for educational purposes, rapid prototyping of physics-based scenes, or simply for creative exploration.

## Dependencies

To build and run Henoc, you will need the following software installed on your system:

*   **A C++ compiler:** Such as `g++`.
*   **Qt Framework:** The project requires the `opengl`, `widgets`, and `xml` modules.
*   **GNU Make:** For managing the build process.

## Building the Project

The project uses a `Makefile` that automates the entire build process, including the compilation of the included ODE (Open Dynamics Engine) library.

To build the Henoc application, follow these steps:

1.  **Navigate to the source directory:**

    ```bash
    cd src
    ```

2.  **Run the make command:**

    ```bash
    make
    ```

This will compile all the necessary components and create the `henoc` executable in the `src` directory.

## Running Henoc

Once the build is complete, you can run the application from the `src` directory:

```bash
./henoc
```

## Cleaning the Project

To remove all build artifacts, including the executable and intermediate object files, run the following command from the `src` directory:

```bash
make clean
```

## Authors

*   Román Cisneros José Eduardo
*   Soto Escobar Álvaro Antonio
*   Valdez Guzmán Rubén
*   Villanueva Cortez Eder
