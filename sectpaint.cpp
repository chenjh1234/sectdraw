#include "sectpaint.h"
//#define data (sectData *)data
#include "sectdraw.h"
#include <QDebug>

sectPaint::sectPaint() : drawing()
{
   imgCom = NULL;
   xPressed = -1;
   m_line = new QGraphicsLineItem(this);
}
sectPaint::~sectPaint()
{
   if (img != NULL) delete img;
}


void sectPaint::paint(QPainter *p, const QStyleOptionGraphicsItem *op,
                      QWidget *w1)
{
   //return;

   int ibit;
   int iw, ih, str, trs, ssam, sam, x0, y0;

   if (!data->m_bStart) return;
   //n = data->n;

   if (data->m_coordinate == RIGHT_UP) ibit = -1;
   else ibit = 1;

   iw = boundingRect().width() - 1;
   ih = boundingRect().height() - ibit;
// fixed scale:

   QRectF f;
   QPointF pf;

   f =   map2Scene(QRectF(0, 0, iw, ih));

   str = f.x();
   if (str < 0) str = 0;

   trs = f.width();
   if (trs > data->trs) // image is not full in horizon
   {
      trs = data->trs;
      //pf = scene2Item(trs, 0);// if trs ,the buf will exceed.
      pf = scene2Item(trs - 1, 0); // trs -1,ensure the buf is not exceed.
      iw = pf.x();
   }
   //qDebug() << "iw = " << iw <<trs;

   ssam = f.y(); //time:
   if (ssam < 0) ssam = 0;

   sam = f.height(); //time
                     //qDebug() << "ssam = " << ssam <<sam;
   if (sam > data->sam * data->si) // image is not full in vertical
   {
      sam = data->sam * data->si; //time
      pf = scene2Item(0, sam); //time
      ih = pf.y(); // *data->si;
   }
   //qDebug() << "iw,ih= " << iw<<ih;

   /**
   core down if drawing area is nagative:
    */
   if (iw <= 0 || ih <= 0) return;

   plotSect(iw, ih, str, trs, ssam, sam);
   if (data->m_xygrid.grid)
       plotHLine(0,0,iw,ih);

   x0 = 0;
   y0 = 0;
   //plotHLine(x0,y0,iw,ih);
   //qDebug() << "paint the image"<<img->bits()[34]<<img->bits()[35];
   p->drawImage(x0, y0, *img);
   // for compare:
   //if (imgCom !=NULL && data->m_compareMode != COMPARE_MODE_NO)
   if (((sectDraw *)view())->m_comView != NULL && data->m_compareMode != COMPARE_MODE_NO)
   {
      //qDebug() << "painter view = " << ((sectDraw *)view())->m_comView << view() <<imgCom;

      if (data->m_iDrawType == DRAW_WA) setComImageColor(3); // blue to white

      if (data->m_compareMode == COMPARE_MODE_V) p->drawImage(data->m_xCom, y0,
                                                              imgCom->copy(data->m_xCom, y0,
                                                                           imgCom->width() - data->m_xCom, imgCom->height()));
      if (data->m_compareMode == COMPARE_MODE_H) p->drawImage(x0, data->m_yCom,
                                                              imgCom->copy(x0, data->m_yCom,
                                                                           imgCom->width(), imgCom->height() - data->m_yCom));
      if (data->m_compareMode == COMPARE_MODE_TURNOVER && data->bDrawTurnover) p->drawImage(x0, y0, *imgCom);
      //p->drawImage( x0, y0,*imgCom);
   }


   plotOutlook(p);
   plotPick(p, iw, ih, str, trs, ssam, sam);

}


