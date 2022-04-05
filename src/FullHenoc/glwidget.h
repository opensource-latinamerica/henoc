#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "henocUniverseI.h"

using namespace HenocUniverseI;

class GLWidget : public QGLWidget{
	Q_OBJECT

public:
	GLWidget(QWidget *parent = 0);
	~GLWidget();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	void addBall(int, int, int, float, float, float, float, int, int, int, float);
	void addBox(int, int, int, int, float, float, float, float, int, int, int, float);
	void addLine(int, int, int, int, float, int, int, float);
	void setWorldParams(const Whstc &);
	void delSpace();

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);

public slots:
        void animate();

private:
	HenocWrapper app;

};

#endif
