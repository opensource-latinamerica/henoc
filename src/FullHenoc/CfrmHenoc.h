#ifndef __CFRMHENOC_H__
#define __CFRMHENOC_H__

#include "ui_frmHenoc.h"
#include<QtGui>
#include<QtXml>
#include<iostream>
#include<henocUniverseI.h>


using namespace std;

using namespace Ui;

class DiagramScene; 
class QGraphicsItem;
class GLWidget;
class QTimer;

class CfrmHenoc : public QMainWindow, frmHenoc {
	Q_OBJECT
	public:
		CfrmHenoc(QWidget * parent = 0, Qt::WFlags flags = 0 );
	public slots:
		void changeProperties();
		void Delete();
		void Stop();
		void Save();
		void Open();
		void Play();
		void AddBox();
		void AddLine();
		void AddCatapult();
		void AddBall();
		void itemInserted(QGraphicsItem *);
	private:
		void XkiAddLine(QDomElement &, int x1, int y1, int x2, int y2, float friction, int colMask, int frictionMask);
		void XkiAddBox(QDomElement &, int x, int y, int w, int h, float mass, float friction, float bounceFactor, float bounceVelocity, int colMask, int frictionMask, int rotation);
		void XkiAddBall(QDomElement &, int x, int y, int r, float mass, float friction, float bounceFactor, float bounceVelocity, int colMask, int frictionMask, int rotation);
		GLWidget *glWidget;
		DiagramScene *scene;
		QTimer *timer;
		HenocUniverseI::Whstc myWorldProp;
};

#endif
