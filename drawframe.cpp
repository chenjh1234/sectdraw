#include "drawframe.h"
#include <QScrollBar>


drawFrame::drawFrame(QWidget *parent) : QGraphicsView(parent)
{
   //init();
   con = win.screen();
#if 0
   qDebug() <<"DotsPerInch_x="<< con->logicalDotsPerInchX();
   qDebug() <<"DotsPerInch_y"<< con->logicalDotsPerInchY();
   qDebug() <<"DotsPerInch_fx="<< con->physicalDotsPerInchX();
   qDebug() <<"DotsPerInch_fy"<< con->physicalDotsPerInchY();
   qDebug() << "ratio =" << con->devicePixelRatio();
   qDebug() << "size  =" << con->size();
   qDebug() << "xy  =" << con->size().rwidth()/con->size().rheight();
#endif
   dotsPerInchX = con->logicalDotsPerInchX();
   dotsPerInchY = con->logicalDotsPerInchY();
   cmsPerInch = 2.54;

}
void drawFrame::setRuler(int l, int r, int t, int b)
{
   m_leftW = l;
   m_rightW = r;
   m_topW = t;
   m_bottomW = b;
   updateRuler();

}
void drawFrame::reDraw()
{
   m_drawing->update();
   leftR->update();
   rightR->update();
   topR->update();
   bottomR->update();
}

void drawFrame::init()
{

   //m_coordinate = 0;//RIGHT_DOWN;
   setAlignment((Qt::AlignLeft | Qt::AlignTop));
   setFitWin();

//data:
//data = new drawData();
   setRuler(LABEL_WIDE, LABEL_WIDE, LABEL_WIDE, LABEL_WIDE);


//scene:
   sc = new drawScene();
   //sc->setData(data);
   setScene(sc);

   //m_sx = 1;
   //m_sy = 1;

//ruler£º
   leftR = new rulerFrame("LEFT");
   rightR = new rulerFrame("RIGHT");
   topR = new rulerFrame("TOP");
   bottomR = new rulerFrame("BOTTOM");


// add ruler to scene
   sc->addItem(leftR);
   sc->addItem(rightR);
   sc->addItem(topR);
   sc->addItem(bottomR);

// link data to ruler
   leftR->setData(data);
   rightR->setData(data);
   topR->setData(data);
   bottomR->setData(data);
#if 0
   bottomR->setZValue(Z_TOP);
   rightR->setZValue(Z_TOP);
   leftR->setZValue(Z_TOP);
   topR->setZValue(Z_TOP);
#endif

//textMsg
   textMsgTop = new messagePad(topR);
   textMsgBottom = new messagePad(bottomR);
   textMsgBottom->setPos(0, 15);
  // qDebug() << "drawFrame :: textMsgTop ===== " << textMsgTop;
// colorBar:

   m_colorBar = new colorBar(bottomR);

   setGrid(5, 50, 5, 50);
   setMaxMin(0, 0, 200, 200);
//color map dialog:
   m_iDPIx = logicalDpiX();
   m_iDPIy = logicalDpiY();
   cmap = new colorMapDlg(this);
//
   setPosLine(true);
   setRulerLine(false);
   setScale(1, 1);


}

void drawFrame::mouseMoveEvent(QMouseEvent *event)
{
   QGraphicsView::mouseMoveEvent(event);
   int x, y;
   x = event->x();
   y = event->y();
   QString str;
   //str.sprintf("%5.1f,%5.1f",mapToScene(x,y).x(),mapToScene(x,y).y());

   str.sprintf("%d,%d  %f,%f,%f,%f", x, y, mapToScene(x, y).x(), mapToScene(x, y).y(),
               transform().dx(), transform().dy());
   //qDebug()<< "111=str"<<str;
   //setTopMessage(str);
   setRulerLine(x, y);
}
void drawFrame::setTopMessage(QString msg)
{
  // qDebug() << "drawFrame::setTopMessage = " << msg;
   //qDebug() << "drawFrame::setTopMessage textMsgTop= " << textMsgTop;
   textMsgTop->setPlainText(msg);
  // qDebug() << "drawFrame::setTopMessage end";
}
void drawFrame::setBottomMessage(QString msg)
{
   textMsgBottom->setPlainText(msg);
}

