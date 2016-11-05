#include "drawing.h"
#include "drawframe.h"

drawing::drawing():drawItem()
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    //point is ok;
    //the hight of range must be minus

}
void drawing::setData(drawData *d)
{
    data = d;
}
QPainterPath drawing::shape() const
{
     QPainterPath path;
     path.addRect(boundingRect());
     return path;
}
QRectF drawing::boundingRect()const
{
    qreal penWidth = 0;
    int h,w;
    h = data->m_frame.height - data->m_ruler.topW - data->m_ruler.bottomW;
    w = data->m_frame.width - data->m_ruler.leftW - data->m_ruler.rightW;
    //qDebug() << " bound = " << h << w;

    QRectF f;
    if(data->m_coordinate == RIGHT_UP)
        h = h*-1;

    f = QRectF(0 - penWidth / 2   ,
               0 - penWidth / 2  ,

               w  + penWidth,   h+ penWidth);

    // we cannot use view in this function;
    return f;

}
void drawing::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *widget)
{
    //plotOutlook(painter);

}
void drawing::plotOutlook(QPainter *p)
{
   int x0,y0,x1,y1;
   int wide;
// draw wide:
   if (!data->m_bOutlook) return;
  
// scene rect
   QRectF rfs,rfm;
   QPointF pf;
   float sx,sy,ssx,ssy,sl,w,h,ww,hh;
   //rfm =   mapRectToScene(boundingRect());
   
   rfs = scene()->sceneRect();
   ((drawFrame *)view())->getScale(sx, sy);
   

   w = rfs.width()*sx - (data->m_ruler.leftW + data->m_ruler.rightW);
   h = rfs.height()*sy- (data->m_ruler.topW + data->m_ruler.bottomW);
   wide = 100;
   ww = wide;
   sl = ww / w;
   hh = h *sl;
   // make sure the outlook is in the viewport();max 100 wide;
   #if 1
   while (hh > boundingRect().height())
   { 
      //qDebug() << "hh = " << hh << wide;
      ww = ww/2;
      sl = ww / w;
      hh = h *sl;
      //qDebug() << "in plot outlook" <<hh <<ww <<sl <<w <<
      if (ww < 1) break;
   }
   #endif

  
   
// item rect
   rfm =   boundingRect(); 
   w = rfm.width()*sl;
   h = rfm.height() *sl;
   if (w > ww) w = ww;
   if (h > hh) h = hh;

   pf = pos();
   x1 = pf.x() *sx* sl;
   y1 = pf.y() * sy * sl;


   x0 = rfm.width() - ww-1;
   y0 = rfm.height() - hh-1;
   p->fillRect(x0, y0, ww, hh,Qt::gray);   
   p->fillRect(x0+x1, y0+y1, w, h,Qt::black);
 
   //qDebug() << "rect = " << sl << w<<h << ww << hh;
}
QRectF drawing::getRect()
{
    return boundingRect(); 

}