int  sectPaint::setImageColor()
{
   int i;
   int len = BAR_COLORS;
   int idx, id;
   idx = 0;
   idx = cb->m_iIdx;
   //qDebug() << "idx = " << idx;
   //idx >0 all the time

   for (i = 0; i < len; i++)
   {
      id = i + idx;
      if (id >= len) id = id - len;

      //img->setColor(i,cb->m_color[idx].rgb());// gb->r
      img->setColor(i, cb->m_color[id].rgb()); // gb->r

   }
   return 0;
}
int  sectPaint::setComImageColor(int iw)
{
   int i, ic;
   int len = BAR_COLORS;
   int idx, id;
   idx = 0;
   idx = cb->m_iIdx;
   //qDebug() << "idx = " << idx;
   //idx >0 all the time
   ic = cb->getColor(); //get current table;
   cb->setColor(iw); // set blue to white corlor table;

   for (i = 0; i < len; i++)
   {
      id = i + idx;
      if (id >= len) id = id - len;

      //img->setColor(i,cb->m_color[idx].rgb());// gb->r
      imgCom->setColor(i, cb->m_color[id].rgb()); // gb->r

   }
   cb->setColor(ic); //restore the table
   return 0;
}

int sectPaint::plotWA(int iw, int ih, int sttr, int trs, int ssam, int sams)
{
   //infact :trs sam is not effective;

   int i, j;
   unsigned char *imgdata;
   float *buf, fmax, fmin, startT, startS;
   int stsam, sam;
   QPointF pf;
// stsam,sam:
   stsam = ssam / data->si;
   sam = sams / data->si;
   if (trs <= 0) trs = 1;
//startT,startS :fraction of the data:
   pf = scene2Item(sttr + 1, stsam + 1); //start from the positon dot the trace1
   startT  = pf.x(); // adjest to the dot that not exactly on the  trace
   pf = map2Scene(0, 0); //time
   startS = (pf.y() / data->si - stsam);
   //startT = pf.x() - sttr; //VD: use this ,is exactly the dot
//buf:
   buf = data->buf;
   fmax = data->fmax;
   fmin = data->fmin;
   int bytes;
   bytes = img->bytesPerLine();
   imgdata = img->bits();

// step,hspd:
   float step;
   // for display trs ,give trs+1 space of traces interval dots
   if (trs > 0)
      // step = (float)iw /((float)(trs-1));
      //step = (float)iw / ((float)(trs));
      step = ((drawFrame *)view())->getScaleX();
   else step = iw;

// gain:
   float g, xx; //(x:0-10)
   xx = data->m_fGain;
   if (xx < 0) xx = 0;
   g = pow((qreal)2, (qreal)xx / GAIN_MAX);
// draw loop :

   float *x, *y;
   int *iy;

   unsigned char pix1;
   pix1 = 0x00;
   plt.pltinit(iw, ih, bytes, imgdata, pix1);

   y = new float[ih + 10];
   iy = new int[ih + 10];

   float ii;
   i = 0;
   //for (i = 0,ii = startT; ii < iw ; ii += step, i++)
   for (i = 0,ii = startT; ii < iw && (sttr + i) <= data->trs - 1; ii += step, i++)
   {
      x = buf + (sttr + i) * data->sam + stsam;
      intp.intpy((float *)x, (float *)y, sam, ih, startS); // interplate in vertical

// get the image data in vertical:
      for (j = 0; j < ih; j++)
      {
         iy[j] = (y[j] - fmin) * g / (fmax - fmin) * step;
      }
      plt.pltwiggle(int(ii), 0, ih, iy);
      plt.pltpa(int(ii), 0, ih, iy);
   }

   delete[] iy;
   delete[] y;

   return 0;
}

