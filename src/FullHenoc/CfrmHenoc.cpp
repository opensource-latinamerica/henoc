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
#include <QVBoxLayout>
#include "glviewport.h"

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
	myWorldProp.gravity = 0.5f;
	myWorldProp.erp = 0.7f;
	myWorldProp.lin = 0.25f;
	myWorldProp.co = 0;

	scene = new DiagramScene(NULL);
	scene->setSceneRect(QRectF(0, 0, 655, 517));
	view->setScene(scene);	

	connect(scene, &DiagramScene::itemInserted, this, &CfrmHenoc::itemInserted);

    // Embed an OpenGL viewport inside page_2 of the stacked widget
    glWidget = new GLViewport(stackedWidget->widget(1));
    QWidget *page2 = stackedWidget->widget(1);
    if (!page2->layout()) {
        auto *v = new QVBoxLayout(page2);
        v->setContentsMargins(0, 0, 0, 0);
    }
    page2->layout()->addWidget(glWidget);

    timer = new QTimer(this);
    // Trigger repaints while playing
    connect(timer, &QTimer::timeout, glWidget, QOverload<>::of(&QOpenGLWidget::update));
}	

void CfrmHenoc::Play(){
    QList<QGraphicsItem*> lista = scene->items();
    int xa, ya, wa, ha;
    //glWidget->delSpace();
    // Reset GL items and configure simple world params
    static_cast<GLViewport*>(glWidget)->clearObjects();
    static_cast<GLViewport*>(glWidget)->setGravity(myWorldProp.gravity);
	for(unsigned int i=0; i < lista.size(); i++){
		lista.at(i);
		xa = view->mapFromScene( lista.at(i)->mapToScene( lista.at(i)->boundingRect() )).boundingRect().x();  
		ya = view->mapFromScene( lista.at(i)->mapToScene( lista.at(i)->boundingRect() )).boundingRect().y(); 
		wa = view->mapFromScene( lista.at(i)->mapToScene( lista.at(i)->boundingRect() )).boundingRect().width(); 
		ha = view->mapFromScene( lista.at(i)->mapToScene( lista.at(i)->boundingRect() )).boundingRect().height();
		
		QGraphicsRectItem * auxR = qgraphicsitem_cast<QGraphicsRectItem*>(lista.at(i));		
		QGraphicsEllipseItem * auxE = qgraphicsitem_cast<QGraphicsEllipseItem*>(lista.at(i));		
		QGraphicsLineItem * auxL = qgraphicsitem_cast<QGraphicsLineItem*>(lista.at(i));		

		QRect fao(xa,ya,wa,ha);
			
		if(0 != auxR){
			HObject obj = ((HBox*)auxR)->obj;
			const QColor stroke = fromThetaColor(obj.getColor());
			const QColor fill = fromThetaIncreasedColor(obj.getColor(), 0.5f);
			static_cast<GLViewport*>(glWidget)->addRectangle(xa, ya, wa, ha, obj.getRotation(), fill, stroke, true);
		}
		else if(0 != auxE){
			div_t q;
			q = div(ha,2);
			HObject obj = ((HBall*)auxE)->obj;
			const QColor stroke = fromThetaColor(obj.getColor());
			const QColor fill = fromThetaIncreasedColor(obj.getColor(), 0.5f);
			static_cast<GLViewport*>(glWidget)->addCircle(fao.center().x(), fao.center().y(), q.quot, fill, stroke, true);
		}
        else if(0 != auxL){
            HObject obj = ((HLine*)auxL)->obj;
            const QColor stroke = fromThetaColor(obj.getColor());
            const QPoint p1 = view->mapFromScene(auxL->line().p1());
            const QPoint p2 = view->mapFromScene(auxL->line().p2());
            static_cast<GLViewport*>(glWidget)->addLine(p1.x(), p1.y(), p2.x(), p2.y(), stroke, 2.0f);
        }

	}



    // Switch to the OpenGL page which hosts the GL viewport
    stackedWidget->setCurrentIndex(1);
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
	btnBox->setChecked(true);
}

void CfrmHenoc::AddCatapult(){
	{
		QLineF ml( 0, 0, 655, 0 );
		HLine *item = new HLine(ml);
		item->setFlag(QGraphicsItem::ItemIsSelectable, false);
		float tColor = (0);
		item->obj.setColor( tColor );
		((QGraphicsLineItem*)item)->setPen(QPen(fromThetaColor(tColor), 1));
		scene->addItem(item);
		item->obj.setType(3);
		item->obj.setFriction( 99 );
		item->obj.setColMask( 0 );
		item->obj.setFrictionMask( 0 );
	}
	
	{
		QLineF ml( 655, 0,  655, 517 );
		HLine *item = new HLine(ml);
		item->setFlag(QGraphicsItem::ItemIsSelectable, false);
		float tColor = (0);
		item->obj.setColor( tColor );
		((QGraphicsLineItem*)item)->setPen(QPen(fromThetaColor(tColor), 1));
		scene->addItem(item);
		item->obj.setType(3);
		item->obj.setFriction( 99 );
		item->obj.setColMask( 0 );
		item->obj.setFrictionMask( 0 );
	}
	
	{
		QLineF ml( 0, 517, 655, 517 );
		HLine *item = new HLine(ml);
		item->setFlag(QGraphicsItem::ItemIsSelectable, false);
		float tColor = (0);
		item->obj.setColor( tColor );
		((QGraphicsLineItem*)item)->setPen(QPen(fromThetaColor(tColor), 1));
		scene->addItem(item);
		item->obj.setType(3);
		item->obj.setFriction( 99 );
		item->obj.setColMask( 0 );
		item->obj.setFrictionMask( 0 );
	}

	{
		QLineF ml( 0, 0, 0, 517 );
		HLine *item = new HLine(ml);
		item->setFlag(QGraphicsItem::ItemIsSelectable, false);
		float tColor = (0);
		item->obj.setColor( tColor );
		((QGraphicsLineItem*)item)->setPen(QPen(fromThetaColor(tColor), 1));
		scene->addItem(item);
		item->obj.setType(3);
		item->obj.setFriction( 99 );
		item->obj.setColMask( 0 );
		item->obj.setFrictionMask( 0 );
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

	CfrmPeCaLi dialog((HObject*)(&(((HBox*)lista.at(0))->obj)),this);
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

                    HBall *item = new HBall(x, y, radius, radius);
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                    scene->addItem(item);
                    item->obj.setType(2);
                    item->obj.setWidth(radius);
                    item->obj.setHeight(radius);
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

                    QLineF ml((qreal)x1, (qreal)y1, (qreal)x2, (qreal)y2);
                    HLine *item = new HLine(ml);
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                    float tColor = (60.0f + (rand() % 90) - 45);
                    item->obj.setColor( tColor );
                    ((QGraphicsLineItem*)item)->setPen(QPen(fromThetaColor(tColor), 2));
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
