#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QColor>
#include <QPen>
#include <QGraphicsView>
#include <QTransform>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QInputDialog>

#include "diagramscene.h"
#include "HObject.h"
#include "HBall.h"
#include "HBox.h"
#include "HLine.h"

#include<iostream>

#include<cmath>

using namespace std;

DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent): QGraphicsScene(parent){
	myItemMenu = itemMenu;
	myMode = MoveItem;
	myItemType = 0;
	itemx = 0;
	myItemColor = Qt::white;
    myLineColor = Qt::black;
    m_defaultLineWidth = 2.0;
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
				((QGraphicsRectItem*)(item))->setTransform(QTransform());
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
                ((QGraphicsLineItem*)item)->setPen(QPen(fromThetaColor(tColor), m_defaultLineWidth));
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
        if (3 != myItemType && !views().isEmpty()) {
            const QRect mapped = views().at(0)->mapFromScene(itemx->mapToScene(itemx->boundingRect())).boundingRect();
            const int w = mapped.width();
            const int h = mapped.height();
            const float m = static_cast<float>(itemx->boundingRect().width() + itemx->boundingRect().height()) / 40.0f;

            if (myItemType == 1) { // Box
                if (auto *box = qgraphicsitem_cast<HBox *>(itemx)) {
                    box->obj.setWidth(w);
                    box->obj.setHeight(h);
                    box->obj.setMass(m);
                }
            } else if (myItemType == 2) { // Ball
                if (auto *ball = qgraphicsitem_cast<HBall *>(itemx)) {
                    ball->obj.setWidth(w);
                    ball->obj.setHeight(h);
                    ball->obj.setMass(m);
                }
            }
        }
        myItemType = 0;
        emit itemInserted(itemx);
        //delete itemx;
        itemx=0;
    }

	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

bool DiagramScene::isItemChange(int type){
    for (QGraphicsItem *item : selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}

void DiagramScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    QGraphicsItem *clicked = itemAt(event->scenePos(), QTransform());
    if (!clicked) {
        QGraphicsScene::contextMenuEvent(event);
        return;
    }
    QGraphicsLineItem *line = qgraphicsitem_cast<QGraphicsLineItem*>(clicked);
    if (!line) {
        QGraphicsScene::contextMenuEvent(event);
        return;
    }
    QMenu menu;
    QAction *a1 = menu.addAction("Thickness: 1 px");
    QAction *a2 = menu.addAction("Thickness: 2 px");
    QAction *a4 = menu.addAction("Thickness: 4 px");
    QAction *a8 = menu.addAction("Thickness: 8 px");
    menu.addSeparator();
    QAction *acustom = menu.addAction("Custom…");
    QAction *chosen = menu.exec(event->screenPos());
    if (!chosen) return;
    qreal w = line->pen().widthF();
    if (chosen == a1) w = 1.0;
    else if (chosen == a2) w = 2.0;
    else if (chosen == a4) w = 4.0;
    else if (chosen == a8) w = 8.0;
    else if (chosen == acustom){
        bool ok=false; double val = QInputDialog::getDouble(nullptr, tr("Line Thickness"), tr("Pixels:"), w, 0.5, 64.0, 1, &ok);
        if (!ok) return; w = val;
    }
    QPen p = line->pen(); p.setWidthF(w); line->setPen(p);
    emit lineThicknessChanged(w);
}
