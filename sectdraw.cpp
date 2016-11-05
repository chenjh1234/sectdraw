// sectDraw.cpp: implementation of the sectDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "sectdraw.h"
#include <QMenu>
#include <QScrollBar>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

sectDraw::sectDraw(QWidget *p) : drawFrame(p)
{
//create data:
   data = new sectData();
   setData(data);

   m_paint = new sectPaint();
   setDrawing(m_paint);
   m_paint->setData(data);

   drawFrame::init();
   init();
}

sectDraw::~sectDraw()
{

}

void sectDraw::init()
{
   sc->addItem(m_drawing);
   cb = m_colorBar;
   m_paint->cb = cb;
   m_paint->img = NULL;
   data->m_bTranspalency = false;
   //m_comDrawing =  NULL;
   m_comView =  NULL;

   bMovedBy = true;
   maxTrs = 0;
   bRedraw = false;
   //m_bSetColor = FALSE;

   //setTransformationAnchor(QGraphicsView::NoAnchor);
   //setTransformationAnchor(QGraphicsView::AnchorViewCenter);
   //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

   setCoordinate(RIGHT_DOWN);
   //setGrid(5,50,5,50);
   setGrid(5, 50, 10, 100);
   setDrawType(DRAW_VD);
   // setDrawType(DRAW_WA);
//    data->m_iVDColorType = 0;
   setShowHeader(false);
   setGain(0);



   //setDrawType(DRAW_WA);
   //setFocusPolicy(QWidget::ClickFocus);
   //setFocus();

   //setVScrollBarMode(AlwaysOff);//(Auto);
   //setHScrollBarMode(Auto);
//	SetScale(0,0);

   //connect(  m_colorBar, SIGNAL(mousemoved(int)), this,SLOT(setColor(int)) );
   //connect( (QObject*)cb, SIGNAL(mousepressed(int)), this,SLOT(setPressedColor(int)) );
   connect(cmap, SIGNAL(sig_apply(int)), this, SLOT(setColor(int)));
}
int  sectDraw::setColor(int dx)
{
   cb->setColorSlot(dx);
   // m_paint->setImageColor();
   m_ic = 0;
   //m_bSetColor = TRUE;
   return 0;
}
void sectDraw::setDisplayHeader(float *f)
{
   int i, trs;
   if (f == NULL)
   {
      qDebug() << "sectDraw::setDisplayHeader(float *f) f=NULL";
      return;
   }
 
   data->displayHeader = f;
   float fmax;
   trs = data->trs;
   fmax = -100;
   for (i = 0; i < trs; i++)
   {
      if (fmax < f[i]) fmax = f[i];
   }
   data->m_MaxDisplayHeader = fmax;
}
float sectDraw::getRMS()
{
   return data->getRMS();
}
void sectDraw::setRMS(float rms)
{
   if (rms > RMS_MAX) rms = RMS_MAX;
   if (rms < RMS_MIN) rms = RMS_MIN;
   data->setRMS(rms);
   reDraw();
}
int sectDraw::startDraw()
{
   int *hd,trs,sam,headLen; 
   float *fdata,si; 
   if (!data->m_bStart) return -1;
    
   si = data->si ;
   headLen = data->headLen;

// get parameters:

   hd = data->head ;
   fdata = data->obuf;
   sam = data->sam;
   trs = data->trs;
   //bRedraw = true;
   //qDebug()<<"startDraw() bg";
   return startDraw(hd, fdata, trs, sam, si, headLen);

}
int sectDraw::startDraw(int *hd, float *fdata, int trs)
{
   int sam, headLen;
   float si;

   si = data->si;
   headLen = data->headLen;
   sam = data->sam;
   //qDebug() << "in sectDraw:" << hd << fdata << trs <<sam <<si <<headLen;
   if (si <= 0 || sam <= 0 || headLen <= 0) return -1;

   return startDraw(hd, fdata, trs, sam, si, headLen);
}
int sectDraw::setDataPara(int sam, int si, int headlen)
{
   data->si = si;
   data->headLen = headlen;
   data->sam = sam;
   return 0;
}
int sectDraw::startDraw(int *hd, float *fdata, int trs, int sam, float si, int headLen)
{
    //qDebug()<<"startDraw(6p) start";
   if (si <= 0 || sam <= 0 || headLen <= 0 || hd == NULL || fdata == NULL) return -1;

   data->m_bStart = true;
   //SetDataPrecision(0,0);
   //SetRulerIntv(1,5,100,500);
   //SetGrid(1);
   //qDebug()<<"startDraw(6p) start1" << trs << maxTrs;
   if (trs > maxTrs)
   {
      if (data->buf != NULL)
         delete[] data->buf;
      data->buf = new float[trs*sam+10];
      maxTrs = trs;
   }
   //qDebug()<<"startDraw(6p) start2 " << trs << maxTrs;

   memcpy(data->buf, fdata, trs * sam*sizeof(float));
   //bRedraw = false;

   data->si = si;
   data->headLen = headLen;
   //qDebug()<<"startDraw(6p) start3 " << trs << maxTrs;

// get parameters:

   data->head = hd;
   data->obuf = fdata;
   data->sam = sam;
   data->trs = trs;
   //qDebug()<<"startDraw(6p) yes";
// get fmax fmin:
   procFilter();
   procAGC();
   data->getMaxMin();
   return 0;

}


