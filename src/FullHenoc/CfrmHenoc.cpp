#include <QtWidgets>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include <CfrmHenoc.h>
#include <CfrmPeCaLi.h>
#include <CfrmMundo.h>
#include <diagramscene.h>

#include <iostream>
#include <HBox.h>
#include <HLine.h>
#include <HBall.h>

#include <henocUniverse.h>
#include "ode_bridge.h"
#include <QVBoxLayout>
#include "glviewport.h"
#include <cmath>

using namespace std;

CfrmHenoc::CfrmHenoc( QWidget * parent, Qt::WindowFlags flags):QMainWindow(parent, flags){
    setupUi(this);
	
	connect(btnBox, &QPushButton::clicked, this, &CfrmHenoc::AddBox);
	connect(btnLine, &QPushButton::clicked, this, &CfrmHenoc::AddLine);
	connect(btnCatapult, &QPushButton::clicked, this, &CfrmHenoc::AddCatapult);
	connect(btnBall, &QPushButton::clicked, this, &CfrmHenoc::AddBall);
	connect(btnPlay, &QPushButton::clicked, this, &CfrmHenoc::Play);
	connect(btnPropiedades, &QPushButton::clicked, this, &CfrmHenoc::changeProperties);
	connect(btnBorrar, &QPushButton::clicked, this, &CfrmHenoc::Delete);
	connect(bntStop, &QPushButton::clicked, this, &CfrmHenoc::Stop);
	connect(btnSave, &QPushButton::clicked, this, &CfrmHenoc::Save);
	connect(btnFrame, &QPushButton::clicked, this, &CfrmHenoc::Open);

	myWorldProp.fAnim = 3;
	myWorldProp.delta = 0.15f; 	
	myWorldProp.cmv = 1;
	myWorldProp.fcs = 0.01f;
    myWorldProp.gravity = 9.8f;
	myWorldProp.erp = 0.7f;
	myWorldProp.lin = 0.25f;
	myWorldProp.co = 0;
    myWorldProp.fillBrightFactor = 0.5f;
    myWorldProp.densityScale = 1.0f;
    myWorldProp.lineThicknessPx = 2.0f;

	scene = new DiagramScene(NULL);
    scene->setSceneRect(QRectF(0, 0, 655, 517));
    view->setScene(scene);
    // Keep scene anchored to top-left so its edges match the visible drawing area
    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);

	connect(scene, &DiagramScene::itemInserted, this, &CfrmHenoc::itemInserted);
    connect(scene, &QGraphicsScene::selectionChanged, this, &CfrmHenoc::onSelectionChanged);
    connect(scene, &DiagramScene::lineThicknessChanged, this, &CfrmHenoc::onLineThicknessChanged);

    // Embed an OpenGL viewport inside page_2 of the stacked widget
    glWidget = new GLViewport(stackedWidget->widget(1));
    QWidget *page2 = stackedWidget->widget(1);
    if (!page2->layout()) {
        auto *v = new QVBoxLayout(page2);
        // Match page 1 layout margins/spacing to avoid visible shifts
        v->setContentsMargins(9, 9, 9, 9);
        v->setSpacing(6);
    } else {
        // If it already had a layout, normalize margins/spacing too
        page2->layout()->setContentsMargins(9, 9, 9, 9);
        page2->layout()->setSpacing(6);
    }
    // Wrap GL viewport in a QFrame to match QGraphicsView's default look
    QFrame *glFrame = new QFrame(page2);
    glFrame->setFrameShape(QFrame::StyledPanel);
    glFrame->setFrameShadow(QFrame::Sunken);
    auto *inner = new QVBoxLayout(glFrame);
    inner->setContentsMargins(0, 0, 0, 0);
    inner->setSpacing(0);
    inner->addWidget(glWidget);
    page2->layout()->addWidget(glFrame);

    timer = new QTimer(this);
    // Trigger repaints while playing
    connect(timer, &QTimer::timeout, glWidget, QOverload<>::of(&QOpenGLWidget::update));

    // Initialize ENMARCA button to the "enable" icon (no frame yet)
    m_enmarcaEnabled = false;
    btnCatapult->setIcon(QIcon(QLatin1String(":/Botones/images/Botones/enmarcax.png")));
    btnCatapult->setToolTip(tr("Activar enmarca"));

    // Fix logical GL size to match the scene rectangle for 1:1 mapping
    const QRectF sr = scene->sceneRect();
    static_cast<GLViewport*>(glWidget)->setLogicalSize((int)std::lround(sr.width()), (int)std::lround(sr.height()));
}

