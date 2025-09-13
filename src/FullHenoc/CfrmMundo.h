#ifndef __CFRMMUNDO_H__
#define __CFRMMUNDO_H__

#include "ui_frmMundo.h"
#include <QtGui>
#include "CfrmHenoc.h"

using namespace Ui;

class CfrmMundo : public QDialog, frmMundo{
	Q_OBJECT
	public:
		CfrmMundo(Whstc *pW, QWidget * parent = 0, Qt::WindowFlags f = 0);
	public slots:
		void Aceptar();
		void Limpiar();
		void Cancelar();
	private:
		Whstc *mW;
};

#endif