void sectDraw::resizeEvent(QResizeEvent *e)
{
//        int w,h;

   drawFrame::resizeEvent(e);
   /*
   int iw,ih,w,h;
   iw = width();//include scroolbar
   ih = height();
   w = viewport()->width();//not include scroolbar
   h = viewport()->height();
   qDebug() << iw<<ih<<w<<h<<e->size()<<e->oldSize();
   */
   autoZ();

}
void sectDraw::setPos(int x, int y)
{
   slotMovedTo(x, y);
}
void sectDraw::getPos(int& x, int& y)
{
   x = horizontalScrollBar()->value();
   y = verticalScrollBar()->value();
}
void sectDraw::slotMovedTo(int dx, int dy)
{
   bMovedBy = false;
   //qDebug() << "in view slot";

   //scrollContentsBy(dx, dy);
   //return;
   //int vx,vy;

   //vx = horizontalScrollBar()->value();
   horizontalScrollBar()->setValue(dx);
   //horizontalScrollBar()->sliderMoved(dx);
   //vy = verticalScrollBar ()->value();
   verticalScrollBar()->setValue(dy);
   // verticalScrollBar ()->sliderMoved(dy);
   //verticalScrollBar ()->valueChanged(dy);
   //  qDebug() << vx << vy;
   //qDebug() << dx << dy;
   //qDebug() << vy + dy ;
}
void sectDraw::scrollContentsBy(int dx, int dy)
{
   int vx, vy;
   drawFrame::scrollContentsBy(dx, dy);
   //qDebug() << " scroll x,y = " << dx << dy <<this;
   vx = horizontalScrollBar()->value();
   vy = verticalScrollBar()->value();
   emit sigMovedTo(vx, vy);
   //if(bMovedBy) emit sigMovedBy(vx, vy);
   //bMovedBy = true;


}
void sectDraw::mouseMoveEvent(QMouseEvent *e)
{
   drawFrame::mouseMoveEvent(e);
   int x, y;
   x = e->x();
   y = e->y();
   //qDebug() << "move1 =" << m_comView;
   if (m_comView != NULL)
   {
      //qDebug() << "move2";
      if (getCompareMode() == COMPARE_MODE_H || getCompareMode() == COMPARE_MODE_V)
      {
         data->m_xCom = x - m_leftW;
         data->m_yCom = y - m_topW;
         reDraw();
      }
   }
//qDebug() << "move3";
   if (bShowHeader)
   {
      int tr;
      tr = mapToScene(x, y).x();
      sigShowHeader(tr); // trIdx
   }

   return;


}
void sectDraw::mouseReleaseEvent(QMouseEvent *e)
{
   drawFrame::mouseReleaseEvent(e);
   // for compare:
   if (m_comView != NULL)
   {
      //m_comView->scene()->addItem(m_comDrawing);
      //m_comDrawing->show();
      if (getCompareMode() == COMPARE_MODE_TURNOVER)
      {
         data->bDrawTurnover = false;
         reDraw();
      }
   }


}
void sectDraw::mousePressEvent(QMouseEvent *e)
{
   drawFrame::mousePressEvent(e);
   QMenu mu(this);

   int x, y, tr, isam, si, sam;
   QString str;

   x = e->x();
   y = e->y();

   tr = mapToScene(x, y).x();
   isam  = mapToScene(x, y).y(); //is the time
   sam = data->sam;
   si = data->si;
#if 1
   if (tr < 0 || tr >= data->trs || isam < 0 || isam >= sam * si) str = "";
   else str.sprintf("%d,%d,%f", tr, isam, data->buf[tr * sam + isam / si]);
   //qDebug() << "sam si = " << sam << si;
   //qDebug() << "color = " << m_paint->img->bits()[34]<< m_paint->img->bits()[35];

   setBottomMessage(str);
#endif
// for compare:
   if (m_comView != NULL)
   {
      //scene()->addItem(m_comDrawing);
      //m_comDrawing->setPos(x,y);
      // m_comDrawing->setOpacity ( 0.5 );
      //  m_comDrawing->setFlag(QGraphicsItem::ItemIsMovable);
      // m_comDrawing->setFlag(QGraphicsItem::ItemIsSelectable);
      m_paint->imgCom = ((sectPaint *)m_comView->getDrawing())->img; // point to the image of compare view

      data->m_xCom = x - m_leftW;
      data->m_yCom = y - m_topW;
      if (getCompareMode() == COMPARE_MODE_TURNOVER)
      {data->bDrawTurnover = true;
         reDraw();
      }
   }

   //cmap->show();
   //qDebug() << "from colorMap = "<< ret;

}
void sectDraw:: keyPressEvent(QKeyEvent *e)
{
   drawFrame::keyPressEvent(e);
   //int sam;
   //sam = data->sam;

   if (e->key() == Qt::Key_C)
   {
      if (data->m_iDrawType == DRAW_VD) setDrawType(DRAW_WA);
      else setDrawType(DRAW_VD);
      reDraw();
   }
}