void CfrmHenoc::Play(){
    // Show the GL viewport so its size is up to date
    stackedWidget->setCurrentIndex(1);

    QList<QGraphicsItem*> lista = scene->items();
    // Reset world and configure ODE params
    static_cast<GLViewport*>(glWidget)->clearObjects();
    ODEBridge::SetWorldParams(myWorldProp);
    static_cast<GLViewport*>(glWidget)->setStepDelta(myWorldProp.delta);

    // Use scene coordinates directly (GL logical size is fixed to scene size)
    const QRectF sr = scene->sceneRect();
    // Ensure ODE/GL logical projection matches the scene size
    ODEBridge::Resize((int)std::lround(sr.width()), (int)std::lround(sr.height()));

    bool boundaryAdded = false; // reset per Play invocation
    for(unsigned int i=0; i < lista.size(); i++){
        const QRectF sbr = lista.at(i)->sceneBoundingRect();
        const int xa = (int)std::lround(sbr.x());
        const int ya = (int)std::lround(sbr.y());
        const int wa = (int)std::lround(sbr.width());
        const int ha = (int)std::lround(sbr.height());
		
		QGraphicsRectItem * auxR = qgraphicsitem_cast<QGraphicsRectItem*>(lista.at(i));		
		QGraphicsEllipseItem * auxE = qgraphicsitem_cast<QGraphicsEllipseItem*>(lista.at(i));		
		QGraphicsLineItem * auxL = qgraphicsitem_cast<QGraphicsLineItem*>(lista.at(i));		

		QRect fao(xa,ya,wa,ha);
			
        if(0 != auxR){
            HObject obj = ((HBox*)auxR)->obj;
            const QPoint c = fao.center();
            const int ow = (int)std::lround(obj.width());
            const int oh = (int)std::lround(obj.height());
            ODEBridge::AddBox(c.x(), c.y(), ow, oh,
                              std::fabs(obj.getMass()), obj.getFriction(), obj.getBounceFactor(), obj.getBounceVelocity(),
                              obj.getColMask(), obj.getFrictionMask(), obj.getRotation(), obj.getColor());
        }
        else if(0 != auxE){
            HObject obj = ((HBall*)auxE)->obj;
            const int ow = (int)std::lround(obj.width());
            const int oh = (int)std::lround(obj.height());
            const int radius = std::max(1, (int)std::lround(std::min(ow, oh) * 0.5));
            ODEBridge::AddBall(fao.center().x(), fao.center().y(), radius,
                               std::fabs(obj.getMass()), obj.getFriction(), obj.getBounceFactor(), obj.getBounceVelocity(),
                               obj.getColMask(), obj.getFrictionMask(), obj.getRotation(), obj.getColor());
        }
        else if(0 != auxL){
            HObject obj = ((HLine*)auxL)->obj;
            // Detect if this is one of our ENMARCA lines using stored references
            HLine *hline = static_cast<HLine*>(auxL);
            bool isBoundary = m_enmarcaEnabled && m_enmarcaLines.contains(hline);
            if (isBoundary){
                if (!boundaryAdded){
                    boundaryAdded = true;
                    // Align to scene logical dimensions (same as GL logical size)
                    ODEBridge::AddBoundaryWalls((int)std::lround(sr.width()), (int)std::lround(sr.height()), 1.0f);
                }
                // Skip adding this line as a regular ODE line; walls handle it
            } else {
                // Use scene endpoints directly
                const QPointF sp1 = auxL->mapToScene(auxL->line().p1());
                const QPointF sp2 = auxL->mapToScene(auxL->line().p2());
                const int x1 = (int)std::lround(sp1.x());
                const int y1 = (int)std::lround(sp1.y());
                const int x2 = (int)std::lround(sp2.x());
                const int y2 = (int)std::lround(sp2.y());
                float thickness = auxL->pen().widthF();
                if (thickness <= 0.0f) thickness = myWorldProp.lineThicknessPx;
                ODEBridge::AddLine(x1, y1, x2, y2, obj.getFriction(), obj.getColMask(), obj.getFrictionMask(), obj.getColor(), thickness);
            }
        }

	}
    glWidget->update();
	//glWidget->setWorldParams(myWorldProp);		
	timer->start( myWorldProp.fAnim );
}

