#ifndef DRAWITEM_H
#define DRAWITEM_H

#include <QGraphicsItem>

#include <QPainter>
#include <QRectF>
#include <QString>
#include <QDebug>
#include <QGraphicsView>
//#include <QObject>
/// base class for all graphicsItems,virtual class.
/**
  It is a virtual class no:paint() boundingRect()
  the ancestor of all draw items:
  include the ruler,drawing,color bar
  */

class drawItem : public QGraphicsItem
{
    //Q_OBJECT
public:
    drawItem(QGraphicsItem  *parent = 0);
    //void setScaleXY(qreal x,qreal y);//in fact no use now
    //qreal m_sx,m_sy; //infact it is no use now
    QGraphicsView * view(); ///< get the view of the item
    void changed(); ///< process size is changed
    QPointF map2Scene(qreal x,qreal y);
    QPointF scene2Item(qreal x,qreal y);
    QRectF map2Scene(QRectF rcf);
    QPointF map2SceneWide(qreal x,qreal y);
    QPointF scene2ItemWide(qreal x,qreal y);
private:
    void init();

};

#endif // DRAWITEM_H
