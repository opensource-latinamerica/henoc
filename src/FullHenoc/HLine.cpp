#include <QtGui>
#include "HLine.h"


HLine::HLine(QGraphicsItem *parent):QGraphicsLineItem(parent){
	//setFlag(QGraphicsItem::ItemIsMovable, true);
}

HLine::HLine( const QLineF & line, QGraphicsItem * parent):QGraphicsLineItem(line,parent){
	//setFlag(QGraphicsItem::ItemIsMovable, true);
}

HLine::HLine( qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent ):QGraphicsLineItem(x,y,width,height,parent){
	//setFlag(QGraphicsItem::ItemIsMovable, true);
}

void HLine::setLength(int w){
	QLineF  fao = line();
	fao.setLength(w);
	setLine(fao);
	obj.setLength(w);
}

