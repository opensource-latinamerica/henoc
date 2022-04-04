#include <QtGui>

#include "diagramscene.h"
#include "HObject.h"
#include "HBall.h"
#include "HBox.h"
#include "HLine.h"

#include<iostream>

#include<math.h>

using namespace std;

DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent): QGraphicsScene(parent){
	myItemMenu = itemMenu;
	myMode = MoveItem;
	myItemType = 0;
	itemx = 0;
	myItemColor = Qt::white;
	myLineColor = Qt::black;
}

void DiagramScene::setLineColor(const QColor &color)
{
}

QColor fromThetaIncreasedColor(float theta, float factor){
	float r,g,b;
	r = g = b = 0;
	if (theta < 0)
		theta = 360 + theta;
	if (theta >= 360)
		theta -= 360;

	if (theta < 120){
		g = theta / 120;
		r = 1 - g;
		b = 0;
	}
	else if (theta < 240){    
		b = (theta - 120) / 120;
		g = 1 - b;
		r = 0;
	}
	else{
		r = (theta - 240) / 120;
		b = 1 - r;
		g = 0;
	}
	r += factor;
	g += factor;
	b += factor;
	if (r > 1)
		r = 1;
	if (g > 1)
		g = 1;
	if (b > 1)
		b = 1;
	return(QColor(r*255,g*255,b*255));
}

QColor fromThetaColor(float theta){
	float r,g,b;
	r = g = b = 0;
	if (theta < 0)
		theta = 360 + theta;
	if (theta >= 360)
		theta -= 360;

	if (theta < 120){
		g = theta / 120;
		r = 1 - g;
		b = 0;
	}
	else if (theta < 240){    
		b = (theta - 120) / 120;
		g = 1 - b;
		r = 0;
	}
	else{
		r = (theta - 240) / 120;
		b = 1 - r;
		g = 0;
	}
	return(QColor(r*255,g*255,b*255));
}

void DiagramScene::setItemColor(const QColor &color)
{
/*    myItemColor = color;
    if (isItemChange(DiagramItem::Type)) {
        DiagramItem *item =
            qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
        item->setBrush(myItemColor);
    }
*/
}

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void DiagramScene::setItemType(int type)
{
    myItemType = type;
}


void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
	if (mouseEvent->button() != Qt::LeftButton)
		return;

	QGraphicsItem *item;
	switch (myMode){
		case InsertItem:
			if(itemx != 0) break;
			if(0 == myItemType ) break;
			if(1 == myItemType ){
				item = new HBox(10,10,20,20);
				((HBox *)item)->obj.setType(1);
				float tColor = (60.0f + (rand() % 90) - 45);
				((HBox *)item)->obj.setColor( tColor );
				((QGraphicsRectItem*)(item))->setPen( QPen( fromThetaColor(tColor) )  );
				((QGraphicsRectItem*)(item))->setBrush(fromThetaIncreasedColor(tColor, 0.5));
				((QGraphicsRectItem*)(item))->resetMatrix();
			}
			if(2 == myItemType ){
				item = new HBall(10,10,20,20);
				((HBall *)item)->obj.setType(2);
				float tColor = (60.0f + (rand() % 90) - 45);
				((HBall *)item)->obj.setColor( tColor );
				((QGraphicsEllipseItem*)(item))->setPen( QPen( fromThetaColor(tColor) )  );
				((QGraphicsEllipseItem*)(item))->setBrush(fromThetaIncreasedColor(tColor, 0.5));
				//item = new QGraphicsEllipseItem(10,10,20,20);
			}
			if(3 == myItemType ){
				item = new HLine(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
				((HLine *)item)->obj.setType(3);
				item->setFlag(QGraphicsItem::ItemIsSelectable, true);
				float tColor = (60.0f + (rand() % 90) - 45);
				((HLine *)item)->obj.setColor( tColor );
				((QGraphicsLineItem*)item)->setPen(QPen(fromThetaColor(tColor), 2));
				addItem(item);
				itemx = item;
				break;
			}
			//item->setBrush(myItemColor);
			//item->setFlag(QGraphicsItem::ItemIsMovable, true);
			item->setFlag(QGraphicsItem::ItemIsSelectable, true);
			addItem(item);
			//item->setPos( mouseEvent->scenePos() );
			item->setPos(mouseEvent->scenePos() -= QPointF(25,25));
			itemx = item;
			//emit itemInserted(item);
            	break;
		default:
			;
	}
	QGraphicsScene::mousePressEvent(mouseEvent);
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
	if(myMode == InsertItem && itemx != 0){
		int xa = views().at(0)->mapFromScene( itemx->mapToScene( itemx->boundingRect() )).boundingRect().x();  
		int ya = views().at(0)->mapFromScene( itemx->mapToScene( itemx->boundingRect() )).boundingRect().y(); 
		int wa = views().at(0)->mapFromScene( itemx->mapToScene( itemx->boundingRect() )).boundingRect().width(); 
		int ha = views().at(0)->mapFromScene( itemx->mapToScene( itemx->boundingRect() )).boundingRect().height();
		QRectF fao;
		if(1 == myItemType ){
			fao = ((QGraphicsRectItem*)itemx)->rect();
			fao.setHeight( fao.height() + (mouseEvent->scenePos().y() - mouseEvent->lastScenePos().y()) );
			fao.setWidth( fao.width() + (mouseEvent->scenePos().x() - mouseEvent->lastScenePos().x())  );
		}
		if(2 == myItemType ){
			fao = ((QGraphicsEllipseItem*)itemx)->rect();
			qreal fx = (mouseEvent->scenePos().x() - mouseEvent->lastScenePos().x());
			qreal fy = (mouseEvent->scenePos().y() - mouseEvent->lastScenePos().y());
			fao.setHeight( fao.height() + ((fx+fy)/2) );
			fao.setWidth( fao.width() + ((fx+fy)/2)  );
		}
		if(1 == myItemType )
			((QGraphicsRectItem*)itemx)->setRect(fao);
		if(2 == myItemType )
			((QGraphicsEllipseItem*)itemx)->setRect(fao);

		if (3 == myItemType) {
			QLineF newLine(((QGraphicsLineItem*)itemx)->line().p1(), mouseEvent->scenePos());
			((QGraphicsLineItem*)itemx)->setLine(newLine);
		}
	}
	else if (myMode == MoveItem) {
		QGraphicsScene::mouseMoveEvent(mouseEvent);
	}
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
	if(myMode == InsertItem && itemx != 0){
		if(3 != myItemType){
			((HBall *)itemx)->obj.setWidth(views().at(0)->mapFromScene( itemx->mapToScene( itemx->boundingRect() )).boundingRect().width());
			((HBall *)itemx)->obj.setHeight(views().at(0)->mapFromScene( itemx->mapToScene( itemx->boundingRect() )).boundingRect().height());
			((HBall *)itemx)->obj.setMass( ( (float)(itemx->boundingRect().width()+itemx->boundingRect().height())/40.0 ) );
		}
		myItemType = 0;
		emit itemInserted(itemx);
		//delete itemx;
		itemx=0;
	}

	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

bool DiagramScene::isItemChange(int type){
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}
