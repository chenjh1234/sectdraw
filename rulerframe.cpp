#include "rulerframe.h"
#define TOP_LAB 1
#define BOTTOM_LAB 2
#define LEFT_LAB 3
#define RIGHT_LAB 4

rulerFrame::rulerFrame(QString str) : drawItem()
{
   where = str;

   setFlag(QGraphicsItem::ItemIgnoresTransformations);
   sline = 5;
   lline = 10;
   m_line = new QGraphicsLineItem(this);
   m_line->setPen(QPen(Qt::red));
   setZValue(Z_TOP);

}
void rulerFrame::setData(drawData *d)
{
   data = d;
}
QRectF rulerFrame::boundingRect() const
{

   if (where == "LEFT") return leftRect();
   if (where == "RIGHT") return rightRect();
   if (where == "TOP") return topRect();
   if (where == "BOTTOM") return bottomRect();

   return bottomRect();

}
QRectF rulerFrame::leftRect() const
{

   qreal penWidth = 0;
   int h, w;
   h = data->m_frame.height;
   w = data->m_ruler.leftW;
   m_line->setLine(0, 0, (qreal)w, 0);
   //qDebug() << w<<h;
   return QRectF(0 - penWidth / 2, 0 - penWidth / 2,
                 w + penWidth, h + penWidth);

}

QRectF rulerFrame::rightRect() const
{
   return leftRect();

}

QRectF rulerFrame::topRect() const
{

   qreal penWidth = 0;
   int h, w;
   w = data->m_frame.width;
   h = data->m_ruler.topW;
   m_line->setLine(0., 0., 0., (qreal)h);
   return QRectF(0 - penWidth / 2, 0 - penWidth / 2,
                 w + penWidth, h + penWidth);

}

QRectF rulerFrame::bottomRect() const
{
   return topRect();

}