void drawFrame::resizeEvent(QResizeEvent *e)
{
   QGraphicsView::resizeEvent(e);

   updateFrame();
   posRuler();
   m_colorBar->setSize(e->size().width() / 2, data->m_ruler.bottomW / 2);
   m_colorBar->changed();
   m_colorBar->setPos(data->m_frame.width / 2, data->m_ruler.bottomW / 2);
   posDraw();
   autoZ();
   isOutlook();
}

void drawFrame::scrollContentsBy(int dx, int dy)
{
   QGraphicsView::scrollContentsBy(dx, dy);

   posRuler();
   posDraw();
}

void drawFrame::updateRuler()
{
   data->m_ruler.leftW = m_leftW;
   data->m_ruler.rightW = m_rightW;
   data->m_ruler.topW = m_topW;
   data->m_ruler.bottomW = m_bottomW;
}

void drawFrame::updateFrame()
{
   /*
   int w,h;
   w = 0;
   h = 0;
   if(verticalScrollBar()->isVisible())
       w  = verticalScrollBar()->width();
   if(verticalScrollBar()->isVisible())
       h  = horizontalScrollBar()->height();
*/
   data->m_frame.width = viewport()->width();
   data->m_frame.height = viewport()->height();
}

void drawFrame::updateScene()
{
   QRectF f;
   f = sc->sceneRect();
   data->m_scene.x = f.x();
   data->m_scene.y = f.y();
   data->m_scene.width = f.width();
   data->m_scene.height = f.height();
   //qDebug() <<  "scene = " << data->m_scene.x << data->m_scene.y << data->m_scene.width<<data->m_scene.height;
   posRuler();
   posDraw();
}
void drawFrame::posRuler()
{
   int w, h; //,sw,sh;

   leftR->changed();
   rightR->changed();
   topR->changed();
   bottomR->changed();

   w = data->m_frame.width;
   h = data->m_frame.height;
   //sw = data->m_scene.width;
   //sh = data->m_scene.height;
   //  qDebug() << "left 0,0" << mapToScene(0,0);
   leftR->setPos(mapToScene(0, 0));
   rightR->setPos(mapToScene(w - m_rightW, 0));
   topR->setPos(mapToScene(0, 0));
   bottomR->setPos(mapToScene(0, h - m_bottomW));

   //draw->setPos(mapToScene(data->m_ruler.leftW,data->m_ruler.topW));
}
void drawFrame::setMaxMin(qreal minx, qreal miny, qreal maxx, qreal maxy)
{
   //qreal lw,rw,tw,bw;
   data->minX = minx;
   data->minY = miny;
   data->maxX = maxx;
   data->maxY = maxy;
   // qDebug() << "setMaxMin = " << data->minX<<data->minY<<data->maxX<<data->maxY;
   setMaxMin();

}
void drawFrame::setMaxMin()
{
   qreal lw, rw, tw, bw, y;
   qreal minx, miny, maxx, maxy;
   minx = data->minX;
   miny = data->minY;
   maxx = data->maxX;
   maxy = data->maxY;
   // qDebug() << "setMaxMin 11 = " <<minx << miny<< maxx<< maxy;

   lw = mapToSceneX(data->m_ruler.leftW);
   rw = mapToSceneX(data->m_ruler.rightW);
   tw = mapToSceneY(data->m_ruler.topW);
   bw = mapToSceneY(data->m_ruler.bottomW);

   //qDebug() << "setMaxMin 12 = "<< lw << rw << tw << bw;

   if (data->m_coordinate == RIGHT_UP) y = miny + tw;
   else y = miny - tw;
   sc->setSceneRect(minx - lw,
                    y,
                    maxx - minx + 1 + lw + rw,
                    maxy - miny + 1 + tw + bw);
   //qDebug() << "sssss="<<maxx<<minx<<lw<<rw;
   //qDebug() << "sssss=" << minx - lw << y << maxx - minx + 1 + lw + rw << maxy - miny + 1 + tw + bw;
   updateScene();
   //qDebug() << "update";
}

