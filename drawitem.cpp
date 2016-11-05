#include "drawitem.h"


drawItem::drawItem(QGraphicsItem *p)
    :QGraphicsItem(p)
{
    // make it draw in bounding rect ,cannot draw outside:
    init();
}
void drawItem::init()
{
    setFlag(ItemClipsToShape);
    //m_sx = 1;
    //m_sy = 1;
}


void drawItem::changed()
{
    prepareGeometryChange();
}

QGraphicsView* drawItem::view()
{
    QList<QGraphicsView *> list;
    list = scene()->views();
    return list[0];
}
/*
void drawItem::setScaleXY(qreal xx,qreal yy)
{
    m_sx = xx;
    m_sy = yy;
    //changed();
}*/
QPointF drawItem::map2Scene(qreal x,qreal y)
{
    QPointF f;
    f = deviceTransform(view()->viewportTransform()).map(QPointF(x,y));//viewport transform
    QPoint pp;
    pp = QPoint(f.x(),f.y());
    return view()->mapToScene(pp);

}
QPointF drawItem::map2SceneWide(qreal x,qreal y)
{
    QPointF f,f1,ppf,pp1f;
    f = deviceTransform(view()->viewportTransform()).map(QPointF(x,y));//viewport transform
    f1 = deviceTransform(view()->viewportTransform()).map(QPointF(0,0));//viewport transform

    QPoint pp,pp1;
    pp = QPoint(f.x(),f.y());
    ppf = view()->mapToScene(pp);

    pp1 = QPoint(f1.x(),f1.y());
    pp1f = view()->mapToScene(pp1);

    f.setX(ppf.x()-pp1f.x());
    f.setY(ppf.y()-pp1f.y());
    return f;
     
}
QRectF drawItem::map2Scene(QRectF rcf)
{
    QRectF f;
    f = deviceTransform(view()->viewportTransform()).mapRect(QRectF(rcf));//viewport transform
    QRect r;
    r = QRect(f.x(),f.y(),f.width(),f.height());
    QPolygonF p;
    p = view()->mapToScene(r);
    return p.boundingRect();

}
QPointF drawItem::scene2Item(qreal x,qreal y)
{
    QPoint pp;
    QPointF f;
    pp = view()->mapFromScene(x,y);
    f = deviceTransform(view()->viewportTransform()).inverted().map(pp);//viewport transform
    return f;

}
QPointF drawItem::scene2ItemWide(qreal x,qreal y)
{
    QPoint pp,pp1;
    QPointF f,f1;
    pp = view()->mapFromScene(x,y);
    pp1 = view()->mapFromScene(0,0);
 
    f = deviceTransform(view()->viewportTransform()).inverted().map(pp);//viewport transform
    f1 = deviceTransform(view()->viewportTransform()).inverted().map(pp1);//viewport transform


    f.setX(f.x() - f1.x());
    f.setY(f.y() - f1.y());

    return f;

}



