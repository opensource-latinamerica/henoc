#ifndef __HBALL_H__
#define __HBALL_H__

#include <QGraphicsEllipseItem>
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


class HBall : public QGraphicsEllipseItem{
	public:
		HBall(QGraphicsItem *parent = 0);
		HBall( qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent = 0 );
		void setWidth(int);
		void setHeight(int);
		HObject obj;
	private:
};

#endif