int sectDraw::setDrawType(int type)
{
   /*
#define DRAW_VD 0
#define DRAW_WA 1
#define DRAW_WW 2
#define DRAW_AA 3
   */
   if (type == DRAW_WA && getDrawType() == DRAW_VD)
   {
      data->m_iIndexColorTBBak = cb->m_iIdx; // backup color pallet
      setColorBar(0);
      setColor(01); //black to white;
   }

   if (type == DRAW_VD && getDrawType() == DRAW_WA)
   {
      setColor(data->m_iIndexColorTBBak); //restore the last color pallet
      setColorBar(1);
   }

   return data->m_iDrawType = type;
}



int sectDraw::getDrawType()
{
   return data->m_iDrawType;
}

int sectDraw::setGain(float gain)
{
   if (gain > GAIN_MAX) gain = GAIN_MAX;
   if (gain < 0) gain = 0;
   data->m_fGain = gain;
   reDraw();
   return gain;
}

float sectDraw::getGain()
{
   return data->m_fGain;
}

int sectDraw::setGainGlobal(bool b)
{
   data->m_bGainGlobal = b;
   return 0;
}

bool sectDraw::getGainGlobal()
{
   return data->m_bGainGlobal;
}

void sectDraw::setDrawing(drawing *d)
{
   m_drawing = d;
}

void sectDraw::setData(drawData *d)
{
   drawFrame::data = d;
}

