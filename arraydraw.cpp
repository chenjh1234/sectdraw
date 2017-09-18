#include "arraydraw.h"
#include <QScrollBar>

arrayDraw::arrayDraw(QWidget *w):drawFrame(w)
{
    data = new arrayData();
    setData(data);

    m_paint = new arrayPaint();
    setDrawing(m_paint);
    m_paint->setData(data);

    drawFrame::init();
    init();

}

arrayDraw::~arrayDraw()
{
      qDebug() << "array draw  " ;
    if(!data->m_drawMAP.empty())
    {
        QStringList keyList;
        keyList = data->m_drawMAP.keys();
        qDebug() << "array draw key " << keyList;
        int len,i;
        len = keyList.size();
        for(i =0; i <len; i++)
            deleteDraw(keyList[i]);
    }
}
void arrayDraw::init()
{

    sc->addItem(m_drawing);
    setDrawStyle();
    //setFitWin();
}
/*
void arrayDraw::scrollContentsBy ( int dx, int dy )
{
    drawFrame::scrollContentsBy(dx,dy);
     if (!data->m_bStart) return;

    //posRuler();

    //m_draw->setPos(mapToScene(data->m_ruler.leftW*2,data->m_ruler.topW*2));
}
*/
void arrayDraw::resizeEvent ( QResizeEvent * event )
{
    drawFrame::resizeEvent (event);
    reDraw();
    if (!data->m_bStart) return;


}
void arrayDraw::mousePressEvent(QMouseEvent *e)
{
   drawFrame::mousePressEvent(e);
  

   int x, y, tr, isam, si, sam;
   QString str;

   x = e->x();
   y = e->y();

   tr = mapToScene(x, y).x();
   isam  = mapToScene(x, y).y(); //is the time
   
#if 1
    
   //str.sprintf("%d,%d,%d,%d", x,y,tr, isam);
   //qDebug() << "data buf = " << data->buf;
   //qDebug() << "color = " << m_paint->img->bits()[34]<< m_paint->img->bits()[35];

   //setBottomMessage(str);
#endif
 
}

/**
Qt::NoBrush
Qt::SolidPattern
Qt::Dense1Pattern
Qt::Dense2Pattern
Qt::Dense3Pattern
Qt::Dense4Pattern
Qt::Dense5Pattern
Qt::Dense6Pattern
Qt::Dense7Pattern
Qt::HorPattern
Qt::VerPattern
Qt::CrossPattern
Qt::BDiagPattern
Qt::FDiagPattern
Qt::DiagCrossPattern
Qt::CustomPattern
*/
int arrayDraw::startDraw(qreal *buf,int len,QString key)
{
    float *fbuf;// it is worry
    fbuf = (float*)buf;
    startDraw(fbuf,len,key);
}
int arrayDraw::startDraw(float *buf,int len,QString key)
{
    data->m_bStart = true;
    //int i;
    data->buf = buf;
    data->m_iDrawLen = len;

    //qDebug() << "start------"<< len << key;
    deleteDraw(key);
     //qDebug()<<"start1------";
    reDrawStyle(key);
     //qDebug() << "start2------";

    return 0;
}
int arrayDraw::endDraw ()
{
    data->m_bStart = false;
}
void arrayDraw::setLabX(qreal *x,int len)
{
    data->setLabX(x,len);
}

int arrayDraw::setDrawStyle(int type,int c,int wide,int fill,int b)

{
    //qDebug() << "style =" << b<<data->getPenColor(b);
    return setDrawStyle(type,data->getPenColor(c),wide,fill,data->getPenColor(b));
}

int arrayDraw::setDrawStyle(int type,QColor c,int wide,int fill,QColor bc)

{
    //qDebug() << c;
    return data->setDrawStyle(type,c,wide,fill,bc);

}
int arrayDraw::reDrawStyle(QString key )
{
        data->m_draw = data->getDraw();
        data->saveDrawToMAP(key);
        return 0;
}
 
int arrayDraw::deleteDraw(QString key)
{
        arrayData::M_DRAWMAP::iterator it;

        it = data->m_drawMAP.find(key);
        if(it != data->m_drawMAP.end())
        {
            //this is callers data,can we release it??????
                //delete []it.value().buf;
                data->m_drawMAP.remove(key);
        }

        return 0;
}
void arrayDraw::setDrawing(drawing *d)
{
    m_drawing = d;
}

void arrayDraw::setData(drawData *d)
{
    drawFrame::data = d;
}
