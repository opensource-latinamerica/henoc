#include<QtGui>
#include<QtCore>
#include<QtOpenGL>
#include<QtXml>

#include<CfrmHenoc.h>
#include<CfrmPeCaLi.h>
#include<CfrmMundo.h>
#include<diagramscene.h>

#include<iostream>
#include<HBox.h>
#include<HLine.h>
#include<HBall.h>

#include"glwidget.h"
#include<henocUniverseI.h>

using namespace HenocUniverseI;
using namespace std;

CfrmHenoc::CfrmHenoc( QWidget * parent, Qt::WFlags flags):QMainWindow(parent, flags){
	setupUi(this);
	
	connect(btnBox, SIGNAL(clicked()), this, SLOT(AddBox()));
	connect(btnLine, SIGNAL(clicked()), this, SLOT(AddLine()));
	connect(btnCatapult, SIGNAL(clicked()), this, SLOT(AddCatapult()));
	connect(btnBall, SIGNAL(clicked()), this, SLOT(AddBall()));
	connect(btnPlay, SIGNAL(clicked()), this, SLOT(Play()));
	connect(btnPropiedades, SIGNAL(clicked()), this, SLOT(changeProperties()));
	connect(btnBorrar, SIGNAL(clicked()), this, SLOT(Delete()));
	connect(bntStop, SIGNAL(clicked()), this, SLOT(Stop()));
	connect(btnSave, SIGNAL(clicked()), this, SLOT(Save()));
	connect(btnFrame, SIGNAL(clicked()), this, SLOT(Open()));

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

	connect(scene, SIGNAL(itemInserted(QGraphicsItem *)),this, SLOT(itemInserted(QGraphicsItem *)));

	QGridLayout *gridLayoutx;
	gridLayoutx = new QGridLayout(page_2);
	glWidget = new GLWidget(page_2);
	gridLayoutx->addWidget(glWidget, 0, 0, 1 ,1);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), glWidget, SLOT(animate()));
}	

