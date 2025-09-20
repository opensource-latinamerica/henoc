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

    // Populate mask preset combos
    auto populatePresets = [](QComboBox *cmb){
        cmb->clear();
        cmb->addItem("All (0xFFFFFFFF)", QVariant::fromValue(qulonglong(0xFFFFFFFFu)));
        cmb->addItem("None (0x0)", QVariant::fromValue(qulonglong(0x0u)));
        cmb->addItem("Group 1 (0x1)", QVariant::fromValue(qulonglong(0x1u)));
        cmb->addItem("Group 2 (0x2)", QVariant::fromValue(qulonglong(0x2u)));
        cmb->addItem("Group 3 (0x4)", QVariant::fromValue(qulonglong(0x4u)));
        cmb->addItem("Custom", QVariant());
    };
    populatePresets(cmbCollisionPreset);
    populatePresets(cmbFrictionPreset);

    auto selectPresetForValue = [](QComboBox *cmb, quint32 value){
        for (int i = 0; i < cmb->count(); ++i){
            QVariant v = cmb->itemData(i);
            if (v.isValid() && v.toULongLong() == (qulonglong)value){ cmb->setCurrentIndex(i); return; }
        }
        // Not found → Custom
        int idx = cmb->findText("Custom");
        if (idx >= 0) cmb->setCurrentIndex(idx);
    };
    selectPresetForValue(cmbCollisionPreset, (quint32)mObj->getColMask());
    selectPresetForValue(cmbFrictionPreset, (quint32)mObj->getFrictionMask());

    // Wire combo -> spinbox
    connect(cmbCollisionPreset, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int idx){
        QVariant v = cmbCollisionPreset->itemData(idx);
        if (v.isValid()){
            QSignalBlocker b(intcolisiones);
            intcolisiones->setValue((int)v.toULongLong());
        }
    });
    connect(cmbFrictionPreset, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int idx){
        QVariant v = cmbFrictionPreset->itemData(idx);
        if (v.isValid()){
            QSignalBlocker b(intmascaraDeFriccion);
            intmascaraDeFriccion->setValue((int)v.toULongLong());
        }
    });

    // Wire spinbox -> combo (show Custom if not matching)
    auto updateComboForSpin = [](QSpinBox *spin, QComboBox *cmb){
        quint32 value = (quint32)spin->value();
        int match = -1;
        for (int i = 0; i < cmb->count(); ++i){
            QVariant v = cmb->itemData(i);
            if (v.isValid() && v.toULongLong() == (qulonglong)value){ match = i; break; }
        }
        QSignalBlocker b(cmb);
        if (match >= 0) cmb->setCurrentIndex(match);
        else {
            int idx = cmb->findText("Custom");
            if (idx >= 0) cmb->setCurrentIndex(idx);
        }
    };
    connect(intcolisiones, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int){ updateComboForSpin(intcolisiones, cmbCollisionPreset); });
    connect(intmascaraDeFriccion, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int){ updateComboForSpin(intmascaraDeFriccion, cmbFrictionPreset); });

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