void drawFrame::setScale(qreal x, qreal y)
{
   float xx, yy;
   m_bFitWin = false;
   if (x == 0 || y == 0)
   {
      autoZ();
      return;
   }
   getScale(xx, yy);
   //scale(x,y);// it will mutiplt prevew scale;
   if (data->m_coordinate == RIGHT_UP) yy = -1 * yy;
   scale(x / xx, y / yy);
   //getScale(xx, yy);
   //qDebug() << "aaaaaa setscale = " << xx << yy;
   isOutlook();


   setMaxMin();
}
void drawFrame::setOutlook(bool b)
{
   data->m_bOutlook = b;
   reDraw();
}
bool drawFrame::getOutlook()
{
   return data->m_bOutlook;
}
void drawFrame::isOutlook()
{
   if (data->m_bOutlookNO)
   {
      data->m_bOutlook = false;
      return;
   }
   QRectF df, sf;
   float sx, sy;
   int w, h, iw, ih;
   getScale(sx, sy);
   df = m_drawing->getRect();
   sf = scene()->sceneRect();
   w = sf.width() * sx - (data->m_ruler.leftW + data->m_ruler.rightW);
   h = sf.height() * sy - (data->m_ruler.topW + data->m_ruler.bottomW);
   iw = df.width();
   ih = df.height();
   // qDebug()<<" in outlook =" << w << h << iw << ih;

   if (ih > 0 && iw > 0)
   {


      if (w / iw >= OUTLOOK_TIMES || h / ih >= OUTLOOK_TIMES)
      {
         data->m_bOutlook = true;
      }
      else data->m_bOutlook = false;
   }


}
float drawFrame::getScaleX()
{
   return mapFromSceneX(1);
}
float drawFrame::getScaleY()
{
   return mapFromSceneY(1);
}
void drawFrame::getScale(float& x, float& y)
{
   // qreal x1,y1
   x = mapFromSceneX(1);
   y = mapFromSceneY(1);
}
void drawFrame::getScale(qreal& x, qreal& y)
{
   // qreal x1,y1
   x = mapFromSceneX(1);
   y = mapFromSceneY(1);
}
void drawFrame::setCoordinate(int c)
{
/*
    x' = m11*x + m21*y + dx
    y' = m22*y + m12*x + dy
if (is not affine) {
     w' = m13*x + m23*y + m33
     x' /= w'
     y' /= w'
 }
 */

   if (data->m_coordinate == c) return;
   data->m_coordinate = c;

   QTransform f, f1;
   // view transform:

   setAlignment((Qt::AlignLeft | Qt::AlignBottom));

   scale(1, -1);
   //translate (0,50);
   //m_draw transform:
   // float fx,fy;
   // getScale(fx, fy);
   //qDebug()<< "scale = "<<fx<<fy;
   //qDebug()<< "transfer = "<<transform();
   //return;

   f = m_drawing->transform();
   f = f.scale(1, -1);
   //f = f.translate(0,50);
   m_drawing->setTransform(f);

   return;

}
void drawFrame::posDraw()
{
   if (!data->m_bStart) return;
   m_drawing->changed();

   m_drawing->setPos(mapToScene(data->m_ruler.leftW, data->m_ruler.topW));


}
void drawFrame::setGrid(qreal x1, qreal x2, qreal y1, qreal y2)
{
   //qreal xx;

   data->m_xygrid.x1 = x1;
   data->m_xygrid.x2 = x2;
   data->m_xygrid.y1 = y1;
   data->m_xygrid.y2 = y2;

}

void drawFrame::setGrid(qreal x1, qreal x2) //y is the same;
{
   setGrid(x1, x2, x1, x2);

}

void drawFrame::setGrid(bool b) //is m_draw draw the grid;
{
   data->m_xygrid.grid = b;
}
bool drawFrame::getGrid()
{
   return data->m_xygrid.grid;
}
void drawFrame::setColorBar(int id)
{
   if (id == COLORBAR_NO)
   {
      m_colorBar->hide();
   }
   else
   {
      m_colorBar->show();
   }

}
qreal drawFrame::mapToSceneX(qreal x)
{
   QPointF p, p1;
   p = mapToScene(0, 0);
   p1 = mapToScene(x, 0);
   return p1.rx() - p.rx();
}

