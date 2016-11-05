#ifndef DRAWING_H
#define DRAWING_H

#include <QGraphicsItem>
//#include <QGraphicsObject>
#include <drawitem.h>
#include <drawdata.h>
/*
#include <QPainter>
#include <QRectF>
#include <QString>
#include <QDebug>
#include <QGraphicsView>*/
//class drawing : public QGraphicsItem
//class drawing : public QGraphicsObject//QGraphicsItem

/// base class for drawing area painting, virtual class
/**
    virtual:\n
        setData();\n
        paint():\n
 
    the size of drawing is the viewport() exclude the four ruler label.\n
    that is the size of drawing area we can acturally see.\n

*/
class drawing : public drawItem
{
public:
    drawing();
    QRectF getRect();

/// pure virtual: set drawData object
    virtual void setData(drawData *d) =0;
    drawData *data;
    protected:
    QRectF boundingRect(void)const;
    QPainterPath shape()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget);
    /// outlook at the low right cornal in the drawing;
    /// it is automaticly appear wh the scene 10 times of drawing size;
    void plotOutlook(QPainter *p);

};

#endif // DRAWING_H