//================================================================
int sectPaint::plotVD(int iw, int ih, int sttr, int trs, int ssam, int sams)
{
   //infact :trs sam is not effective;

   int i, j;
   unsigned char *imgdata;
   float *buf, fmax, fmin, startT, startS;
   int stsam, sam;
   QPointF pf;
// trs,sam:
   stsam = ssam / data->si;
   sam = sams / data->si;
   if (trs <= 0) trs = 1;
//startT,startS :fraction of the data:
   pf = map2Scene(0, 0); //time
   startT = pf.x() - sttr; //ok is rigth: we can scrool horizon smoothly now;
   startS = (pf.y() / data->si - stsam);
//buf:
   buf = data->buf;
   fmax = data->fmax;
   fmin = data->fmin;

//window msg:
   int bytes; // number of bytes per line
   bytes = img->bytesPerLine();
   imgdata = img->bits();

//step:hspd:

   float hspd, f, f1;
   int ispd;
   int i0, i1, i2, i3;
   //step = (float)iw /((float)(trs));
   hspd  = 1 / ((drawFrame *)view())->getScaleX();
   // hspd = float(trs - 1) / (float)(iw); // sample in horizon//dots per TR
   //hspd = float(trs+1)/ (float )(iw-1);// sample in horizon trs/dots:
   //hspd = float(trs -1)/ (float )(iw);// sample in horizon//dots per TR
   // qDebug() << "hspd = " << hspd << 1 / data->m_fScaleX;
//gain:
   float g, xx; //(x:0-GAIN_MAX)
   xx = data->m_fGain;
   if (xx < 0) xx = 0;
   g = pow((float)2, (float)xx / GAIN_MAX);

// draw loop:

   float *x, *y;
   x = new float[sam + 10];
   y = new float[ih + 10];

   float ff;
   ff = startT;
// control the first trace draw to the position of 1 trace dots
   if (sttr > 0)   sttr = sttr - 1;

/**:120 traces of the gather:
 * the begin 3traces of gather:position0:
 * 0-0-0-1,0-0-1-2,0-1-2-3
 * the last 3traces of gather:positon120:
 * 116-117-118-119,117-118-119-119,118-119-119-119
 */
   for (i = 0 , f = ff; i < iw; i++, f += hspd) //donot control the last over trace here
                                                //for (i = 0 , f = ff; i < iw && (sttr+f) <data->trs; i++, f += hspd) // we control the last trace in the loop;
   {
      ispd = f; // int of (f), f: re sample of horizon,(traces)
                //ispd = f-startT;

      i0 = ispd - 1;
      //if(i0 <0) i0 = 0;
      //if(sttr + i0 <0) i0 = 0;
      //if( sttr +i0 > data->trs -1) i0 = data->trs -1;
      //2015.8:
      // qDebug() << "in =" <<i<< ispd << i0 << f <<trs << iw;
      if (sttr + i0 < 0) i0 = 0 - sttr;
      if (sttr + i0 > data->trs - 1) i0 = data->trs - 1  - sttr;

      i1 = ispd;
      if (sttr + i1 < 0) i1 = 0 - sttr;
      if (sttr + i1 > data->trs - 1) i1 = data->trs - 1 - sttr;

      i2 = ispd + 1;
      if (sttr + i2 < 0) i2 = 0 - sttr;
      if (sttr + i2 > data->trs - 1) i2 = data->trs - 1 - sttr;

      i3 =  ispd + 2;
      if (sttr + i3 < 0) i3 = 0 - sttr;
      if (sttr + i3 > data->trs - 1) i3 = data->trs - 1 - sttr;

      f1 = f - ispd; // f1 < 1;
                     //if (i <10)
                     //    qDebug() << "f1 = " << i << iw << f1 << f << ispd;
                     // the last traces
#if 0 // test the last trace:
      if (sttr + ispd >= data->trs-1)
      {
         qDebug() << sttr + ispd << ispd;
      }
#endif

      intp.vlgint(sam, &f1,
                  (float *)buf + (sttr + i0) * data->sam + stsam,
                  (float *)buf + (sttr + i1) * data->sam + stsam,
                  (float *)buf + (sttr + i2) * data->sam + stsam,
                  (float *)buf + (sttr + i3) * data->sam + stsam,
                  (float *)x); // interplate in horizon;  resolution:dots

      intp.intpy(x, y, sam, ih, startS); // interplate in vertical:resolution:sample;
// control the over trace:
      if (sttr + ispd > data->trs - 1) for (j = 0; j < ih; j++) y[j] = 0;


// get the image data in vertical:

      for (j = 0; j < ih; j++)
      {
         imgdata[j * bytes + i] = (y[j] - fmin) * g / (fmax - fmin) * 128 + 128;
      }
      // qDebug() << "in VD= "<<img->bits()[34]<<img->bits()[35];

   }
   //qDebug() << "after VD= "<<img->bits()[34]<<img->bits()[35];
   delete[] x;
   delete[] y;
   //qDebug("end of vd\n" );
   return 0;
}

