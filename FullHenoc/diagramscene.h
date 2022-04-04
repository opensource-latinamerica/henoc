#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>

class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;

class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem };

    DiagramScene(QMenu *itemMenu, QObject *parent = 0);
    QColor itemColor() const
        { return myItemColor; }
    QColor lineColor() const
        { return myLineColor; }
    void setLineColor(const QColor &color);
    void setItemColor(const QColor &color);

public slots:
	void setMode(Mode mode);
	void setItemType(int);

signals:
    void itemInserted(QGraphicsItem *item);
//    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
	bool isItemChange(int type);
	int myItemType;
	QMenu *myItemMenu;
	QGraphicsItem *itemx;
	Mode myMode;
	bool leftButtonDown;
	QPointF startPoint;
	//QGraphicsLineItem *line;
	QColor myItemColor;
	QColor myLineColor;
};

QColor fromThetaIncreasedColor(float theta, float factor);
QColor fromThetaColor(float theta);

#endif
