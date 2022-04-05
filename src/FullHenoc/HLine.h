#ifndef __HLINE_H__
#define __HLINE_H__

#include <QGraphicsLineItem>
#include <QList>
#include "HObject.h"

class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;


class HLine : public QGraphicsLineItem{
	public:
		HLine(QGraphicsItem *parent = 0);
		HLine( qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent = 0 );
		HLine( const QLineF & line, QGraphicsItem * parent = 0 );
		void setLength(int);
		HObject obj;
	private:
};

#endif
