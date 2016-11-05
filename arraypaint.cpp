#include "arraypaint.h"

arrayPaint::arrayPaint() : drawing()
{
}


void arrayPaint::paint(QPainter *p, const QStyleOptionGraphicsItem *op,
                       QWidget *ww)
{
   int i, ibit;
   QStyleOptionGraphicsItem *op1;
   QWidget *ww1;
   ww1 = ww;
   op1  = (QStyleOptionGraphicsItem *)op;
   if (!data->m_bStart) return;
   //n = data->n;
   if (data->m_coordinate == RIGHT_UP) ibit = -1;
   else ibit = 1;
   p->drawRect(boundingRect().x(), boundingRect().y(),
               boundingRect().width() - 1, boundingRect().height() - ibit);

   QString str;
   qreal x1, y1, x2, y2;
   //int idx,iy1;
   // p->drawRect(50, 50, 100, 100);
   // return;
   QPen o_pen;
   QBrush o_brush;

   int len;
   QString key;
   arrayData::M_DRAWMAP::iterator it;
   len = data->m_drawMAP.count();

   for (it = data->m_drawMAP.begin(); it != data->m_drawMAP.end(); ++it)
   {
      key = it.key();
      //qDebug()<< "in paint key ="<<key;
      //if (key != "2cur") continue;
  
      data->getDrawFromMAP(key);
      data->setDrawStyle(data->m_iDrawType, data->m_drawColor,
                         data->m_iDrawWide, data->m_iDrawFill,data->m_brushColor);

      o_pen = p->pen();
      p->setPen(data->m_pen);

      o_brush = p->brush();
      p->setBrush(data->m_brush);


      for (i = 0; i < data->m_iDrawLen; i++)
      {
         x1 = i;
         x2 =  i + 1;
         y1 = data->buf[i];
         y2 = data->buf[i + 1];
         paintP(p, x1, y1, x2, y2);
      }

      p->setPen(o_pen);
      p->setBrush(o_brush);
   }


   //qDebug() << "i  in painter = " << data->buf[5]<<i1<<i2<<data->n;
//why we can draw oot of bounding rect ???
}
void arrayPaint::paintP(QPainter *p, qreal x1, qreal y1, qreal x2, qreal y2)
{
   int ibit;

   if (data->m_coordinate == RIGHT_UP) ibit = -1;
   else ibit = 1;

   switch (data->m_iDrawType)
   {
   case DRAW_NULL:
      break;
   case DRAW_LINE:
      p->drawLine(scene2Item(x1, y1), scene2Item(x2, y2));
      //p->drawLine(map2Scene(x1,y1),map2Scene(x2,y2));
      // p->drawLine(mapFromScene(x1,y1),mapFromScene(x2,y2));
      //p->drawLine(x1,y1,x2,y2);
      break;
   case DRAW_AREA:
      p->drawLine(scene2Item(x1, y1), scene2Item(x1, 0));
      p->drawLine(scene2Item(x2, y2), scene2Item(x2, 0));
      p->drawLine(scene2Item(x1, y1), scene2Item(x2, y2));
      break;
   case DRAW_RECT:
      //mapFromScene(i,i1),mapFromScene(i+1,i2)
      p->drawRect(scene2Item(x1, y1).x(), scene2Item(x1, y1).y(),
                  scene2Item(x2, y2).x() - scene2Item(x1, y1).x(),
                  scene2Item(x1, ibit * y2).y() // -mapFromScene(x1,y1).y()
                 );
      break;
   default:
      p->drawLine(scene2Item(x1, y1), scene2Item(x2, y2));

   } // end of switch


}
void arrayPaint::setData(drawData *d)
{
   data = (arrayData *)d;
   drawing::data = d;
}
void arrayPaint::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
   //QGraphicsItem::mousePressEvent(e);
   //qDebug() << "mouse"; 
   drawing::mousePressEvent(e);
   int x,y,sx,sy;
   x=e->pos().x();
   y = e->pos().y();
   sx = e->scenePos().x();
   sy = e->scenePos().y();
   qDebug() << "array paint:x,y,sx,sy = " << x << y << sx << sy;
    

}