void CfrmHenoc::Play(){
	QList<QGraphicsItem*> lista = scene->items();
	int xa, ya, wa, ha;
	glWidget->delSpace();
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
			glWidget->addBox(fao.center().x(), fao.center().y(), obj.width(), obj.height(), fabsf(obj.getMass()), obj.getFriction(), obj.getBounceFactor(), obj.getBounceVelocity(), obj.getColMask(), obj.getFrictionMask(), obj.getRotation(), obj.getColor());
		}
		else if(0 != auxE){
			div_t q;
			q = div(ha,2);
			HObject obj = ((HBall*)auxE)->obj;
			glWidget->addBall(fao.center().x(), fao.center().y(), q.quot, fabsf(obj.getMass()), obj.getFriction(), obj.getBounceFactor(), obj.getBounceVelocity(), obj.getColMask(), obj.getFrictionMask(), obj.getRotation(), obj.getColor());
		}
		else if(0 != auxL){
			HObject obj = ((HLine*)auxL)->obj;
			glWidget->addLine((int)auxL->line().x1(), (int)auxL->line().y1(), (int)auxL->line().x2(), (int)auxL->line().y2(), obj.getFriction(), obj.getColMask(), obj.getFrictionMask(), obj.getColor() );
			//glWidget->addLine(xa, ya, xa+wa, ya+ha);
		}

	}



	stackedWidget->setCurrentIndex(1);
	glWidget->setWorldParams(myWorldProp);		
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
			((QGraphicsRectItem*)lista.at(0))->resetMatrix();
			QMatrix xx = ((QGraphicsRectItem*)lista.at(0))->matrix( );
			((QGraphicsRectItem*)lista.at(0))->moveBy(-((QGraphicsRectItem*)lista.at(0))->rect().width()/2,-((QGraphicsRectItem*)lista.at(0))->rect().height()/2);
			((QGraphicsRectItem*)lista.at(0))->rotate( (((HBox*)lista.at(0))->obj.getRotation()) );
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
	foreach (QGraphicsItem *item, scene->selectedItems()) {
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
        
	QDomDocument doc("XKI");
	QDomElement root = doc.createElement("Sketch");
	doc.appendChild(root);

	QList<QGraphicsItem*> lista = scene->items();
	int xa, ya, wa, ha;
	glWidget->delSpace();
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
			QDomElement tag = doc.createElement("Box");
			HObject obj = ((HBox*)auxR)->obj;
			XkiAddBox(tag, xa, ya, obj.width(), obj.height(), 
				obj.getMass(), obj.getFriction(), obj.getBounceFactor(), 
				obj.getBounceVelocity(), obj.getColMask(), 
				obj.getFrictionMask(), obj.getRotation());
			root.appendChild(tag);
		}
		else if(0 != auxE){
			div_t q;
			q = div(ha,2);
			HObject obj = ((HBall*)auxE)->obj;
			QDomElement tag = doc.createElement("Ball");
			XkiAddBall(tag, xa, ya, wa, 
				obj.getMass(), obj.getFriction(), obj.getBounceFactor(), 
				obj.getBounceVelocity(), obj.getColMask(), 
				obj.getFrictionMask(), obj.getRotation());
			root.appendChild(tag);
		}
		else if(0 != auxL){
			QDomElement tag = doc.createElement("Line");
			HObject obj = ((HLine*)auxL)->obj;
			XkiAddLine(tag, (int)auxL->line().x1(), (int)auxL->line().y1(), 
				(int)auxL->line().x2(), (int)auxL->line().y2(), 
				obj.getFriction(), obj.getColMask(), obj.getFrictionMask() );
			root.appendChild(tag);
		}

	}

	QIODevice *device = &file;
	QTextStream out(device);
	doc.save(out, 4);
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
	
	QIODevice *device = &file;
	QString errorStr;
	int errorLine;
	int errorColumn;
	QDomDocument domDocument;
	
	if (!domDocument.setContent(device, true, &errorStr, &errorLine, &errorColumn)) {
		QMessageBox::information(window(), tr("XKI"),
			tr("Error de parser en linea %1, columna %2:\n%3")
			.arg(errorLine)
			.arg(errorColumn)
			.arg(errorStr));
		return;
	}

	
	//Limpia la escena
	foreach (QGraphicsItem *item, scene->items()) {
		scene->removeItem(item);
	}

	QDomElement root = domDocument.documentElement();

	QDomNodeList balls = root.elementsByTagName("Ball");	
	QDomNodeList lines = root.elementsByTagName("Line");	
	QDomNodeList boxes = root.elementsByTagName("Box");

	int i = 0;

	for(i = 0; i < balls.count(); i++){
		QDomElement e = balls.at(i).toElement();
		HBall *item = new HBall( e.attribute("x").toInt(), e.attribute("y").toInt(),
					e.attribute("radius").toInt(), e.attribute("radius").toInt() );
		item->setFlag(QGraphicsItem::ItemIsSelectable, true);
		scene->addItem(item);
		item->obj.setType(2);
		item->obj.setWidth( e.attribute("radius").toInt() );
		item->obj.setHeight( e.attribute("radius").toInt() );
		item->obj.setMass( e.attribute("mass").toFloat() );
		item->obj.setFriction( e.attribute("friction").toFloat() );
		item->obj.setBounceFactor( e.attribute("bounceFactor").toFloat() );
		item->obj.setBounceVelocity( e.attribute("bounceVelocity").toFloat() );
		item->obj.setColMask( e.attribute("collisionMask").toFloat() );
		item->obj.setFrictionMask( e.attribute("frictionMask").toFloat() );
		item->obj.setRotation( e.attribute("rotation").toFloat() );
		float tColor = (60.0f + (rand() % 90) - 45);
		item->obj.setColor( tColor );
		((QGraphicsEllipseItem*)(item))->setPen( QPen( fromThetaColor(tColor) )  );
		((QGraphicsEllipseItem*)(item))->setBrush(fromThetaIncreasedColor(tColor, 0.5));
	}

	for(i = 0; i < boxes.size(); i++){
		QDomElement e = boxes.at(i).toElement();
		//HBox *item = new HBox( e.attribute("x").toInt(), e.attribute("y").toInt(),
		//			e.attribute("width").toInt(), e.attribute("height").toInt() );
		
		HBox *item = new HBox( 10, 10,
					20, 20 );
		item->setFlag(QGraphicsItem::ItemIsSelectable, true);
		item->obj.setType(1);
		item->obj.setWidth( e.attribute("width").toInt() );
		item->obj.setHeight( e.attribute("height").toInt() );
		item->obj.setMass( e.attribute("mass").toFloat() );
		item->obj.setFriction( e.attribute("friction").toFloat() );
		item->obj.setBounceFactor( e.attribute("bounceFactor").toFloat() );
		item->obj.setBounceVelocity( e.attribute("bounceVelocity").toFloat() );
		item->obj.setColMask( e.attribute("collisionMask").toFloat() );
		item->obj.setFrictionMask( e.attribute("frictionMask").toFloat() );
		item->obj.setRotation( e.attribute("rotation").toFloat() );
		float tColor = (60.0f + (rand() % 90) - 45);

		QRectF dao(e.attribute("x").toInt(), e.attribute("y").toInt(),
                                      e.attribute("width").toInt(), e.attribute("height").toInt() );
		
		item->obj.setColor( tColor );

		((QGraphicsRectItem*)(item))->setPen( QPen( fromThetaColor(tColor) )  );
		((QGraphicsRectItem*)(item))->setBrush(fromThetaIncreasedColor(tColor, 0.5));
		((QGraphicsRectItem*)(item))->setFlag(QGraphicsItem::ItemIsSelectable, true);
		((QGraphicsRectItem*)(item))->setRect( dao);
	//	((QGraphicsRectItem*)item)->moveBy(-((QGraphicsRectItem*)item)->rect().width()/2,-((QGraphicsRectItem*)item)->rect().height()/2);
		((QGraphicsRectItem*)item)->rotate( (((HBox*)item)->obj.getRotation()) );
	//	((QGraphicsRectItem*)item)->moveBy(((QGraphicsRectItem*)item)->rect().width()/2,((QGraphicsRectItem*)item)->rect().height()/2);
		scene->addItem(((QGraphicsRectItem*)item));

		QRectF fao = ((QGraphicsRectItem*)item)->rect();
		fao.setHeight( item->obj.height() );
		fao.setWidth( item->obj.width() );

		int auxx=0, auxy=0, xao, yao;
		for(xao=0; xao < 1600; xao++){
			auxx = (((QGraphicsRectItem*)(item))->mapToScene( QPointF(xao,10) )).x();
			if(auxx == e.attribute("x").toInt()){
				break;
			}
		}
		for(yao=0; yao < 1600; yao++){
			auxy = (((QGraphicsRectItem*)(item))->mapToScene( QPointF(10,yao) )).y();
			if(auxy == e.attribute("y").toInt()){
				break;
			}
		}

		float ix = (((QGraphicsRectItem*)(item))->mapToScene( ((QGraphicsRectItem*)(item))->boundingRect() )).boundingRect().x();  
		float iy = (((QGraphicsRectItem*)(item))->mapToScene( ((QGraphicsRectItem*)(item))->boundingRect()  )).boundingRect().y(); 
		
		float dx = auxx - ((auxx-e.attribute("x").toInt())+ix);
		float dy = auxy - ((auxy-e.attribute("y").toInt())+iy);
		((QGraphicsRectItem*)item)->setPos(dx,dy);
			
	}

	for(i = 0; i < lines.size(); i++){
		QDomElement e = lines.at(i).toElement();
		QLineF ml( (qreal)e.attribute("x1").toInt(), (qreal)e.attribute("y1").toInt(),
			(qreal)e.attribute("x2").toInt(), (qreal)e.attribute("y2").toInt() );
		HLine *item = new HLine(ml);
		item->setFlag(QGraphicsItem::ItemIsSelectable, true);
		float tColor = (60.0f + (rand() % 90) - 45);
		item->obj.setColor( tColor );
		((QGraphicsLineItem*)item)->setPen(QPen(fromThetaColor(tColor), 2));
		scene->addItem(item);
		item->obj.setType(3);
		item->obj.setFriction( e.attribute("friction").toFloat() );
		item->obj.setColMask( e.attribute("collisionMask").toFloat() );
		item->obj.setFrictionMask( e.attribute("frictionMask").toFloat() );
	}
	file.close();
	//QLineF newLine(((QGraphicsLineItem*)itemx)->line().p1(), mouseEvent->scenePos());

}

