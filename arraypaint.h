#ifndef ARRAYPAINT_H
#define ARRAYPAINT_H
#include <drawing.h>
#include <arraydata.h>
#include <QGraphicsSceneMouseEvent>
/// paint the Array data
/**
   implement the paint number for arrayDraw;
  */
class arrayPaint : public drawing
{
public:
    arrayPaint();
    void setData(drawData *d);
protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *e);

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget);
    void paintP(QPainter *painter, qreal x1,qreal y1,qreal x2,qreal y2);
    arrayData *data;

};

#endif // ARRAYPAINT_H
