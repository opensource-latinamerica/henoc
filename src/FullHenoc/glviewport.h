#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QVector>
#include <QColor>

struct GLItem {
    enum Type { Box, Ball, Line } type;
    QColor stroke;
    QColor fill;
    bool dynamic = false;
    float vx = 0.0f;
    float vy = 0.0f;
    float x = 0, y = 0, w = 0, h = 0, rotDeg = 0;
    float cx = 0, cy = 0, r = 0;
    float x1 = 0, y1 = 0, x2 = 0, y2 = 0, lw = 1;
};

class GLViewport : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    explicit GLViewport(QWidget *parent = nullptr);
    void setStepDelta(float dt) { m_step = dt; }
    void clearObjects();
    void addRectangle(int x, int y, int w, int h, float rotationDeg, const QColor &fill, const QColor &stroke, bool dynamic = true);
    void addCircle(int cx, int cy, int r, const QColor &fill, const QColor &stroke, bool dynamic = true);
    void addLine(int x1, int y1, int x2, int y2, const QColor &color, float width = 2.0f);
    void setGravity(float g) { m_gravity = g; }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void stepSimulation(float dt);
    QVector<GLItem> m_items;
    float m_gravity = 0.0f;
    float m_step = 0.015f;
};
