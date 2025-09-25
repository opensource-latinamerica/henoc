#include "ode_bridge.h"
#include <ode/ode.h>
#include "CfrmHenoc.h"
#include <list>
#include <cmath>
#include <GL/gl.h>

namespace {
    struct Obj {
        dBodyID body = 0;
        dGeomID geom = 0;
        bool dynamic = false;
        enum Type { Box, Ball, Line } type;
        // for drawing
        float w=0, h=0, r=0; // sizes; for line we store length as w and thickness h
        float mu = 1.0f;
        float bounce = 0.35f;
        float bounceVel = 0.0f;
        // colors
        float fillR=0.8f, fillG=0.8f, fillB=0.8f;
        float strokeR=0.0f, strokeG=0.0f, strokeB=0.0f;
        unsigned int collisionMask = ~0u;
        unsigned int frictionMask = ~0u;
    };

    dWorldID g_world = 0;
    dSpaceID g_space = 0;
    dJointGroupID g_contactGroup = 0;
    std::list<Obj*> g_objs;
    float g_fillBright = 0.5f;
    float g_densityScale = 1.0f;

    inline void ensureWorld(){
        if (g_world) return;
        dInitODE2(0);
        g_world = dWorldCreate();
        g_space = dSimpleSpaceCreate(0);
        g_contactGroup = dJointGroupCreate(0);
        dWorldSetGravity(g_world, 0, 0.5, 0);
        dWorldSetERP(g_world, 0.2);
        dWorldSetCFM(g_world, 0.0);
    }

    void cleanup_space(){
        for (auto *o : g_objs){
            if (o->geom) dGeomDestroy(o->geom);
            if (o->body) dBodyDestroy(o->body);
            delete o;
        }
        g_objs.clear();
        if (g_contactGroup) dJointGroupEmpty(g_contactGroup);
    }

    void nearCallback(void*, dGeomID g1, dGeomID g2){
        Obj *o1 = static_cast<Obj*>(dGeomGetData(g1));
        Obj *o2 = static_cast<Obj*>(dGeomGetData(g2));
        if (!o1 || !o2) return;

        // Only collide dynamic vs any or static vs dynamic
        if (!o1->dynamic && !o2->dynamic) return;
        // Collision mask filtering
        if ((o1->collisionMask & o2->collisionMask) == 0) return;

        dContact contacts[8];
        int n = dCollide(g1, g2, 8, &contacts[0].geom, sizeof(dContact));
        for (int i = 0; i < n; ++i){
            dContact &c = contacts[i];
            c.surface.mode = dContactBounce;
            if ((o1->frictionMask & o2->frictionMask) != 0)
                c.surface.mu = (o1->mu == dInfinity || o2->mu == dInfinity) ? dInfinity : (o1->mu * o2->mu);
            else
                c.surface.mu = 0;
            c.surface.bounce = (o1->bounce + o2->bounce) * 0.5f;
            c.surface.bounce_vel = (o1->bounceVel + o2->bounceVel) * 0.5f;
            dJointID j = dJointCreateContact(g_world, g_contactGroup, &c);
            dJointAttach(j, dGeomGetBody(c.geom.g1), dGeomGetBody(c.geom.g2));
        }
    }

    static void thetaToRGB(float theta, float &r, float &g, float &b){
        r = g = b = 0.f;
        if (theta < 0) theta = 360 + theta;
        if (theta >= 360) theta -= 360;
        if (theta < 120){ g = theta / 120.f; r = 1 - g; b = 0; }
        else if (theta < 240){ b = (theta - 120.f) / 120.f; g = 1 - b; r = 0; }
        else { r = (theta - 240.f) / 120.f; b = 1 - r; g = 0; }
    }

    static void increaseBrightness(float &r, float &g, float &b, float factor){
        r += factor; g += factor; b += factor;
        if (r > 1.f) r = 1.f; if (g > 1.f) g = 1.f; if (b > 1.f) b = 1.f;
    }
}

