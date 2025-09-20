#include "glviewport.h"
#include <QPainter>
#include <QElapsedTimer>
#include <cmath>
// Toggle ODE-backed simulation (1) vs. built-in painter physics (0)
#ifndef USE_ODE_BRIDGE
#define USE_ODE_BRIDGE 1
#endif

#if USE_ODE_BRIDGE
#include "ode_bridge.h"
#endif

static inline float dot2(float ax, float ay, float bx, float by) {
    return ax * bx + ay * by;
}

static inline float length2(float x, float y) {
    return std::sqrt(x * x + y * y);
}

static inline float clampf(float v, float lo, float hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

static inline void closestPointOnSegment(float px, float py, float ax, float ay, float bx, float by, float &outX, float &outY, float &t) {
    const float abx = bx - ax;
    const float aby = by - ay;
    const float abLen2 = abx * abx + aby * aby;
    if (abLen2 <= 1e-6f) { outX = ax; outY = ay; t = 0.0f; return; }
    const float apx = px - ax;
    const float apy = py - ay;
    t = (apx * abx + apy * aby) / abLen2;
    t = clampf(t, 0.0f, 1.0f);
    outX = ax + t * abx;
    outY = ay + t * aby;
}

GLViewport::GLViewport(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

void GLViewport::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
#if USE_ODE_BRIDGE
    ODEBridge::Init();
#endif
}

void GLViewport::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
#if USE_ODE_BRIDGE
    ODEBridge::Resize(w, h);
#endif
}

void GLViewport::paintGL()
{
    #if USE_ODE_BRIDGE
    ODEBridge::StepAndDraw(1.0f);
    #else
    // Painter-based fallback simulation
    // Basic step: advance simple gravity
    stepSimulation(1.0f);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    for (const auto &it : m_items) {
        switch (it.type) {
        case GLItem::Box: {
            p.save();
            p.translate(it.x + it.w / 2.0f, it.y + it.h / 2.0f);
            p.rotate(it.rotDeg);
            p.translate(-it.w / 2.0f, -it.h / 2.0f);
            p.setPen(QPen(it.stroke, 1));
            p.setBrush(it.fill);
            p.drawRect(QRectF(0, 0, it.w, it.h));
            p.restore();
            break;
        }
        case GLItem::Ball: {
            p.setPen(QPen(it.stroke, 1));
            p.setBrush(it.fill);
            const QRectF r(it.cx - it.r, it.cy - it.r, it.r * 2, it.r * 2);
            p.drawEllipse(r);
            break;
        }
        case GLItem::Line: {
            p.setPen(QPen(it.stroke, it.lw));
            p.drawLine(QPointF(it.x1, it.y1), QPointF(it.x2, it.y2));
            break;
        }
        }
    }
    #endif
}

void GLViewport::clearObjects()
{
    #if USE_ODE_BRIDGE
    ODEBridge::Clear();
    #else
    m_items.clear();
    #endif
}

void GLViewport::addRectangle(int x, int y, int w, int h, float rotationDeg, const QColor &fill, const QColor &stroke, bool dynamic)
{
    #if USE_ODE_BRIDGE
    (void)fill; (void)stroke; (void)dynamic;
    ODEBridge::AddBox(x + w/2, y + h/2, w, h, 5.0f, 1.0f, 0.35f, 0.f, ~0u, ~0u, rotationDeg, 0.f);
    #else
    GLItem it;
    it.type = GLItem::Box;
    it.x = x; it.y = y; it.w = w; it.h = h; it.rotDeg = rotationDeg;
    it.fill = fill; it.stroke = stroke; it.dynamic = dynamic;
    m_items.push_back(it);
    #endif
}

void GLViewport::addCircle(int cx, int cy, int r, const QColor &fill, const QColor &stroke, bool dynamic)
{
    #if USE_ODE_BRIDGE
    (void)fill; (void)stroke; (void)dynamic;
    ODEBridge::AddBall(cx, cy, r, 5.0f, 1.0f, 0.35f, 0.f, ~0u, ~0u, 0.f, 0.f);
    #else
    GLItem it; it.type = GLItem::Ball; it.cx = cx; it.cy = cy; it.r = r;
    it.fill = fill; it.stroke = stroke; it.dynamic = dynamic;
    m_items.push_back(it);
    #endif
}