void CfrmHenoc::AddBox(){
	scene->setMode(DiagramScene::InsertItem);
	scene->setItemType(1);
	btnBox->setChecked(true);
}

void CfrmHenoc::AddLine(){
    scene->setMode(DiagramScene::InsertItem);
    scene->setItemType(3);
    scene->setDefaultLineWidth(myWorldProp.lineThicknessPx);
    btnBox->setChecked(true);
}

void CfrmHenoc::AddCatapult(){
    // Toggle ENMARCA (frame) on/off
    if (!m_enmarcaEnabled) {
        // Add frame lines matching the visible drawing area (viewport)
        auto addFrameLine = [&](const QPointF &a, const QPointF &b){
            HLine *item = new HLine(QLineF(a, b));
            item->setFlag(QGraphicsItem::ItemIsSelectable, false);
            float tColor = 0.0f;
            item->obj.setColor(tColor);
            static const qreal kFrameWidth = 1.0;
            static const int kFriction = 99;
            QPen pen(fromThetaColor(tColor), kFrameWidth);
            pen.setCosmetic(true);
            ((QGraphicsLineItem*)item)->setPen(pen);
            scene->addItem(item);
            item->obj.setType(3);
            item->obj.setFriction(kFriction);
            item->obj.setColMask(~0);
            item->obj.setFrictionMask(~0);
            m_enmarcaLines.append(item);
        };

        // Derive edges from the view's viewport so frame covers the full drawing space
        const int vw = view->viewport()->width();
        const int vh = view->viewport()->height();
        const QPointF tl = view->mapToScene(QPoint(0, 0));
        const QPointF topRight = view->mapToScene(QPoint(vw - 1, 0));
        const QPointF bl = view->mapToScene(QPoint(0, vh - 1));
        const QPointF br = view->mapToScene(QPoint(vw - 1, vh - 1));
        addFrameLine(tl, topRight); // top
        addFrameLine(topRight, br); // right
        addFrameLine(bl, br); // bottom
        addFrameLine(tl, bl); // left

        m_enmarcaEnabled = true;
        btnCatapult->setIcon(QIcon(QLatin1String(":/Botones/images/Botones/enmarcax.png")));
        btnCatapult->setToolTip(tr("Desactivar enmarca"));
    } else {
        // Remove previously added frame lines
        for (HLine *line : m_enmarcaLines) {
            if (!line) continue;
            if (line->scene() == scene) scene->removeItem(line);
            delete line;
        }
        m_enmarcaLines.clear();
        m_enmarcaEnabled = false;
        btnCatapult->setIcon(QIcon(QLatin1String(":/Botones/images/Botones/enmarcax.png")));
        btnCatapult->setToolTip(tr("Activar enmarca"));
    }
}

void CfrmHenoc::AddBall(){
	scene->setMode(DiagramScene::InsertItem);
	scene->setItemType(2);
	btnBox->setChecked(true);
}

void CfrmHenoc::itemInserted(QGraphicsItem *){
    scene->setMode(DiagramScene::MoveItem);
    btnBox->setChecked(false);
    scene->setItemType(0);
}

