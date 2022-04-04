#include <QtGui>
#include "HBox.h"


HBox::HBox(QGraphicsItem *parent):QGraphicsRectItem(parent){
	setFlag(QGraphicsItem::ItemIsMovable, true);
}


HBox::HBox( qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent ):QGraphicsRectItem(x,y,width,height,parent){
	setFlag(QGraphicsItem::ItemIsMovable, true);
}

void HBox::setWidth(int w){
	QRectF  fao = rect();
	fao.setWidth(w);
	setRect(fao);
	obj.setWidth(w);
}

void HBox::setHeight(int h){
	QRectF  fao = rect();
	fao.setHeight(h);
	setRect(fao);
	obj.setHeight(h);
}


