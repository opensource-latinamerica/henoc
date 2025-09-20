#include<QtGui>
#include<QtCore>
#include<CfrmPeCaLi.h>
#include<diagramscene.h>

#include<HObject.h>
#include<HLine.h>
#include<HBox.h>
#include<HBall.h>


CfrmPeCaLi::CfrmPeCaLi(HObject *_obj, QGraphicsItem *item, QWidget *parent, Qt::WindowFlags f):QDialog(parent,f),mObj(_obj),mItem(item){
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
    // Additional groups
    cmbCollisionPreset->insertItem(5, "Group 4 (0x8)", QVariant::fromValue(qulonglong(0x8u)));
    cmbCollisionPreset->insertItem(6, "Group 5 (0x10)", QVariant::fromValue(qulonglong(0x10u)));
    cmbCollisionPreset->insertItem(7, "Group 6 (0x20)", QVariant::fromValue(qulonglong(0x20u)));
    cmbCollisionPreset->insertItem(8, "Group 7 (0x40)", QVariant::fromValue(qulonglong(0x40u)));
    cmbCollisionPreset->insertItem(9, "Group 8 (0x80)", QVariant::fromValue(qulonglong(0x80u)));
    cmbFrictionPreset->insertItem(5, "Group 4 (0x8)", QVariant::fromValue(qulonglong(0x8u)));
    cmbFrictionPreset->insertItem(6, "Group 5 (0x10)", QVariant::fromValue(qulonglong(0x10u)));
    cmbFrictionPreset->insertItem(7, "Group 6 (0x20)", QVariant::fromValue(qulonglong(0x20u)));
    cmbFrictionPreset->insertItem(8, "Group 7 (0x40)", QVariant::fromValue(qulonglong(0x40u)));
    cmbFrictionPreset->insertItem(9, "Group 8 (0x80)", QVariant::fromValue(qulonglong(0x80u)));
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

    // Tooltips
    lblColisiones->setToolTip("Bitmask: objects collide if (A & B) != 0. Use presets for common groups.");
    intcolisiones->setToolTip("Integer mask; combine bits for multiple groups.");
    cmbCollisionPreset->setToolTip("Select a collision group mask or choose Custom and edit the integer.");
    lblMascaraDeFriccion->setToolTip("Bitmask: friction applies if (A & B) != 0. Use presets for common groups.");
    intmascaraDeFriccion->setToolTip("Integer mask; combine bits for multiple groups.");
    cmbFrictionPreset->setToolTip("Select a friction mask or choose Custom and edit the integer.");

    // Line thickness controls (only visible for lines)
    auto setLineControlsVisible = [this](bool vis){
        lblLineThicknessObj->setVisible(vis);
        cmbLineThicknessObj->setVisible(vis);
        fltLineThicknessObj->setVisible(vis);
    };
    bool isLine = (mObj->getType() == 3);
    setLineControlsVisible(isLine);
    if (isLine){
        cmbLineThicknessObj->clear();
        cmbLineThicknessObj->addItem("Thin (1 px)", 1.0);
        cmbLineThicknessObj->addItem("Normal (2 px)", 2.0);
        cmbLineThicknessObj->addItem("Bold (4 px)", 4.0);
        cmbLineThicknessObj->addItem("Heavy (8 px)", 8.0);
        cmbLineThicknessObj->addItem("Custom", QVariant());
        float current = 2.0f;
        if (mItem){
            if (auto line = qgraphicsitem_cast<QGraphicsLineItem*>(mItem)){
                current = line->pen().widthF();
            }
        }
        fltLineThicknessObj->setRange(0.5, 64.0);
        fltLineThicknessObj->setSingleStep(0.5);
        fltLineThicknessObj->setValue(current);
        // select closest preset
        int bestIdx = 0; double bestDiff = 1e9;
        for (int i=0;i<cmbLineThicknessObj->count();++i){
            QVariant d = cmbLineThicknessObj->itemData(i);
            if (!d.isValid()) continue;
            double v = d.toDouble(); double diff = std::fabs(v - current);
            if (diff < bestDiff){ bestDiff = diff; bestIdx = i; }
        }
        cmbLineThicknessObj->setCurrentIndex(bestIdx);
        auto updateCustomEnabledObj = [this](){
            bool custom = !cmbLineThicknessObj->currentData().isValid();
            fltLineThicknessObj->setEnabled(custom);
        };
        updateCustomEnabledObj();
        connect(cmbLineThicknessObj, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int){ updateCustomEnabledObj(); });
        lblLineThicknessObj->setToolTip("Per-line thickness; choose a preset or Custom and set a value.");
        cmbLineThicknessObj->setToolTip("Preset thickness; choose Custom to set a specific value.");
        fltLineThicknessObj->setToolTip("Custom line thickness in pixels.");
    }

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
		case 3:
			if (mItem){
				if (auto line = qgraphicsitem_cast<QGraphicsLineItem*>(mItem)){
					qreal w = fltLineThicknessObj->isEnabled() ? (qreal)fltLineThicknessObj->value()
																: (qreal)cmbLineThicknessObj->currentData().toDouble();
					QPen p = line->pen();
					p.setWidthF(w);
					line->setPen(p);
				}
			}
			break;
	}


	accept();
}


void CfrmPeCaLi::Limpiar(){

}

void CfrmPeCaLi::Cancelar(){
	reject();
}