void CfrmHenoc::onSelectionChanged(){
    QList<QGraphicsItem*> sel = scene->selectedItems();
    if (sel.size() == 1){
        if (auto line = qgraphicsitem_cast<QGraphicsLineItem*>(sel.first())){
            qreal w = line->pen().widthF();
            statusBar()->showMessage(tr("Line width: %1 px").arg(w));
            return;
        }
    }
    statusBar()->clearMessage();
}

void CfrmHenoc::onLineThicknessChanged(qreal w){
    statusBar()->showMessage(tr("Line width: %1 px").arg(w), 3000);
}

void CfrmHenoc::changeProperties(){
	QList<QGraphicsItem*> lista = scene->selectedItems();

	if(0 == lista.size()){
		CfrmMundo dialog(&myWorldProp, this);
		if( QDialog::Accepted == dialog.exec() ){
		}
		return;	
	}

	if(lista.size() != 1){
		QMessageBox::warning(this, tr("Henoc"),tr("Error, solo 1 elemento puede estar seleccionado"));
		return;
	}

	CfrmPeCaLi dialog((HObject*)(&(((HBox*)lista.at(0))->obj)), lista.at(0), this);
	if( QDialog::Accepted == dialog.exec() ){
		if( 1 == ((HBox*)lista.at(0))->obj.getType() ){
			QRectF fao = ((QGraphicsRectItem*)lista.at(0))->rect();
			fao.setHeight(  ((HBox*)lista.at(0))->obj.height() );
			fao.setWidth( ((HBox*)lista.at(0))->obj.width() );
			((QGraphicsRectItem*)lista.at(0))->setRect(fao);
			QTransform matrix = ((QGraphicsRectItem*)lista.at(0))->transform();
			((QGraphicsRectItem*)lista.at(0))->moveBy(-((QGraphicsRectItem*)lista.at(0))->rect().width()/2,-((QGraphicsRectItem*)lista.at(0))->rect().height()/2);
			((QGraphicsRectItem*)lista.at(0))->setRotation( (((HBox*)lista.at(0))->obj.getRotation()) );
			((QGraphicsRectItem*)lista.at(0))->moveBy(((QGraphicsRectItem*)lista.at(0))->rect().width()/2,((QGraphicsRectItem*)lista.at(0))->rect().height()/2);
		}
		else{
			QRectF fao = ((QGraphicsEllipseItem*)lista.at(0))->rect();
			fao.setHeight(  ((HBox*)lista.at(0))->obj.height() );
			fao.setWidth( ((HBox*)lista.at(0))->obj.width() );
			((QGraphicsEllipseItem*)lista.at(0))->setRect(fao);
		}	
	}
}

void CfrmHenoc::Delete(){
	for (QGraphicsItem *item : scene->selectedItems()) {
		scene->removeItem(item);
	}
}

void CfrmHenoc::Stop(){
	timer->stop();
	stackedWidget->setCurrentIndex(0);
}