void GLViewport::addLine(int x1, int y1, int x2, int y2, const QColor &color, float width)
{
    #if USE_ODE_BRIDGE
    (void)color; (void)width;
    ODEBridge::AddLine(x1, y1, x2, y2, 1.0f, ~0u, ~0u, 0.f);
    #else
    GLItem it; it.type = GLItem::Line; it.x1 = x1; it.y1 = y1; it.x2 = x2; it.y2 = y2; it.lw = width; it.stroke = color;
    it.fill = Qt::transparent; it.dynamic = false;
    m_items.push_back(it);
    #endif
}

void GLViewport::stepSimulation(float dt)
{
    const float g = m_gravity;
    const float bottom = static_cast<float>(height());
    const float mu = 0.25f;

    for (auto &it : m_items) {
        if (!it.dynamic) continue;

        it.vy += g * dt;
        if (it.type == GLItem::Ball) {
            it.cx += it.vx * dt;
            it.cy += it.vy * dt;
        } else if (it.type == GLItem::Box) {
            it.x  += it.vx * dt;
            it.y  += it.vy * dt;
        }

        if (it.type == GLItem::Ball) {
            if (it.cy + it.r > bottom) {
                it.cy = bottom - it.r;
                it.vy = (std::fabs(it.vy) < 0.8f) ? 0.0f : (-0.35f * it.vy);
                it.vx *= 0.98f;
            }
        } else if (it.type == GLItem::Box) {
            if (it.y + it.h > bottom) {
                it.y = bottom - it.h;
                it.vy = (std::fabs(it.vy) < 0.8f) ? 0.0f : (-0.25f * it.vy);
                it.vx *= 0.98f;
            }
        }

        for (const auto &ln : m_items) {
            if (ln.type != GLItem::Line) continue;
            float dx = ln.x2 - ln.x1;
            float dy = ln.y2 - ln.y1;
            float len = length2(dx, dy);
            if (len < 1e-5f) continue;
            float tx = dx / len;
            float ty = dy / len;
            float nx = -ty;
            float ny = tx;
            if (ny > 0.0f) { nx = -nx; ny = -ny; }

            if (it.type == GLItem::Ball) {
                float projx, projy, u;
                closestPointOnSegment(it.cx, it.cy, ln.x1, ln.y1, ln.x2, ln.y2, projx, projy, u);
                float dc = dot2(it.cx - ln.x1, it.cy - ln.y1, nx, ny);
                if (u >= 0.0f && u <= 1.0f && dc <= it.r + 0.5f) {
                    float pen = it.r - dc;
                    if (pen > 0.0f) {
                        it.cx += nx * pen;
                        it.cy += ny * pen;
                    }
                    float vn = dot2(it.vx, it.vy, nx, ny);
                    float vt = dot2(it.vx, it.vy, tx, ty);
                    if (vn < 0.0f) vn = (std::fabs(vn) < 0.8f) ? 0.0f : (-0.2f * vn);
                    else if (vn < 0.1f) vn = 0.0f;
                    float at = dot2(0.0f, g, tx, ty);
                    vt += at * dt;
                    vt *= (1.0f - mu * dt);
                    it.vx = vn * nx + vt * tx;
                    it.vy = vn * ny + vt * ty;
                }
            } else if (it.type == GLItem::Box) {
                float cx = it.x + it.w * 0.5f;
                float cy = it.y + it.h * 0.5f;
                float rr = 0.5f * std::min(it.w, it.h);
                float projx, projy, u;
                closestPointOnSegment(cx, cy, ln.x1, ln.y1, ln.x2, ln.y2, projx, projy, u);
                float dc = dot2(cx - ln.x1, cy - ln.y1, nx, ny);
                if (u >= 0.0f && u <= 1.0f && dc <= rr + 0.5f) {
                    float pen = rr - dc;
                    if (pen > 0.0f) {
                        cx += nx * pen;
                        cy += ny * pen;
                        it.x = cx - it.w * 0.5f;
                        it.y = cy - it.h * 0.5f;
                    }
                    float vn = dot2(it.vx, it.vy, nx, ny);
                    float vt = dot2(it.vx, it.vy, tx, ty);
                    if (vn < 0.0f) vn = (std::fabs(vn) < 0.8f) ? 0.0f : (-0.2f * vn);
                    else if (vn < 0.1f) vn = 0.0f;
                    float at = dot2(0.0f, g, tx, ty);
                    vt += at * dt;
                    vt *= (1.0f - mu * dt);
                    it.vx = vn * nx + vt * tx;
                    it.vy = vn * ny + vt * ty;
                }
            }
        }
    }
}
