#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
class QGraphicsSceneContextMenuEvent;
class QMenu;
class QGraphicsLineItem;

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
    void setDefaultLineWidth(qreal w) { m_defaultLineWidth = w; }

public slots:
	void setMode(Mode mode);
	void setItemType(int);

signals:
    void itemInserted(QGraphicsItem *item);
//    void itemSelected(QGraphicsItem *item);
    void lineThicknessChanged(qreal width);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

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
    qreal m_defaultLineWidth = 2.0;
};

QColor fromThetaIncreasedColor(float theta, float factor);
QColor fromThetaColor(float theta);

#endif