int sectPaint::plotSect(int w, int h, int sttr, int trs, int stsam, int sam)
{
   //window msg:
   int bytes; // number of bytes per line
   bytes = (w + 4 - 1) / 4 * 4;
// image:
   if (img != NULL) delete img;

   img = new QImage(bytes, h, QImage::Format_Indexed8);

   if (!data->m_bTranspalency)
   {
      setImageColor();
      img->fill(0xff);
   }
//   qDebug() << "sect = " << data->m_iDrawType;
//    qDebug() << "bf sect "<<img->bits()[34]<<img->bits()[35];
   if (data->m_iDrawType == DRAW_VD)    plotVD(w, h, sttr, trs, stsam, sam);
   else plotWA(w, h, sttr, trs, stsam, sam);
   return 0;

}
int sectPaint::plotHLine(int x, int y, int w, int h)
{
   int ib, i, ix, iy,j,j1; //,isi;
   int step, nbpl, col;
   unsigned int bmask;
   unsigned char *bit;
   float ff;
   QPointF pf;
   ix = x;
   ix = 0;
   bit =  img->bits();
   bmask = 0x11111111; // dot line:1 dot 3 null dot.
   bmask = 0xffffffff;
   col = 0x00;
   nbpl = img->bytesPerLine();
   step = data->m_xygrid.y2;
   //step = 100;
   ib = 1;
   j1 = -1;
   if (bit != NULL)
    for (i = ib; i < y + h; i++)
      {
         iy = i - y;
         pf = map2Scene(1,iy);
         ff= pf.y();
         j = int(ff)/step;
         if (j1 != j)
         {
            // qDebug()<<" j =" << j<<j1<<ff << ix<<iy<<i;
             if (iy >= 0 && iy < img->height() - 1)
                  plt.hlined(ix, iy, w, nbpl, col, bmask, bit);
         }
         j1 = j;
      }
   return 0;


}

void sectPaint::setData(drawData *d)
{
   data = (sectData *)d;
   drawing::data = d;
}
//mouse event: =====================================================================
void sectPaint::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
   //QGraphicsItem::mousePressEvent(e);
   drawing::mousePressEvent(e);
// pick layer:
   if (data->m_pickMode != data->PICK_LAYER) return;

   e->setAccepted(true);
   if (e->button() == Qt::LeftButton) m_iButton = 0;
   if (e->button() == Qt::RightButton) m_iButton = 1;
   // any button:
   {
      xPressed = e->pos().x();
      yPressed = e->pos().y();
      sxPressed = e->scenePos().x();
      syPressed = e->scenePos().y();
      xMove = xPressed;
      yMove = yPressed; //remove start moving line
   }

}
void sectPaint::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
{
   //qDebug() << "Move = " << xPressed<<yPressed<<xMove << yMove;
   drawing::mouseMoveEvent(e);
// pick layer:
   if (data->m_pickMode != data->PICK_LAYER) return;
   xMove = e->pos().x();
   yMove = e->pos().y();
   update(getRect());

}

void sectPaint::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
   //qDebug() << " release button";
   //QGraphicsItem::mousePressEvent(e);
   drawing::mouseReleaseEvent(e);
// pick layer:
   if (data->m_pickMode != data->PICK_LAYER) return;
   int sx, sy, sx1, sy1;
// add PickPoint:
   if (e->button() == Qt::LeftButton)
   {

      sx = e->scenePos().x();
      sy = e->scenePos().y();
      sx1 = sxPressed;
      sy1 = syPressed;
      if (sx > sx1) addPickPoint(sx1, sy1, sx, sy);
      else addPickPoint(sx, sy, sx1, sy1);
      //qDebug() << " add ok";
   }
// remove button:
   if (e->button() == Qt::RightButton)
   {

      sx = e->scenePos().x();
      sy = e->scenePos().y();
      sx1 = sxPressed;
      sy1 = syPressed;
      if (sx > sx1) removePickPoint(sx1, sx);
      else removePickPoint(sx, sx1);
   }
