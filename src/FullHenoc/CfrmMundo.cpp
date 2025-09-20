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

    // New tunables
    fltFillBright->setRange(0.0, 1.0);
    fltFillBright->setSingleStep(0.05);
    fltFillBright->setValue(mW->fillBrightFactor);

    fltDensityScale->setRange(0.01, 10.0);
    fltDensityScale->setSingleStep(0.1);
    fltDensityScale->setValue(mW->densityScale);

    cmbLineThickness->clear();
    cmbLineThickness->addItem("Thin (1 px)", 1.0);
    cmbLineThickness->addItem("Normal (2 px)", 2.0);
    cmbLineThickness->addItem("Bold (4 px)", 4.0);
    cmbLineThickness->addItem("Heavy (8 px)", 8.0);
    // Select closest preset
    int bestIdx = 0; double bestDiff = 1e9;
    for (int i=0;i<cmbLineThickness->count();++i){
        double v = cmbLineThickness->itemData(i).toDouble();
        double d = std::fabs(v - mW->lineThicknessPx);
        if (d < bestDiff){ bestDiff = d; bestIdx = i; }
    }
    cmbLineThickness->setCurrentIndex(bestIdx);

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
    mW->fillBrightFactor = (float)fltFillBright->value();
    mW->densityScale = (float)fltDensityScale->value();
    mW->lineThicknessPx = (float)cmbLineThickness->currentData().toDouble();
	accept();
}

void CfrmMundo::Limpiar(){

}

void CfrmMundo::Cancelar(){
	reject();
}

