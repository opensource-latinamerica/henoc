#ifndef __CFRMPECALI_H__
#define __CFRMPECALI_H__

#include "ui_frmPeCaLi.h"
#include <QtGui>
#include <HObject.h>

using namespace Ui;

class HLine;
class HBox;
class HBall;

class CfrmPeCaLi : public QDialog, frmPeCaLi{
	Q_OBJECT
	public:
		CfrmPeCaLi(HObject *, QWidget * parent = 0, Qt::WindowFlags f = 0);
	public slots:
		void Aceptar();
		void Limpiar();
		void Cancelar();
	private:
		HObject *mObj;
};

#endif
