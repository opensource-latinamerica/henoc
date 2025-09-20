/*
 * TT102 HENOC 
 * 2008 @ ESCOM-IPN
 * Implementacion de HenocUniverse
 */
 

#ifndef HENOCUNIVERSEI_H
#define HENOCUNIVERSEI_H

/*
 * This header acts as an intermediary to include the ODE library headers.
 * It resolves a name collision for the 'round' function which exists in both
 * the standard C++ library and the ODE library. This is a common issue when
 * integrating older C libraries with modern C++ code.
 */

// The ODE library can conflict with the standard C++ library over the
// definition of 'round'. To prevent a compilation error, we undefine
// 'round' before including the ODE header.
#ifdef round
#undef round
#endif

#include <ode/ode.h>
#include "henocUniverse.h"

// Lightweight API to drive the Henoc ODE world from Qt widgets
// without exposing internal wrapper types.
struct Whstc; // forward from CfrmHenoc.h

namespace HenocUniverseI {
    // Initialize GL state and ODE world (must be called with a current GL context).
    void HU_Init();
    // Resize viewport orthographic projection.
    void HU_Resize(int w, int h);
    // Apply world parameters (gravity, ERP, etc.).
    void HU_SetWorldParams(const Whstc &p);
    // Clear all existing objects from the world/space.
    void HU_ClearSpace();
    // Add primitives (pixel units, rotation in degrees).
    void HU_AddBox(int cx, int cy, int w, int h, float mass, float friction, float bounceFactor, float bounceVelocity, int colMask, int frictionMask, float rotationDeg, float thetaColor);
    void HU_AddBall(int cx, int cy, int r, float mass, float friction, float bounceFactor, float bounceVelocity, int colMask, int frictionMask, float rotationDeg, float thetaColor);
    void HU_AddLine(int x1, int y1, int x2, int y2, float friction, int colMask, int frictionMask, float thetaColor);
    // Advance physics and draw one frame.
    void HU_StepAndDraw(float delta);
}

#endif // HENOCUNIVERSEI_H
