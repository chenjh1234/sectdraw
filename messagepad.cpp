#include "messagepad.h"

messagePad::messagePad(QGraphicsItem  *parent) :
    QGraphicsTextItem(parent)
{

}

void messagePad::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{
    QString str;
    //qreal x1,y1;
   // x1 = 1./m_sx;
    //y1 = 1./m_sy;
    //x1 = 1./parent()->view()->transform().m11();
    //y1 =  1./parent()->view()->transform().m22();

    //painter->scale(x1,y1);
    QGraphicsTextItem::paint(painter,option,widget);
    //  str = "1234567890123456789012345678901234567890";
    //str = toPlainText();
   // painter->drawText(0,16,str);//first line text
   // qDebug()<<"i am test";

}
/*
QRectF messagePad::boundingRect()const
{
       QRectF rc;
       rc =  QGraphicsTextItem::boundingRect();

      // qDebug() << rc.width() << rc.height();
       //rc.setHeight(rc.height()/m_sy);
      // rc.setWidth(rc.width()/m_sx);
       return rc;
}
*/