void CfrmHenoc::Save(){
	QString fileName = QFileDialog::getSaveFileName(this, tr("Guardar xki"),
				QDir::currentPath() + "/demo01.xki",
				tr("eXtended sKetch sImulation (*.xki)"));

	if ( fileName.isEmpty() ) return;

	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("XKI"),
			tr("No se pudo escribir en archivo %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return;
	}
        
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("Sketch");

	QList<QGraphicsItem*> lista = scene->items();
	int xa, ya, wa, ha;
	//glWidget->delSpace();
	for(unsigned int i=0; i < lista.size(); i++){
		xa =  ( lista.at(i)->mapToScene( lista.at(i)->boundingRect() )).boundingRect().x();  
		ya =  ( lista.at(i)->mapToScene( lista.at(i)->boundingRect() )).boundingRect().y(); 
		wa =  ( lista.at(i)->mapToScene( lista.at(i)->boundingRect() )).boundingRect().width(); 
		ha =  ( lista.at(i)->mapToScene( lista.at(i)->boundingRect() )).boundingRect().height();
		
		QGraphicsRectItem * auxR = qgraphicsitem_cast<QGraphicsRectItem*>(lista.at(i));		
		QGraphicsEllipseItem * auxE = qgraphicsitem_cast<QGraphicsEllipseItem*>(lista.at(i));		
		QGraphicsLineItem * auxL = qgraphicsitem_cast<QGraphicsLineItem*>(lista.at(i));		

		QRect fao(xa,ya,wa,ha);
			
		if(0 != auxR){
			xmlWriter.writeStartElement("Box");
			HObject obj = ((HBox*)auxR)->obj;
            xmlWriter.writeAttribute("x", QString::number(xa));
            xmlWriter.writeAttribute("y", QString::number(ya));
            xmlWriter.writeAttribute("width", QString::number(obj.width()));
            xmlWriter.writeAttribute("height", QString::number(obj.height()));
            xmlWriter.writeAttribute("mass", QString::number(obj.getMass()));
            xmlWriter.writeAttribute("friction", QString::number(obj.getFriction()));
            xmlWriter.writeAttribute("bounceFactor", QString::number(obj.getBounceFactor()));
            xmlWriter.writeAttribute("bounceVelocity", QString::number(obj.getBounceVelocity()));
            xmlWriter.writeAttribute("collisionMask", QString::number(obj.getColMask()));
            xmlWriter.writeAttribute("frictionMask", QString::number(obj.getFrictionMask()));
            xmlWriter.writeAttribute("rotation", QString::number(obj.getRotation()));
            xmlWriter.writeEndElement();
		}
		else if(0 != auxE){
			div_t q;
			q = div(ha,2);
			HObject obj = ((HBall*)auxE)->obj;
			xmlWriter.writeStartElement("Ball");
            xmlWriter.writeAttribute("x", QString::number(xa));
            xmlWriter.writeAttribute("y", QString::number(ya));
            xmlWriter.writeAttribute("radius", QString::number(wa));
            xmlWriter.writeAttribute("mass", QString::number(obj.getMass()));
            xmlWriter.writeAttribute("friction", QString::number(obj.getFriction()));
            xmlWriter.writeAttribute("bounceFactor", QString::number(obj.getBounceFactor()));
            xmlWriter.writeAttribute("bounceVelocity", QString::number(obj.getBounceVelocity()));
            xmlWriter.writeAttribute("collisionMask", QString::number(obj.getColMask()));
            xmlWriter.writeAttribute("frictionMask", QString::number(obj.getFrictionMask()));
            xmlWriter.writeAttribute("rotation", QString::number(obj.getRotation()));
            xmlWriter.writeEndElement();
		}
		else if(0 != auxL){
			xmlWriter.writeStartElement("Line");
			HObject obj = ((HLine*)auxL)->obj;
            xmlWriter.writeAttribute("x1", QString::number(auxL->line().x1()));
            xmlWriter.writeAttribute("y1", QString::number(auxL->line().y1()));
            xmlWriter.writeAttribute("x2", QString::number(auxL->line().x2()));
            xmlWriter.writeAttribute("y2", QString::number(auxL->line().y2()));
            xmlWriter.writeAttribute("friction", QString::number(obj.getFriction()));
            xmlWriter.writeAttribute("collisionMask", QString::number(obj.getColMask()));
            xmlWriter.writeAttribute("frictionMask", QString::number(obj.getFrictionMask()));
            xmlWriter.writeEndElement();
		}

	}
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
	file.close();
}

