#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class GLViewport : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    explicit GLViewport(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