void rulerFrame::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{

   //qreal x1,y1;
   //return;
   painter->fillRect(boundingRect(), Qt::white);

   if (where == "LEFT") leftPaint(painter, option, widget);
   if (where == "RIGHT") rightPaint(painter, option, widget);
   if (where == "TOP") topPaint(painter, option, widget);
   if (where == "BOTTOM") bottomPaint(painter, option, widget);

}
void rulerFrame::leftPaint(QPainter *p, const QStyleOptionGraphicsItem *op, QWidget *ww)
{
   vPaint(p, op, ww, LEFT_LAB);
   return;

}
void rulerFrame::rightPaint(QPainter *p, const QStyleOptionGraphicsItem *op, QWidget *ww)
{
   vPaint(p, op, ww, RIGHT_LAB);
   return;
}
void rulerFrame::vPaint(QPainter *p, const QStyleOptionGraphicsItem *op, QWidget *ww, int flag)
{
   int h, w, step1, i, step2, tw, bw;
   int x1, x2;
   QString str;
   //QStyleOptionGraphicsItem *op1;
   // QWidget *ww1;
   // ww1 = ww;
   //op1  = (QStyleOptionGraphicsItem *)op;
   h = data->m_frame.height;
   w = data->m_ruler.rightW;
   tw = data->m_ruler.topW;
   bw = data->m_ruler.bottomW;
   step1 = data->m_xygrid.iy1;
   step2 = data->m_xygrid.iy2;

#if 0
   if (data->m_xygrid.y1 == 0)
   {
      for(i = tw; i < h-bw; i=i+step1)
      {
         if(i /step2*step2 == i|| i == tw)
         {
            if(flag == LEFT_LAB)
            {
               x1 = w - lline;
               x2 = w;
            }
            else
            {
               x1 = 0;
               x2 = lline;
            }
            p->drawLine(x1, i, x2, i);
            str.sprintf("%.0f",map2Scene(0,i).y());
            if(flag == LEFT_LAB) x1 = 0;
            else x1 = lline;
            p->drawText(x1,i,str);
         }
         else
         {
            if(flag == LEFT_LAB)
            {
               x1 = w - sline;
               x2 = w;
            }
            else
            {
               x1 = 0;
               x2 = sline;
            }
            p->drawLine(x1,i,x2,i);
         }

      }
      str.sprintf("%.0f",map2Scene(0,h-bw-1).y());
      if(flag == LEFT_LAB) x1 = 0;
      else x1 = lline;

      p->drawText(x1,h-bw-1,str);
   }
#endif
   qreal s1, s2, s3; //scene data range to diaplay
   qreal st1, st2; //grid parameters
   int ist1, ist2, j;
   qreal f, f1, ff;
   st1 = data->m_xygrid.y1;
   st2 = data->m_xygrid.y2;
   ist1 = st1;
   ist2 = st2;
   s1 = map2Scene(0, tw).y();
   s2 = map2Scene(0, h - bw).y();
   if (s2 > data->maxY) s2 = data->maxY;
 
   s3 = map2Scene(0, tw + 1).y();
   f1 = s3 - s1;
   if (f1 <= 0) f1 = 1; //dots

   if (data->m_coordinate == RIGHT_UP)
   {
      s1 = s1 * -1;
      s2 = s2 * -1;
   }

#if 0
// every 5 dots,and 50 dots have label
   i = 0;

   for (ff = s1; ff < s2; ff = ff + f1) // loop every dots
   {
      if (data->m_coordinate == RIGHT_UP) f = ff * -1;
      else f = ff;


      // ensure 1st time enter in the some dot;
      if (int(i/ ist1) * ist1 == i)
      {
         if (flag == LEFT_LAB)
         {
            x1 = w - sline;
            x2 = w;
         }
         else
         {
            x1 = 0;
            x2 = sline;
         }
         p->drawLine(x1, scene2Item(0, f).y(), x2, scene2Item(0, f).y());
      }

      if (int(i / ist2) * ist2 == i) //|| i == s1)
      {
         if (flag == LEFT_LAB)
         {
            x1 = w - lline;
            x2 = w;
         }
         else
         {
            x1 = 0;
            x2 = lline;
         }
         p->drawLine(x1, scene2Item(0, f).y(), x2, scene2Item(0, f).y());
         str.sprintf("%.0f", data->labMethodY(f));
         // str.sprintf("%d",i);
         if (flag == LEFT_LAB) x1 = 0;
         else x1 = lline;

         p->drawText(x1, scene2Item(0, f).y(), str);
      }
      i++;
   }
#endif
#if 0
// for exactly data control, the print label is not all printed
   for (ff = s1; ff < s2; ff = ff + f1) // loop every dots
   {
      if (data->m_coordinate == RIGHT_UP) f = ff * -1;
      else f = ff;

      i = ff;
      if (i != j) // ensure 1st time enter in the some dot;
      if (int(ff / ist1) * ist1 == i )
      {
         if (flag == LEFT_LAB)
         {
            x1 = w - sline;
            x2 = w;
         }
         else
         {
            x1 = 0;
            x2 = sline;
         }
         p->drawLine(x1, scene2Item(0, f).y(), x2, scene2Item(0, f).y());
      }
      if (i != j)
      //if (ff / ist2 * ist2 == i) //|| i == s1)
      if (int(ff / ist2) * ist2 == i )
      {
         if (flag == LEFT_LAB)
         {
            x1 = w - lline;
            x2 = w;
         }
         else
         {
            x1 = 0;
            x2 = lline;
         }
         p->drawLine(x1, scene2Item(0, f).y(), x2, scene2Item(0, f).y());
         str.sprintf("%.0f", data->labMethodY(f));
         // str.sprintf("%d",i);
         if (flag == LEFT_LAB) x1 = 0;
         else x1 = lline;

         p->drawText(x1, scene2Item(0, f).y(), str);
      }
      j = i;
   }
#endif
#if 1
// for exactly dabled :data control
   int i1, j1;
   i1 = -1;
   j1 = -1;
   //qDebug() << "s1 = " << s1;
   for (ff = s1; ff < s2; ff = ff + f1) // loop every dots
   {
      if (data->m_coordinate == RIGHT_UP) f = ff * -1;
      else f = ff;

      i = int(ff / ist1);
      j = int(ff / ist2);
      //qDebug() << i << i1 << j << j1;
      if (i1 != i) 
      {
         //qDebug() << " in s="<< i << i1;
         if (flag == LEFT_LAB)
         {
            x1 = w - sline;
            x2 = w;
         }
         else
         {
            x1 = 0;
            x2 = sline;
         }
         p->drawLine(x1, scene2Item(0, f).y(), x2, scene2Item(0, f).y());
      }

      if (j1 != j)
      {
         if (flag == LEFT_LAB)
         {
            x1 = w - lline;
            x2 = w;
         }
         else
         {
            x1 = 0;
            x2 = lline;
         }
         p->drawLine(x1, scene2Item(0, f).y(), x2, scene2Item(0, f).y());
         
         if (flag == LEFT_LAB)str=QString("%1").arg(int(data->labMethodY(f)),LABEL_SPACE);
         else str=QString("%1").arg(int(data->labMethodY(f)));

         // str.sprintf("%d",i);
         if (flag == LEFT_LAB) x1 = 0;
         else x1 = lline;

         p->drawText(x1, scene2Item(0, f).y(), str);
         //p->drawText(x1, int(scene2Item(0, f).y()/ist2)*ist2, str);
      }
      i1 = i;
      j1 = j;
   }
    
#endif

}
void rulerFrame::topPaint(QPainter *p, const QStyleOptionGraphicsItem *op, QWidget *ww)
{
   hPaint(p, op, ww, TOP_LAB);
   return;

}
void rulerFrame::bottomPaint(QPainter *p, const QStyleOptionGraphicsItem *op, QWidget *ww)
{
   hPaint(p, op, ww, BOTTOM_LAB);
   return;

}
void rulerFrame::hPaint(QPainter *p, const QStyleOptionGraphicsItem *op, QWidget *ww, int flag)
{
   int h, w, step1, i, j, step2;
   int lw, rw;
   QString str;
   int y1, y2;
   //QStyleOptionGraphicsItem *op1;
   // QWidget *ww1;
   // ww1 = ww;
   //op1  = (QStyleOptionGraphicsItem *)op;
   w = data->m_frame.width;
   h = data->m_ruler.topW;
   lw = data->m_ruler.leftW;
   rw = data->m_ruler.rightW;
   step1 = data->m_xygrid.ix1;
   step2 = data->m_xygrid.ix2;
#if 0
   if (data->m_xygrid.x1 == 0)
   {
      for(i = lw; i < w-rw; i= i+step1)
      {
         if(i /step2*step2 == i|| i == lw)
         {
            if(flag == BOTTOM_LAB)
            {
               y1 = 0;
               y2 = lline;
            }
            else //TOP:
            {
               y1 = h - lline;
               y2 = h;

            }
            p->drawLine(i, y1, i,y2);
            str.sprintf("%.0f",map2Scene(i,0).x());
            if(flag == BOTTOM_LAB) y1 = lline + 8;
            else y1 = h - lline;

            p->drawText(i, y1, str);
         }
         else
         {
            if(flag == BOTTOM_LAB)
            {
               y1 = 0;
               y2 = sline;
            }
            else //TOP:
            {
               y1 = h - sline;
               y2 = h;

            }
            p->drawLine(i,y1 , i , y2);
         }
      }
   }
#endif
   qreal s1, s2, s3; //scene data range to display
   qreal st1, st2; //grid parameters
   int ist1, ist2;
   qreal f, f1;
   st1 = data->m_xygrid.x1;
   st2 = data->m_xygrid.x2;
   ist1 = st1;
   ist2 = st2;
   s1 = map2Scene(lw, 0).x();
   s2 = map2Scene(w - rw, 0).x();
   if ((s2 > data->maxX)) s2 = data->maxX;
 
   s3 = map2Scene(lw + 1, 0).x();
   f1 = s3 - s1; // step :is every dots in viewport
   if (f1 <= 0) f1 = 1;

#if 1
//data grid control:
   int i1, j1;
   i1 = -1;
   j1 = -1;

   for (f = s1; f < s2; f = f + f1)
   {
      i = f / ist1;
      j = f / ist2;
      if (i != i1)
      {
         if (flag == BOTTOM_LAB)
         {
            y1 = 0;
            y2 = sline;
         }
         else //TOP:
         {
            y1 = h - sline;
            y2 = h;

         }
         p->drawLine(scene2Item(f, 0).x(), y1, scene2Item(f, 0).x(), y2);
      }

      if (j != j1) //|| i == s1)
      {
         if (flag == BOTTOM_LAB)
         {
            y1 = 0;
            y2 = lline;
         }
         else //TOP:
         {
            y1 = h - lline;
            y2 = h;

         }
         p->drawLine(scene2Item(f, 0).x(), y1, scene2Item(f, 0).x(), y2);
          
          
         //   str.sprintf("%.0f", float(int(data->labMethodX(f)/ist2) * ist2));
         str=QString("%1").arg(int(data->labMethodX(f)));

         // str.sprintf("%d",i);
         if (flag == BOTTOM_LAB) y1 = lline + 8;
         else y1 = h - lline;
         p->drawText(scene2Item(f, 0).x(), y1, str);
         //p->drawText(int(scene2Item(f, 0).x()/ist2)*ist2, y1, str);
      }
      j1 = j;
      i1 = i;
   }
#endif
#if 0
// data grid control ,some label conot printed
   for (f = s1; f < s2; f = f + f1)
   {
      i = f;
      if (i != j) if (i / ist1 * ist1 == i)
      {
         if (flag == BOTTOM_LAB)
         {
            y1 = 0;
            y2 = sline;
         }
         else //TOP:
         {
            y1 = h - sline;
            y2 = h;

         }
         p->drawLine(scene2Item(f, 0).x(), y1, scene2Item(f, 0).x(), y2);
      }

      if (i != j) if (i / ist2 * ist2 == i) //|| i == s1)
      {
         if (flag == BOTTOM_LAB)
         {
            y1 = 0;
            y2 = lline;
         }
         else //TOP:
         {
            y1 = h - lline;
            y2 = h;

         }
         p->drawLine(scene2Item(f, 0).x(), y1, scene2Item(f, 0).x(), y2);
         str.sprintf("%.0f", data->labMethodX(f));
         // str.sprintf("%d",i);
         if (flag == BOTTOM_LAB) y1 = lline + 8;
         else y1 = h - lline;
         p->drawText(scene2Item(f, 0).x(), y1, str);
      }
      j = i;
   }
#endif
}





