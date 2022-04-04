#include<QtGui>
#include<QtCore>
#include<CfrmPeCaLi.h>
#include<diagramscene.h>

#include<HObject.h>
#include<HLine.h>
#include<HBox.h>
#include<HBall.h>


CfrmPeCaLi::CfrmPeCaLi(HObject *_obj, QWidget *parent, Qt::WindowFlags f):QDialog(parent,f),mObj(_obj){
	setupUi(this);
	
	intalto->setEnabled(true);
	intlargo->setEnabled(true);
	intRadio->setEnabled(true);	
	intRotacion->setEnabled(true);
	connect(btnAceptar, SIGNAL(clicked()), this, SLOT(Aceptar()));
	connect(btnCancelar, SIGNAL(clicked()), this, SLOT(Cancelar()));
	connect(btnLimpiar, SIGNAL(clicked()), this, SLOT(Limpiar()));
	fltdensidad->setValue( mObj->getMass() );
	fltfriccion->setValue( mObj->getFriction() );
	fltfactorDeRebote->setValue( mObj->getBounceFactor() );
	fltvelocidadDeRebote->setValue( mObj->getBounceVelocity() );
	intcolisiones->setValue( mObj->getColMask() );
	intmascaraDeFriccion->setValue( mObj->getFrictionMask() );
	intRotacion->setValue( (int)mObj->getRotation() );
	mObj->lastRotation = mObj->getRotation();

	switch( mObj->getType() ){
		case 1:
			intRadio->setEnabled(false);
			intalto->setValue( mObj->height() );
			intlargo->setValue( mObj->width() );	
			break;
		case 2:
			intalto->setEnabled(false);
			intlargo->setEnabled(false);
			intRotacion->setEnabled(false);
			intRadio->setValue( mObj->width() );
			break;
		case 3:
			intRadio->setEnabled(false);	
			intalto->setEnabled(false);
			intalto->setEnabled(false);
			intRotacion->setEnabled(false);
			break;
	}

}

void CfrmPeCaLi::Aceptar(){
	mObj->setMass( fltdensidad->value() );
	mObj->setFriction( fltfriccion->value() );
	mObj->setBounceFactor( fltfactorDeRebote->value() );
	mObj->setBounceVelocity( fltvelocidadDeRebote->value() );
	mObj->setColMask( intcolisiones->value() );
	mObj->setFrictionMask( intmascaraDeFriccion->value() );	
	mObj->setRotation( intRotacion->value() ); 

	switch( mObj->getType() ){
		case 1:
			mObj->setWidth( intlargo->value() );	
			mObj->setHeight( intalto->value() );	
			break;
		case 2:
			mObj->setWidth( intRadio->value() );	
			mObj->setHeight( intRadio->value() );	
			break;
	}


	accept();
}


void CfrmPeCaLi::Limpiar(){

}

void CfrmPeCaLi::Cancelar(){
	reject();
}