// remove moving line;
   xPressed = -1;
   m_line->setLine(0, 0, 0, 0);
   //qDebug() << " add1 ok";
   update(getRect());
   //qDebug() << " add2 ok";
}
/// pick event: =====================================================================
void sectPaint::plotPick(QPainter *p, int iw, int ih, int sttr, int trs, int ssam, int sams)
{
   //infact :trs sam is not effective;
   if (data->m_pickMode != data->PICK_LAYER) return;

   int i;
   float  startT;
   int stsam;
   // *item;
   QPointF pf;
// stsam,sam:
   stsam = ssam / data->si;
   //sam = sams / data->si;
   if (trs <= 0) trs = 1;
//startT,startS :fraction of the data:
   pf = scene2Item(sttr + 1, stsam + 1); //start from the positon dot the trace1
   startT  = pf.x(); // adjest to the dot that not exactly on the  trace
   pf = map2Scene(0, 0); //time
                         //startS = (pf.y() / data->si - stsam);
// step,hspd:
   float step;
   // for display trs ,give trs+1 space of traces interval dots
   if (trs > 0) step = ((drawFrame *)view())->getScaleX();
   else step = iw;
   float ii;
   int idx, idx0;


   //inVisibleMarks();// this will make the cup 100%;
   //qDebug() << "paint fff = ";
   QPolygonF ply, plyo;
   for (i = 0,ii = startT; ii < iw && (sttr + i) <= data->trs - 1; ii += step, i++)
   {
      idx = sttr + i;
      idx0 = getPickIntpHD(idx); //offset
      ply << QPointF(reLocIndex(idx), idx0); //idx+1:  picked left of the trace;to fit with pick point;

   }
   plyo = getIntpPickTime(PICKDATA->pickPoints, ply);
   if (data->m_iDrawType == DRAW_VD)     p->setPen(QPen(Qt::black, 2));
   else p->setPen(QPen(Qt::red, 2));

   if (plyo.size() >= 0) p->drawPolyline(plyo);
//  rubber line: red,pick blue remove:
   if (xPressed >= 0)
   {
      //scene()->addItem(&m_line);
      if (m_iButton == 0) m_line->setPen(QPen(Qt::red, 2));
      else m_line->setPen(QPen(Qt::blue, 2));
      m_line->setLine(xPressed, yPressed, xMove, yMove);
      m_line->setLine(0, 0, xMove - xPressed, yMove - yPressed);
      m_line->setPos(xPressed, yPressed);

   }
   return;
}
void sectPaint::setVisiblePickMarks(bool b)
{
   QGraphicsEllipseItem *item;
   int num;
   num = PICKDATA->pickMarks.size();
   //qDebug() << "pick points = " << num;
   if (num <= 0) return;
   QMap<int, QGraphicsEllipseItem *>::const_iterator i = PICKDATA->pickMarks.constBegin();
   while (i != PICKDATA->pickMarks.constEnd())
   {
      item = i.value();

      item->setVisible(b);
      //qDebug() << "invisible =" << b;

      //item->setPos(-5 * PICK_ITEM_WIDE, -1 * PICK_ITEM_WIDE);
      i++;
   }
}
// ensure: idx00 <idx01
void sectPaint::addPickPoint(int idx00, float time0, int idx01, float time1)
{
   //qDebug() << "add 2:" << idx00 << time0 << idx01 << time1;
   removePickPoint(idx00, idx01); // not include the eadge
   addPickPoint(idx00, time0);
   addPickPoint(idx01, time1);
   return;

}
void sectPaint::addPickPointHD(float hdv, float time)
{
   int i;
   for (i = 0; i < data->trs; i++)
   {
      if (hdv == data->displayHeader[i]) break;
   }
   addPickPoint(i, time);

}
void sectPaint::addPickPoint(int idx0, float time0) //index
{
   if (idx0 < 0 || idx0 >= data->trs) return;
   int idx;
   float time;
   idx = getPickIntpHD(idx0);//offset:
   time = data->adjustPick(idx0, time0);
   //qDebug() << "t0,t =" << time0 << time;

   QGraphicsEllipseItem *item;
   //idx = idx +1;//+1 picked left of the trace;

   QMap<int, QGraphicsEllipseItem *>::const_iterator i = PICKDATA->pickMarks.constFind(idx);
   //qDebug() << "const add = " << i;
   PICKDATA->pickPoints[idx] = time;
   if (i == PICKDATA->pickMarks.constEnd()) // no this point;
   {
      item = new QGraphicsEllipseItem();
      PICKDATA->pickMarks[idx] = item;
      scene()->addItem(item);
      item->setPen(QPen(Qt::black, 2));
      item->setZValue(Z_TOP_1);
      item->setFlag(QGraphicsItem::ItemIgnoresTransformations);
      //qDebug() << "item in addMark="  << item;
      item->setRect(-1 * PICK_ITEM_WIDE / 2, -1 * PICK_ITEM_WIDE / 2, PICK_ITEM_WIDE, PICK_ITEM_WIDE);
   }
   else // already picked the pointl
   {
      item = PICKDATA->pickMarks[idx];
   }
   item->setPos(reLocIndex(idx0), time0); //+1 picked left of the trace;
}