qreal drawFrame::mapToSceneY(qreal y)
{
   QPointF p, p1;
   p = mapToScene(0, 0);
   p1 = mapToScene(0, y);
   return p1.ry() - p.ry();
}

qreal drawFrame::mapFromSceneX(qreal x)
{
#if 0
   QPointF p,p1;
   p = mapFromScene(0,0);
   p1 = mapFromScene(x,0); // found the return is int
   return p1.rx() - p.rx();
#endif
   QMatrix mt;
   mt = matrix();
   return x * mt.m11();
}

qreal drawFrame::mapFromSceneY(qreal y)
{
#if 0
   QPointF p,p1;
   p = mapFromScene(0,0);
   p1 = mapFromScene(0,y);
   return p1.ry() - p.ry();
#endif
   QMatrix mt;
   mt = matrix();
   return y * mt.m22();


}
void drawFrame::setFitWin()
{
   m_bFitWin = true;
   autoZ();
}
void drawFrame::autoZ()
{
   if (!m_bFitWin) return;
   //fitInView(data->m_scene.x,data->m_scene.y,data->m_scene.width,data->m_scene.height);
   //return;

   int w, h;
   h = data->m_frame.height - data->m_ruler.topW - data->m_ruler.bottomW;
   //h = data->m_frame.height+data->m_ruler.topW + data->m_ruler.bottomW;;
   w = data->m_frame.width - data->m_ruler.leftW - data->m_ruler.rightW;

   if (w <= 0 || h <= 0) return;

   qreal minx, maxx, miny, maxy; //lw,rw,tw,bw;
   minx = data->minX;
   miny = data->minY;
   maxx = data->maxX;
   maxy = data->maxY;
   qreal sx, sy;
// insure no scrollBar appeared,
   /** this is not good,it easy to cause:
           no ruler,make the scale crazy
   sx = (w-1)/(maxx -minx + 1 );
   sy = (h-1)/(maxy - miny +1 );
     */
   sx = (w) / (maxx - minx + 1 + 0.001);
   sy = (h) / (maxy - miny + 1 + 0.001);

   //x = mapToSceneX(1);
   //y = mapToSceneY(1); //
   //qDebug() << "in autoz,w,h,miny,maxy="<<w<<h<<miny<<maxy<<minx<<maxx;
//    return;
   setScale(sx, sy);
   //setFitWin(m_bFitWin);
   m_bFitWin = true;
   return;

}
/*
void drawFrame::focusInEvent ( QFocusEvent * e )
{
    QGraphicsView::focusInEvent(e);
    qDebug() << "in "<< e->reason();
    setRulerLine(TRUE);


}
void drawFrame::focusOutEvent ( QFocusEvent * e )
{
    QGraphicsView::focusOutEvent(e);
    qDebug() << "out " <<e->reason();
    setRulerLine(FALSE);

}
*/
void drawFrame::setRulerLine(int x, int y)
{
   QPointF p, p1;
   if (!m_bPosLine) return;
   p = mapToScene(x, y);
   p1 = m_drawing->scene2Item(p.x(), p.y());
   if (m_drawing->contains(p1))
   {
      setRulerLine(true);
      leftR->m_line->setPos(0, y);
      rightR->m_line->setPos(0, y);
      topR->m_line->setPos(x, 0);
      bottomR->m_line->setPos(x, 0);
   }
   else
   {
      setRulerLine(false);
   }
}
void drawFrame::setPosLine(bool r)
{
   m_bPosLine = r;
}

void drawFrame::setRulerLine(bool r)
{
   if (r)
   {
      leftR->m_line->show();
      rightR->m_line->show();
      topR->m_line->show();
      bottomR->m_line->show();
   }
   else
   {
      leftR->m_line->hide();
      rightR->m_line->hide();
      topR->m_line->hide();
      bottomR->m_line->hide();
   }
   //scene()->update();
   //reDraw();
}
