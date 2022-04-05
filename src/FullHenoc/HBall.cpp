#include <QtGui>

#include "HBall.h"


HBall::HBall(QGraphicsItem *parent):QGraphicsEllipseItem(parent){
	setFlag(QGraphicsItem::ItemIsMovable, true);
}


HBall::HBall( qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent ):QGraphicsEllipseItem(x,y,width,height,parent){
	setFlag(QGraphicsItem::ItemIsMovable, true);
}

void HBall::setWidth(int w){
	QRectF  fao = rect();
	fao.setWidth(w);
	setRect(fao);
	obj.setWidth(w);
}

void HBall::setHeight(int h){
	QRectF  fao = rect();
	fao.setHeight(h);
	setRect(fao);
	obj.setHeight(h);
}