void CfrmHenoc::XkiAddLine(QDomElement &element, int x1, int y1, int x2, int y2, float friction, int colMask, int frictionMask){
	element.setAttribute("x1",x1);
	element.setAttribute("x2",x2);
	element.setAttribute("y1",y1);
	element.setAttribute("y2",y2);
	element.setAttribute("friction",friction);
	element.setAttribute("collisionMask",colMask);
	element.setAttribute("frictionMask",frictionMask);
}

void CfrmHenoc::XkiAddBox(QDomElement &element, int x, int y, int w, int h, 
			float mass, float friction, float bounceFactor, float bounceVelocity,
			 int colMask, int frictionMask, int rotation){

	element.setAttribute("x",x);
	element.setAttribute("y",y);
	element.setAttribute("width",w);
	element.setAttribute("height",h);
	element.setAttribute("mass",mass);
	element.setAttribute("friction",friction);
	element.setAttribute("bounceFactor",bounceFactor);
	element.setAttribute("bounceVelocity",bounceVelocity);
	element.setAttribute("collisionMask",colMask);
	element.setAttribute("frictionMask",frictionMask);
	element.setAttribute("rotation",rotation);
}

void CfrmHenoc::XkiAddBall(QDomElement &element, int x, int y, int r, 
		float mass, float friction, float bounceFactor, 
		float bounceVelocity, int colMask, int frictionMask, int rotation){
	
	element.setAttribute("x",x);
	element.setAttribute("y",y);
	element.setAttribute("radius",r);
	element.setAttribute("mass",mass);
	element.setAttribute("friction",friction);
	element.setAttribute("bounceFactor",bounceFactor);
	element.setAttribute("bounceVelocity",bounceVelocity);
	element.setAttribute("collisionMask",colMask);
	element.setAttribute("frictionMask",frictionMask);
	element.setAttribute("rotation",rotation);

}