void CfrmHenoc::Open(){
	QString fileName = QFileDialog::getOpenFileName(this, tr("Abrir XKI"),
				QDir::currentPath() + "/demo01.xki",
				tr("eXtended sKetch sImulation (*.xki)"));

	if (fileName.isEmpty()) return;

	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("XKI"),
			tr("No se pudo leer %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return;
	}
	
    QXmlStreamReader xmlReader(&file);

	//Limpia la escena
	for (QGraphicsItem *item : scene->items()) {
		scene->removeItem(item);
	}

    if (xmlReader.readNextStartElement()) {
        // Safe drawing area inside ENMARCA borders (use current frame thickness)
        const QRectF sr = scene->sceneRect();
        int margin = 1;
        if (m_enmarcaEnabled && !m_enmarcaLines.isEmpty()) {
            qreal w = 0.0;
            for (HLine *ln : m_enmarcaLines) {
                if (!ln) continue;
                qreal lw = ln->pen().widthF();
                if (lw > w) w = lw;
            }
            margin = (int)std::ceil(w);
            if (margin < 0) margin = 0;
        }
        const int safeLeft = (int)std::ceil(sr.left()) + margin;
        const int safeTop = (int)std::ceil(sr.top()) + margin;
        const int safeRight = (int)std::floor(sr.right()) - margin;
        const int safeBottom = (int)std::floor(sr.bottom()) - margin;

        auto clampInt = [](int v, int lo, int hi){ return v < lo ? lo : (v > hi ? hi : v); };
        auto clampRectTopLeft = [&](int &x, int &y, int w, int h){
            const int maxX = std::max(safeLeft, safeRight - std::max(0, w));
            const int maxY = std::max(safeTop, safeBottom - std::max(0, h));
            x = clampInt(x, safeLeft, maxX);
            y = clampInt(y, safeTop, maxY);
        };
        auto clampSize = [&](int &w, int &h){
            if (w > (safeRight - safeLeft)) w = std::max(1, safeRight - safeLeft);
            if (h > (safeBottom - safeTop)) h = std::max(1, safeBottom - safeTop);
        };
        auto clampPoint = [&](int &x, int &y){
            x = clampInt(x, safeLeft, safeRight);
            y = clampInt(y, safeTop, safeBottom);
        };
        if (xmlReader.name().toString() == "Sketch"){
            while(xmlReader.readNextStartElement()){
                if(xmlReader.name().toString() == "Ball"){
                    int x = xmlReader.attributes().value(QStringLiteral("x")).toInt();
                    int y = xmlReader.attributes().value(QStringLiteral("y")).toInt();
                    int radius = xmlReader.attributes().value(QStringLiteral("radius")).toInt();
                    float mass = xmlReader.attributes().value(QStringLiteral("mass")).toFloat();
                    float friction = xmlReader.attributes().value(QStringLiteral("friction")).toFloat();
                    float bounceFactor = xmlReader.attributes().value(QStringLiteral("bounceFactor")).toFloat();
                    float bounceVelocity = xmlReader.attributes().value(QStringLiteral("bounceVelocity")).toFloat();
                    int colMask = xmlReader.attributes().value(QStringLiteral("collisionMask")).toInt();
                    int frictionMask = xmlReader.attributes().value(QStringLiteral("frictionMask")).toInt();
                    int rotation = xmlReader.attributes().value(QStringLiteral("rotation")).toInt();

                    // Ensure ball fits inside safe rect
                    int w = radius;
                    int h = radius;
                    clampSize(w, h);
                    clampRectTopLeft(x, y, w, h);

                    HBall *item = new HBall(x, y, w, h);
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                    scene->addItem(item);
                    item->obj.setType(2);
                    item->obj.setWidth(w);
                    item->obj.setHeight(h);
                    item->obj.setMass(mass);
                    item->obj.setFriction(friction);
                    item->obj.setBounceFactor(bounceFactor);
                    item->obj.setBounceVelocity(bounceVelocity);
                    item->obj.setColMask(colMask);
                    item->obj.setFrictionMask(frictionMask);
                    item->obj.setRotation(rotation);
                    float tColor = (60.0f + (rand() % 90) - 45);
                    item->obj.setColor(tColor);
                    ((QGraphicsEllipseItem*)(item))->setPen( QPen( fromThetaColor(tColor) )  );
                    ((QGraphicsEllipseItem*)(item))->setBrush(fromThetaIncreasedColor(tColor, 0.5));
                } else if(xmlReader.name().toString() == "Box"){
                    int x = xmlReader.attributes().value(QStringLiteral("x")).toInt();
                    int y = xmlReader.attributes().value(QStringLiteral("y")).toInt();
                    int width = xmlReader.attributes().value(QStringLiteral("width")).toInt();
                    int height = xmlReader.attributes().value(QStringLiteral("height")).toInt();
                    float mass = xmlReader.attributes().value(QStringLiteral("mass")).toFloat();
                    float friction = xmlReader.attributes().value(QStringLiteral("friction")).toFloat();
                    float bounceFactor = xmlReader.attributes().value(QStringLiteral("bounceFactor")).toFloat();
                    float bounceVelocity = xmlReader.attributes().value(QStringLiteral("bounceVelocity")).toFloat();
                    int colMask = xmlReader.attributes().value(QStringLiteral("collisionMask")).toInt();
                    int frictionMask = xmlReader.attributes().value(QStringLiteral("frictionMask")).toInt();
                    int rotation = xmlReader.attributes().value(QStringLiteral("rotation")).toInt();

                    // Ensure box fits inside safe rect
                    clampSize(width, height);
                    clampRectTopLeft(x, y, width, height);

                    HBox *item = new HBox(10, 10, 20, 20);
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                    item->obj.setType(1);
                    item->obj.setWidth(width);
                    item->obj.setHeight(height);
                    item->obj.setMass(mass);
                    item->obj.setFriction(friction);
                    item->obj.setBounceFactor(bounceFactor);
                    item->obj.setBounceVelocity(bounceVelocity);
                    item->obj.setColMask(colMask);
                    item->obj.setFrictionMask(frictionMask);
                    item->obj.setRotation(rotation);
                    float tColor = (60.0f + (rand() % 90) - 45);

                    QRectF dao(x, y, width, height);
                    
                    item->obj.setColor(tColor);

                    ((QGraphicsRectItem*)(item))->setPen( QPen( fromThetaColor(tColor) )  );
                    ((QGraphicsRectItem*)(item))->setBrush(fromThetaIncreasedColor(tColor, 0.5));
                    ((QGraphicsRectItem*)(item))->setFlag(QGraphicsItem::ItemIsSelectable, true);
                    ((QGraphicsRectItem*)(item))->setRect(dao);
                    ((QGraphicsRectItem*)item)->setRotation(item->obj.getRotation());
                    scene->addItem(((QGraphicsRectItem*)item));

                } else if(xmlReader.name().toString() == "Line"){
                    int x1 = xmlReader.attributes().value(QStringLiteral("x1")).toInt();
                    int y1 = xmlReader.attributes().value(QStringLiteral("y1")).toInt();
                    int x2 = xmlReader.attributes().value(QStringLiteral("x2")).toInt();
                    int y2 = xmlReader.attributes().value(QStringLiteral("y2")).toInt();
                    float friction = xmlReader.attributes().value(QStringLiteral("friction")).toFloat();
                    int colMask = xmlReader.attributes().value(QStringLiteral("collisionMask")).toInt();
                    int frictionMask = xmlReader.attributes().value(QStringLiteral("frictionMask")).toInt();

                    // Clamp endpoints into safe rect
                    clampPoint(x1, y1);
                    clampPoint(x2, y2);
                    QLineF ml((qreal)x1, (qreal)y1, (qreal)x2, (qreal)y2);
                    HLine *item = new HLine(ml);
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                    float tColor = (60.0f + (rand() % 90) - 45);
                    item->obj.setColor( tColor );
                    ((QGraphicsLineItem*)item)->setPen(QPen(fromThetaColor(tColor), myWorldProp.lineThicknessPx));
                    scene->addItem(item);
                    item->obj.setType(3);
                    item->obj.setFriction(friction);
                    item->obj.setColMask(colMask);
                    item->obj.setFrictionMask(frictionMask);
                }
                xmlReader.skipCurrentElement();
            }
        }
    }
    if (xmlReader.hasError()) {
        QMessageBox::information(this, tr("XKI"),
                                 tr("Error de parser: %1").arg(xmlReader.errorString()));
    }

	file.close();
}
