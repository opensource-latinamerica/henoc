#include "glviewport.h"
#include <QPainter>
#include <QElapsedTimer>

GLViewport::GLViewport(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

void GLViewport::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.15f, 0.18f, 0.22f, 1.0f);
}

void GLViewport::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLViewport::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Basic step: advance simple gravity
    stepSimulation(1.0f); // one unit per timer tick; external code controls timer

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    // Draw items
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

    // Small debug watermark so users know GL is live
    p.setPen(Qt::yellow);
    p.setFont(QFont("Sans", 10, QFont::DemiBold));
    p.drawText(QRect(8, 8, width() - 16, 20), Qt::AlignLeft | Qt::AlignVCenter, QStringLiteral("Play Mode"));
}

void GLViewport::clearObjects()
{
    m_items.clear();
}

void GLViewport::addRectangle(int x, int y, int w, int h, float rotationDeg, const QColor &fill, const QColor &stroke, bool dynamic)
{
    GLItem it;
    it.type = GLItem::Box;
    it.x = x;
    it.y = y;
    it.w = w;
    it.h = h;
    it.rotDeg = rotationDeg;
    it.fill = fill;
    it.stroke = stroke;
    it.dynamic = dynamic;
    m_items.push_back(it);
}

void GLViewport::addCircle(int cx, int cy, int r, const QColor &fill, const QColor &stroke, bool dynamic)
{
    GLItem it;
    it.type = GLItem::Ball;
    it.cx = cx;
    it.cy = cy;
    it.r = r;
    it.fill = fill;
    it.stroke = stroke;
    it.dynamic = dynamic;
    m_items.push_back(it);
}

void GLViewport::addLine(int x1, int y1, int x2, int y2, const QColor &color, float width)
{
    GLItem it;
    it.type = GLItem::Line;
    it.x1 = x1;
    it.y1 = y1;
    it.x2 = x2;
    it.y2 = y2;
    it.stroke = color;
    it.fill = Qt::transparent;
    it.lw = width;
    it.dynamic = false;
    m_items.push_back(it);
}

void GLViewport::stepSimulation(float dt)
{
    if (m_gravity == 0.0f) return;
    const float g = m_gravity;
    const float bottom = static_cast<float>(height());
    for (auto &it : m_items) {
        if (!it.dynamic) continue;
        switch (it.type) {
        case GLItem::Ball: {
            it.vy += g * dt;
            it.cy += it.vy * dt;
            if (it.cy + it.r > bottom) {
                it.cy = bottom - it.r;
                it.vy *= -0.5f; // simple bounce
            }
            break;
        }
        case GLItem::Box: {
            it.vy += g * dt;
            it.y += it.vy * dt;
            if (it.y + it.h > bottom) {
                it.y = bottom - it.h;
                it.vy *= -0.3f;
            }
            break;
        }
        case GLItem::Line:
            break;
        }
    }
}
