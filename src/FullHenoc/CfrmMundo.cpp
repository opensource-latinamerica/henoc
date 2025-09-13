#include<QtGui>
#include<CfrmMundo.h>

CfrmMundo::CfrmMundo(Whstc *pW, QWidget *parent, Qt::WindowFlags f):QDialog(parent,f),mW(pW){
	setupUi(this);
	connect(btnAceptar, SIGNAL(clicked()), this, SLOT(Aceptar()));
	connect(btnCancelar, SIGNAL(clicked()), this, SLOT(Cancelar()));
	connect(btnLimpiar, SIGNAL(clicked()), this, SLOT(Limpiar()));

	intfactorDeAnimacion->setValue( mW->fAnim );
	fltdelta->setValue( mW->delta );
	intCorrelacionMaximaDeVelocidad->setValue( mW->cmv );
	fltFactorDeContactoSuperficial->setValue( mW->fcs );
	intGravedad->setValue( mW->gravity );
	fltUmbralDeLinealidad->setValue( mW->lin );
	fltUmbralDeCorreccionDeErrores->setValue( mW->erp );
	intCoaccion->setValue( mW->co );

}

void CfrmMundo::Aceptar(){
	mW->fAnim = intfactorDeAnimacion->value();
	mW->delta = fltdelta->value();
	mW->cmv = intCorrelacionMaximaDeVelocidad->value();
	mW->fcs = fltFactorDeContactoSuperficial->value();
	mW->gravity = intGravedad->value();
	mW->lin = fltUmbralDeLinealidad->value();
	mW->erp = fltUmbralDeCorreccionDeErrores->value();
	mW->co = intCoaccion->value();
	accept();
}

void CfrmMundo::Limpiar(){

}

void CfrmMundo::Cancelar(){
	reject();
}


