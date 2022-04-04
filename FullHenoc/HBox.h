#ifndef __HBOX_H__
#define __HBOX_H__

#include <QGraphicsRectItem>
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


class HBox : public QGraphicsRectItem{
	public:
		HBox(QGraphicsItem *parent = 0);
		HBox( qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent = 0 );
		void setWidth(int);
		void setHeight(int);
		HObject obj;
	private:
};

#endif
