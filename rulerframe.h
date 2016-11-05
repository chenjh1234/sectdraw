#ifndef RULERFRAME_H
#define RULERFRAME_H

#include <QGraphicsItem>
#include "drawitem.h"
#include <drawdata.h>
#include <QGraphicsLineItem>
#define LABEL_SPACE 5
/*

#include <QPainter>
#include <QRectF>
#include <QString>
#include <QGraphicsView>
#include <QDebug>*/
//#include <drawscene.h>
/// base class for ruler panel;
/**
  the four rulers use the same class;
*/
class rulerFrame : public drawItem
{
public:
    rulerFrame(QString where);
    /// set data
    void setData(drawData *d);
    /// red line object ,for position the cursor in viewport
    QGraphicsLineItem *m_line;

private:
    QRectF boundingRect(void)const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget);


    /// "LEFT",RIGHT",¡°TOP",¡±BOTTOM"£¬ is the position of the ruler
    QString where;
    /// rent of the 4 ruler
    QRectF leftRect()const;
    QRectF rightRect()const;
    QRectF topRect()const;
    QRectF bottomRect()const;

    drawData *data;

    ///  paint numbers:
    void leftPaint(QPainter *p, const QStyleOptionGraphicsItem *op,QWidget *w);
    void rightPaint(QPainter *p, const QStyleOptionGraphicsItem *op,QWidget *w);
    void topPaint(QPainter *p, const QStyleOptionGraphicsItem *op,QWidget *w);
    void bottomPaint(QPainter *p, const QStyleOptionGraphicsItem *op,QWidget *w);
    void hPaint(QPainter *p, const QStyleOptionGraphicsItem *op,QWidget *w,int flag);
    void vPaint(QPainter *p, const QStyleOptionGraphicsItem *op,QWidget *w,int flag);
    /// the length of the luler line in dots,in short and long.
    int sline,lline;
};

#endif // RULERFRAME_H
