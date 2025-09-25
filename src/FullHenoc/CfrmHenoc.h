#ifndef __CFRMHENOC_H__
#define __CFRMHENOC_H__

#include "ui_frmHenoc.h"
#include <QtWidgets>
#include <QtXml>
#include <iostream>
#include "henocUniverseI.h"

// This struct holds the simulation's world-specific properties.
// It is edited by the CfrmMundo dialog.
struct Whstc {
	int fAnim;
	float delta;
	float cmv;
	float fcs;
	float gravity;
	float erp;
	float lin;
	float co;
	// New rendering/physics tunables
	float fillBrightFactor; // 0..1
	float densityScale;     // multiplier for mass->density scaling
	float lineThicknessPx;  // default line thickness in pixels
};

using namespace std;

using namespace Ui;

class DiagramScene; 
class QGraphicsItem;
class QOpenGLWidget;
class QTimer;
class HLine;

class CfrmHenoc : public QMainWindow, frmHenoc {
	Q_OBJECT
	public:
		CfrmHenoc(QWidget * parent = 0, Qt::WindowFlags flags = 0 );
	public slots:
		void changeProperties();
		void Delete();
		void Stop();
		void Save();
		void Open();
		void Play();
		void AddBox();
		void AddLine();		void AddCatapult();
		void AddBall();
		void itemInserted(QGraphicsItem *);
        void onSelectionChanged();
        void onLineThicknessChanged(qreal w);
	private:
		void XkiAddLine(QDomElement &, int x1, int y1, int x2, int y2, float friction, int colMask, int frictionMask);
		void XkiAddBox(QDomElement &, int x, int y, int w, int h, float mass, float friction, float bounceFactor, float bounceVelocity, int colMask, int frictionMask, int rotation);
		void XkiAddBall(QDomElement &, int x, int y, int r, float mass, float friction, float bounceFactor, float bounceVelocity, int colMask, int frictionMask, int rotation);
		QOpenGLWidget *glWidget;
		DiagramScene *scene;
		QTimer *timer;
		Whstc myWorldProp;
        // ENMARCA (frame) state and references
        bool m_enmarcaEnabled = false;
        QList<HLine*> m_enmarcaLines;
};

#endif