void sectPaint::clearPickPoints()
{
   int i, num;
   QList<int> klist;
   num = PICKDATA->pickPoints.size();
   if (num <= 0) return;
   klist = PICKDATA->pickPoints.keys();
   for (i = 0; i < num; i++)
   {
      removePickPoint(klist[i]);
   }
   return;

}
// ensure: idx00 <idx01
void sectPaint::removePickPoint(int idx00, int idx01) //index
{
   QList<int> klist;
   int num, i;
   int idx0, idx1, v1, v2;
   idx0 = getPickIntpHD(idx00);
   idx1 = getPickIntpHD(idx01);
   //qDebug() << "remove =" << idx0 << idx1;
   if (idx0 > idx1)
   {
      v1 = idx1;
      v2 = idx0;
   }
   else
   {
      v1 = idx0;
      v2 = idx1;
   }
   //qDebug() << "remove v1,v2=" << v1 << v2;
   num = PICKDATA->pickPoints.size();
   if (num <= 0) return;
   klist = PICKDATA->pickPoints.keys();
   for (i = 0; i < num; i++)
   {
      //qDebug() << "point = " << klist[i];
      if (klist[i] > v1 && klist[i] < v2)
      {
         removePickPoint(klist[i]);
      }
   }
   return;
}


void sectPaint::removePickPoint(int idx0) // not index
{

   QGraphicsEllipseItem *item;

   if (isPickPoint(idx0))
   {
      PICKDATA->pickPoints.remove(idx0);
      item = PICKDATA->pickMarks[idx0];
      PICKDATA->pickMarks.remove(idx0);
      if (item != NULL) delete item;
   }
}
bool sectPaint::isPickPoint(int idx0)
{
   int num;
   QMap<int, float>::const_iterator i = PICKDATA->pickPoints.constBegin();
   num = PICKDATA->pickPoints.size();
   //qDebug() << "pick points = " << num;
   if (num <= 0) return false;
   while (i != PICKDATA->pickPoints.constEnd())
   {
      //qDebug() << "333";
      if (i.key() == idx0) return true;
      i++;
   }
   return false;

}
//==========================================================
float sectPaint::getPickTime(int idx0)
{
   if (idx0 < 0 || idx0 >= data->trs) return 0;
   int idx;
   idx = reLocIndex(idx0);
   int num;
   num = PICKDATA->pickMarks.size();
   //qDebug() << "pick points = " << num;
   if (num <= 0) return 0;

   int id0, id1, len;
   float in[2];
   float f[2], *ff, fr;
   id1 = -1;
   id0 = -1;
   len = idx + 10;
   QMap<int, float>::const_iterator i = PICKDATA->pickPoints.constBegin();
   while (i != PICKDATA->pickPoints.constEnd())
   {
      if (i.key() >= idx)
      {
         id1 = i.key();
         break;
      }
      else
      {
         id0 = i.key();
      }
      i++;
   }
   if (id0 == -1 && id1 == -1) return 0; // no pick
   if (id0 == -1 && id1  >= 0) return PICKDATA->pickPoints[id1]; // left
   if (id0 >= 0 && id1  == -1) return PICKDATA->pickPoints[id0]; //right
                                                                 // interplation:
   in[0] = id0;
   in[1] = id1;
   f[0] = PICKDATA->pickPoints[id0];
   f[1] = PICKDATA->pickPoints[id1];

   //linearInterpolation( int numSamplesIn, float const* xin, float const* yin,
   //                     int numSamplesOut, float sampleIntOut, float* yout )
   ff = new float[len];
   //qDebug() << "getTime0=" << in[0] << in[1] << f[0] << f[1] << idx << id0;
   data->linearInterpolation(2, in, f, len, 1, ff);
   //qDebug() << "getTime=" << ff[idx];
   fr = ff[idx];
   delete []ff;
   return fr;

}
// plotPick
QPolygonF sectPaint::getIntpPickTime(QMap<int, float> knee, QPolygonF p)//p.x :idx+1,py :offset
{
   int numIn, numOut, i;
   QPolygonF ret1, ret;
   QPointF pf;
   float time;
   // get max of p.y()
   float fmax;
   fmax = 0;
   for (i = 0; i < p.size(); i++) if (fmax < p[i].y()) fmax = p[i].y();


   numIn = knee.size();
   if (numIn <= 0)
   {

      // get the intp time that application set;
      ret1 = PICKDATA->m_intpPolyTimes;
      if (ret1.size() <= 0) return ret1;

      numIn = fmax;
      // qDebug() << "intp fmax =" << fmax << ret1.size() << p.size();
      for (i = 0; i < p.size(); i++)
      {
         //ret << scene2Item(p[i].x(), ret1[p[i].y()].y());
         //qDebug() << "inloop =" << i << p[i].x() << p[i].y();
         time = data->adjustPick(p[i].x()-1, ret1[p[i].y()].y());
         //time =ret1[p[i].y()].y();

         pf = scene2Item(p[i].x(), time); //[offset,time]
         ret << QPointF(pf.x(), pf.y());
      }
      //qDebug() << "intp ret =" << ret.size();
      return ret;
   }
   if (p.size() <= 0) return ret;

   numIn = fmax;

   ret1 = getIntpPickTime(knee, numIn + 1);
   if (ret1.size() <= 0) return ret1;


   numOut = numIn;
   for (i = 0; i < p.size(); i++)
   {
      time = data->adjustPick(p[i].x()-1, ret1[p[i].y()].y());
     //time =   ret1[p[i].y()].y();
      pf = scene2Item(p[i].x(), time); //[offset,time]

      ret << QPointF(pf.x(), pf.y());
   }
   return ret;
}
QPolygonF sectPaint::getIntpPickTime(QMap<int, float> knee, int num)
{

   int numIn, numOut, i;
   QList<int> xlist;
   QPolygonF ret;

   numIn = knee.size();
   if (numIn <= 0) return ret;

   xlist = knee.keys();
   numOut = num;
   float *xin, *yin, *xout, *yout;
   xin = new float[numIn];
   yin = new float[numIn];
   xout = new float[numOut + 10];
   yout = new float[numOut + 10];
   for (i = 0; i < numIn; i++)
   {
      xin[i] = xlist[i];
      yin[i] = knee[xlist[i]];
   }
   for (i = 0; i < numOut; i++)
   {
      xout[i] = i;
   }

   //linearInterpolation( int numSamplesIn, float const* xin, float const* yin,
   //                     int numSamplesOut, float sampleIntOut, float* yout )

   data->linearInterpolation(numIn, xin, yin, numOut, 1, yout);

   ret.clear();
   for (i = 0; i < numOut; i++)
   {
      ret << QPointF(xout[i], yout[i]);
   }
   return ret;
}

int sectPaint::getPickIntpHD(int idx)
{

   if (data->displayHeader == NULL)
   {
      return idx + 1;
   }
   return data->displayHeader[idx];
}
int sectPaint::reLocIndex(int idx)
{
   return idx + 1;
}
