#pragma once

#include <cstdint>

struct Whstc;

namespace ODEBridge {
    void Init();
    void Resize(int w, int h);
    void SetWorldParams(const Whstc &);
    void Clear();
    void AddBox(int cx, int cy, int w, int h, float mass, float friction, float bounceFactor, float bounceVelocity, int colMask, int frictionMask, float rotationDeg, float thetaColor);
    void AddBall(int cx, int cy, int r, float mass, float friction, float bounceFactor, float bounceVelocity, int colMask, int frictionMask, float rotationDeg, float thetaColor);
    void AddLine(int x1, int y1, int x2, int y2, float friction, int colMask, int frictionMask, float thetaColor, float thicknessPx);
    void StepAndDraw(float delta);
}