float sectDraw::user2ScaleX(float x)
{
   float xx;
   xx = dotsPerInchX  / (x * cmsPerInch); // 1 / x : CMS/unit
                                          //final scale: dots/unit = dots/trace
   return xx;
}
float sectDraw::user2ScaleY(float y)
{
   float yy;
   yy =   y * dotsPerInchY / (cmsPerInch * 1000); // 1 / x : CMS/unit
                                                  //final scale: dots/sample
   return yy;

}
void sectDraw::user2Scale(float x, float y, float& xx, float& yy)
{
   xx = user2ScaleX(x);
   yy = user2ScaleY(y);

}
void sectDraw::setUserScale(qreal x, qreal y)
{
   float sx, sy;

   user2Scale(x, y, sx, sy);
   setScale(sx, sy);


}
void sectDraw::getUserScale(float& x, float& y)
{
   float sx, sy;
   getScale(sx, sy);
   // xx = dotsPerInchX  / (x * cmsPerInch); // 1 / x : CMS/unit
   //yy =   y * dotsPerInchY / (cmsPerInch * 1000); // 1 / x : CMS/unit
   x =  dotsPerInchX  / (sx * cmsPerInch);
   y = sy * cmsPerInch * 1000 / dotsPerInchY;
}
drawing* sectDraw::getDrawing()
{
   return m_drawing;
}
void sectDraw::setCompareMode(int mode)
{
   data->m_compareMode = mode;
}
int sectDraw::getCompareMode()
{
   return data->m_compareMode;
}
void sectDraw::setShowHeader(bool b)
{
   bShowHeader = b;
}
void sectDraw::setPickMode(int m)
{
   if (m == PICK_MODE_NO)
   {
      data->m_pickMode = data->PICK_NO;
      m_paint->setVisiblePickMarks(false);
   }
   if (m == PICK_MODE_LAYER)
   {
      data->m_pickMode = data->PICK_LAYER;
      m_paint->setVisiblePickMarks(true);
   }
   if (m == PICK_MODE_MUTE)
   {
      data->m_pickMode = data->PICK_MUTE;
      m_paint->setVisiblePickMarks(false);
   }
   reDraw();
}
int sectDraw::getPickMode()
{
   return data->m_pickMode;
}
void sectDraw::setPickTo(int m)
{
   if (m == PICK_TO_NO)
   {
      data->m_pickTo = data->PICK_NOW;
   }
   if (m == PICK_TO_PEAK)
   {
      data->m_pickTo = data->PICK_PEAK;
   }
   if (m == PICK_TO_VALLEY)
   {
      data->m_pickTo = data->PICK_VALLEY;
   }
   if (m == PICK_TO_CROSS)
   {
      data->m_pickTo = data->PICK_CROSS;
   }
   reDraw();
}
int sectDraw::getPickTo()
{
   return data->m_pickTo;
}

void sectDraw::setAGC(int len)
{
   data->m_iAGCLen = len;
}
int sectDraw::getAGC()
{
   return data->m_iAGCLen;
}
void sectDraw::setFilter(float f1, float f2, float f3, float f4)
{
   data->m_f1 = f1;
   data->m_f2 = f2;
   data->m_f3 = f3;
   data->m_f4 = f4;
}
void sectDraw::getFilter(float& f1, float& f2, float& f3, float& f4)
{
   f1 = data->m_f1;
   f2 = data->m_f2;
   f3 = data->m_f3;
   f4 = data->m_f4;
}
void sectDraw::procFilter()
{
    int tr,i,trs,sam;
    float *buf;
    trs = data->trs;
    sam = data->sam;
   
    for (i = 0; i <trs;i++)
    { 
        buf = data->buf + i*sam;
        data->doFilter(buf, sam, data->si, data->m_f1, data->m_f2, data->m_f3, data->m_f4);
    }
    
}
void sectDraw::procAGC()
{
    if (data->m_iAGCLen <=0 ) return;
    if (data->m_iAGCLen / data->si <= 1) return;
    if (data->m_iAGCLen / data->si >= data->sam) return;

    int tr,i,trs,sam;
    float *buf;
    trs = data->trs;
    sam = data->sam;
   
    for (i = 0; i <trs;i++)
    {
        buf = data->buf + i*sam;
        data->doAGC(buf, sam, data->m_iAGCLen/data->si); 
    }
}