namespace ODEBridge {
    void Init(){
        ensureWorld();
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POINT_SMOOTH);
    }

    void Resize(int w, int h){
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // 1:1 pixels with origin at top-left
        glOrtho(0, w, h, 0, -2, 2);
        glMatrixMode(GL_MODELVIEW);
    }

    void SetWorldParams(const Whstc &p){
        ensureWorld();
        dWorldSetCFM(g_world, p.co);
        dWorldSetERP(g_world, p.erp);
        dWorldSetContactMaxCorrectingVel(g_world, p.cmv);
        dWorldSetContactSurfaceLayer(g_world, p.fcs);
        dWorldSetGravity(g_world, 0, p.gravity, 0);
        // Store global tunables
        g_fillBright = p.fillBrightFactor;
        g_densityScale = p.densityScale;
    }

    void Clear(){
        ensureWorld();
        cleanup_space();
    }

    void AddBox(int cx, int cy, int w, int h, float mass, float friction, float bounceFactor, float bounceVelocity, int colMask, int frictionMask, float rotationDeg, float thetaColor){
        ensureWorld();
        Obj *o = new Obj(); o->type = Obj::Box; o->dynamic = true; o->w = (float)w; o->h = (float)h; o->mu = friction; o->bounce = bounceFactor; o->bounceVel = bounceVelocity;
        float r,g,b; thetaToRGB(thetaColor, r,g,b); o->strokeR = r; o->strokeG = g; o->strokeB = b;
        // exact fromThetaIncreasedColor behavior with configurable factor
        float fr=r, fg=g, fb=b; increaseBrightness(fr, fg, fb, g_fillBright);
        o->fillR = fr; o->fillG = fg; o->fillB = fb;
        o->collisionMask = (unsigned int)colMask;
        o->frictionMask = (unsigned int)frictionMask;
        o->body = dBodyCreate(g_world);
        // scale density by pixel area, with global densityScale multiplier
        float area = std::max(1.f, (float)w * (float)h);
        float density = std::max(1e-4f, (mass / area));
        density *= g_densityScale;
        dMass m; dMassSetBox(&m, density, w, h, 1); dBodySetMass(o->body, &m);
        dBodySetPosition(o->body, cx, cy, 0);
        dMatrix3 R; dRFromAxisAndAngle(R, 0, 0, 1, rotationDeg * M_PI/180.0);
        dBodySetRotation(o->body, R);
        o->geom = dCreateBox(g_space, w, h, 1);
        dGeomSetBody(o->geom, o->body);
        dGeomSetData(o->geom, o);
        g_objs.push_back(o);
    }

    void AddBall(int cx, int cy, int r, float mass, float friction, float bounceFactor, float bounceVelocity, int colMask, int frictionMask, float /*rotationDeg*/, float thetaColor){
        ensureWorld();
        Obj *o = new Obj(); o->type = Obj::Ball; o->dynamic = true; o->r = (float)r; o->mu = friction; o->bounce = bounceFactor; o->bounceVel = bounceVelocity;
        float rr,gg,bb; thetaToRGB(thetaColor, rr,gg,bb); o->strokeR=rr; o->strokeG=gg; o->strokeB=bb; float fr2=rr, fg2=gg, fb2=bb; increaseBrightness(fr2, fg2, fb2, g_fillBright); o->fillR = fr2; o->fillG = fg2; o->fillB = fb2;
        o->collisionMask = (unsigned int)colMask;
        o->frictionMask = (unsigned int)frictionMask;
        o->body = dBodyCreate(g_world);
        // density scaled by circular pixel area, with global densityScale
        float areaS = std::max(1.f, (float)M_PI * r * r);
        float densityS = std::max(1e-4f, mass / areaS);
        densityS *= g_densityScale;
        dMass m; dMassSetSphere(&m, densityS, r); dBodySetMass(o->body, &m);
        dBodySetPosition(o->body, cx, cy, 0);
        o->geom = dCreateSphere(g_space, r);
        dGeomSetBody(o->geom, o->body);
        dGeomSetData(o->geom, o);
        g_objs.push_back(o);
    }

    void AddLine(int x1, int y1, int x2, int y2, float friction, int colMask, int frictionMask, float thetaColor, float thicknessPx){
        // Represent as a thin static box
        float dx = (float)(x2 - x1), dy = (float)(y2 - y1);
        float length = std::sqrt(dx*dx + dy*dy);
        float angle = std::atan2(dy, dx);
        float cx = (x1 + x2) * 0.5f, cy = (y1 + y2) * 0.5f;
        ensureWorld();
        Obj *o = new Obj(); o->type = Obj::Line; o->dynamic = false; o->w = length; o->h = std::max(1.f, thicknessPx); o->mu = friction; o->bounce = 0.0f; o->bounceVel = 0.0f;
        float r,g,b; thetaToRGB(thetaColor, r,g,b); o->strokeR=r; o->strokeG=g; o->strokeB=b; o->fillR=r; o->fillG=g; o->fillB=b;
        o->collisionMask = (unsigned int)colMask;
        o->frictionMask = (unsigned int)frictionMask;
        o->geom = dCreateBox(g_space, length, o->h, 1);
        dGeomSetData(o->geom, o);
        dGeomSetPosition(o->geom, cx, cy, 0);
        dMatrix3 R; dRFromAxisAndAngle(R, 0, 0, 1, angle);
        dGeomSetRotation(o->geom, R);
        g_objs.push_back(o);
    }

    void StepAndDraw(float delta){
        ensureWorld();
        // Collide
        dJointGroupEmpty(g_contactGroup);
        dSpaceCollide(g_space, 0, &nearCallback);
        dWorldQuickStep(g_world, delta);

        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        // Draw simple shapes from ODE state
        for (auto *o : g_objs){
            const dReal *R; const dReal *P;
            if (o->dynamic){ R = dBodyGetRotation(o->body); P = dBodyGetPosition(o->body); }
            else { R = dGeomGetRotation(o->geom); P = dGeomGetPosition(o->geom); }
            // Extract angle from rotation
            float angle = std::atan2(R[1*4+0], R[0*4+0]) * 180.0f / M_PI;
            glPushMatrix();
            glTranslatef(P[0], P[1], 0);
            glRotatef(angle, 0, 0, 1);
            if (o->type == Obj::Box){
                glColor4f(o->fillR, o->fillG, o->fillB, 1.0f);
                glBegin(GL_QUADS);
                glVertex2f(-o->w/2, -o->h/2); glVertex2f(o->w/2, -o->h/2);
                glVertex2f(o->w/2, o->h/2); glVertex2f(-o->w/2, o->h/2);
                glEnd();
                glColor4f(o->strokeR, o->strokeG, o->strokeB, 1);
                glBegin(GL_LINE_LOOP);
                glVertex2f(-o->w/2, -o->h/2); glVertex2f(o->w/2, -o->h/2);
                glVertex2f(o->w/2, o->h/2); glVertex2f(-o->w/2, o->h/2);
                glEnd();
            } else if (o->type == Obj::Ball){
                const int slices = (int)std::max(20.f, o->r);
                const float d = 2.f * M_PI / slices;
                glColor4f(o->fillR, o->fillG, o->fillB, 1.0f);
                glBegin(GL_TRIANGLE_FAN);
                glVertex2f(0,0);
                for (int i = 0; i <= slices; ++i){ float t = i * d; glVertex2f(std::sin(t) * o->r, std::cos(t) * o->r); }
                glEnd();
                glColor4f(o->strokeR, o->strokeG, o->strokeB, 1);
                glBegin(GL_LINE_LOOP);
                for (int i = 0; i < slices; ++i){ float t = i * d; glVertex2f(std::sin(t) * o->r, std::cos(t) * o->r); }
                glEnd();
            } else { // Line: draw as a thin quad for thickness
                glColor4f(o->strokeR, o->strokeG, o->strokeB, 1);
                float halfL = o->w * 0.5f;
                float halfT = o->h * 0.5f;
                glBegin(GL_QUADS);
                glVertex2f(-halfL, -halfT);
                glVertex2f( halfL, -halfT);
                glVertex2f( halfL,  halfT);
                glVertex2f(-halfL,  halfT);
                glEnd();
            }
            glPopMatrix();
        }
    }

    void AddBoundaryWalls(int width, int height, float wallThicknessPx){
        ensureWorld();
        float t = std::max(1.f, wallThicknessPx);
        // Draw walls exactly on the logical edges, matching scene lines
        // Top: centered at y = 0.5 to match 1px Qt line at y=0
        {
            Obj *o = new Obj(); o->type = Obj::Line; o->dynamic = false; o->w = (float)width; o->h = t; o->mu = dInfinity; o->collisionMask = ~0u; o->frictionMask = ~0u; o->strokeR=1.0f; o->strokeG=0.0f; o->strokeB=0.0f;
            o->geom = dCreateBox(g_space, width, t, 1);
            dGeomSetPosition(o->geom, width * 0.5f, 0.5f, 0);
            dGeomSetData(o->geom, o);
            g_objs.push_back(o);
        }
        // Bottom: centered at y = height - 0.5 to match Qt line at y=height
        {
            Obj *o = new Obj(); o->type = Obj::Line; o->dynamic = false; o->w = (float)width; o->h = t; o->mu = dInfinity; o->collisionMask = ~0u; o->frictionMask = ~0u; o->strokeR=1.0f; o->strokeG=0.0f; o->strokeB=0.0f;
            o->geom = dCreateBox(g_space, width, t, 1);
            dGeomSetPosition(o->geom, width * 0.5f, (float)height - 0.5f, 0);
            dGeomSetData(o->geom, o);
            g_objs.push_back(o);
        }
        // Left: centered at x = 0.5 to match Qt line at x=0
        {
            Obj *o = new Obj(); o->type = Obj::Line; o->dynamic = false; o->w = t; o->h = (float)height; o->mu = dInfinity; o->collisionMask = ~0u; o->frictionMask = ~0u; o->strokeR=1.0f; o->strokeG=0.0f; o->strokeB=0.0f;
            o->geom = dCreateBox(g_space, t, height, 1);
            dGeomSetPosition(o->geom, 0.5f, height * 0.5f, 0);
            dGeomSetData(o->geom, o);
            g_objs.push_back(o);
        }
        // Right: centered at x = width - 0.5 to match Qt line at x=width
        {
            Obj *o = new Obj(); o->type = Obj::Line; o->dynamic = false; o->w = t; o->h = (float)height; o->mu = dInfinity; o->collisionMask = ~0u; o->frictionMask = ~0u; o->strokeR=1.0f; o->strokeG=0.0f; o->strokeB=0.0f;
            o->geom = dCreateBox(g_space, t, height, 1);
            dGeomSetPosition(o->geom, (float)width - 0.5f, height * 0.5f, 0);
            dGeomSetData(o->geom, o);
            g_objs.push_back(o);
        }
    }
}
