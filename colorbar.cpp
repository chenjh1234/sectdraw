#include "colorbar.h"
//#include "drawframe.h"
#include <QDebug>
colorBar::colorBar(QGraphicsItem  *parent) :
                   drawItem(parent)
{
    //qDebug() << "i am in construct of color bar ";
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    //setMouseTracking ( true);

    //setFlag(QGraphicsItem::ItemIsMovable);
    //setSelected(true);
    init();
}
void colorBar::init()
{

        m_color = new QColor[BAR_COLORS];
        setVertical(false);
        x0 = 0;
        y0 = 0;
        dx = 0;
        dy = 0;
        df = 0;
        m_iColorTBIdx = 0;
        m_iIdx = 0;
        setDisplay(true);
        setMove(true);
        setColor(0);
        iw = 0;
        ih = 0;
        m_keyControl = false;
        m_keyShift = false;
        setSize(10,10);
        m_iMaps =COLOR_TABLES;
        m_names = new QString[m_iMaps];
        m_iSelect = 0;
        int i ;
        i = 0;
        m_names[i]= "black_white";i++;
        m_names[i]= "white_black";i++;
        m_names[i]= "white_black";i++;
        m_names[i]= "white_black";i++;
        m_names[i]= "white_black";i++;
        m_names[i]= "white_black";i++;
        m_names[i]= "white_black";i++;
        m_names[i]= "white_black";i++;
        m_names[i]= "white_black";i++;
        m_names[i]= "white_black";i++;//10

        m_names[i]= "white_black";i++;
        m_names[i]= "white_black";i++;
        m_names[i]= "white_black";i++;


}
QPainterPath colorBar::shape() const
{
    //qDebug() << "i am in shape of color bar ";
     QPainterPath path;
     path.addRect(boundingRect());
     return path;
}
QRectF colorBar::boundingRect()const
{
    //qDebug() << "i am in bounding of color bar ";
    qreal penWidth = 0;
    int h,w;
   // h = data->m_ruler.bottomW/2 ;
   // w = (data->m_frame.width)/2;
    w = iw;
    h = ih;

    QRectF f;

    f = QRectF(0- penWidth / 2   ,
               0 - penWidth / 2  ,
              // w /m_sx+ penWidth, h /m_sy+ penWidth);
                w + penWidth, h + penWidth);


    return f;
}
void colorBar::paint(QPainter *p, const QStyleOptionGraphicsItem *op,
             QWidget *ww)
{
    //QStyleOptionGraphicsItem *op1;
    //QWidget *ww1;
    //ww1 = ww;
    //op1  = (QStyleOptionGraphicsItem *)op;

    plot(p);
    if(m_iSelect)
    {
      // p->drawRect(boundingRect().x()+1,boundingRect().y()+1,
       //           iw-2,ih-2);
      // p->drawRect(boundingRect().x(),boundingRect().y(),
       //          iw,ih);
    }


}
void colorBar::setColorSlot(int idx)
{
     setColor( idx);
     view()->scene()->update();
     return;
}
int colorBar::getColor()
{
        return m_iColorTBIdx;
}
int colorBar::setColor(int idx)
{
        int i,len;
        len = BAR_COLORS;

        m_iColorIdxMax = COLOR_TABLES;
        m_iColorTBIdx = idx;


        for(i = 0; i <len; i++)
        {

                switch(idx)
                {
                case 01://black--->white
                        m_color[i].setRgb( i,i,i);
                        break;
                case 02://white   black
                        m_color[i].setRgb(len-1- i,len-1 - i,len-1 - i);

                        break;
                case 03://blue white
                        m_color[i].setRgb( i,i,255);// g->b
                        break;
                case 04://yellow blue
                        m_color[i].setRgb( 255-i,255-i,i);
                        break;
                case 05://purple green
                        m_color[i].setRgb( 255-i,i,255-i);
                        break;
                case 06://black blue
                        m_color[i].setRgb( 0,0,i);
                        break;
                case 07://cyan red
                        m_color[i].setRgb( i,255-i,255-i);
                        break;
                //case 08://cyan red
                        m_color[i].setRgb( i,255-i,0);
                //	m_color[i].setRgb( i,i,i);
                        break;
                }

        }
        int ilen ,j,id ;
        int idd;
        idd = 1;
        id = 6;
        ilen = len/id;;
//-----------------------------------for sea default
        /*
        colors[0] = Color.cyan;
           colors[1] = Color.black;
           colors[2] = Color.white;
           colors[3] = Color.red;
           colors[4] = Color.yellow;
           */
        //idd = 1;
        //id = 6;
        //ilen = len/id;;
//-----------------------------------9:blue green cyan purplr yellow red
        if(idx == 9)
        for(i = 0; i <len; i++)
        {
                j = i/ilen;
                if(j == 0)
                        m_color[i].setRgb(200- i*id >0?200- i*id :0
                                ,200 - i*id>0?200- i*id :0
                                ,255/idd*idd );

                if(j == 1)
                        m_color[i].setRgb(0,(i-j*ilen)*id/idd*idd,0);
                if(j == 2)
                        m_color[i].setRgb(0,(i-j*ilen)*id/idd*idd,(i-j*ilen)*id/idd*idd);
                if(j == 3)
                        m_color[i].setRgb((i-j*ilen)*id/idd*idd,0,(i-j*ilen)*id/idd*idd);
                if(j == 4)
                        m_color[i].setRgb((i-j*ilen)*id/idd*idd,(i-j*ilen)*id/idd*idd,0);
                if(j == 5)
                        m_color[i].setRgb((i-j*ilen)*id/idd*idd,0,0);
                if(j == 6)
                        m_color[i].setRgb(255,0,0);
        }
        //b->w->r
        id = 2;
        ilen = len/id;
//--------------------------------------0:blue white red
        if(idx == 0)
        for(i = 0; i <len; i++)
        {
                j = i/ilen;
                if(j == 0)
                        m_color[i].setRgb(i*id-1<0?0:i*id-1,i*id-1<0?0:i*id-1,len-1);
                if(j == 1)
                        m_color[i].setRgb(len-1 ,len - id*(i-ilen)-1,
                                                 len - id*(i-ilen)-1);
        }
        //b->w->r
        id = 2;
        ilen = len/id;
//---------------------------------------10:blue gray red
        if(idx == 10)
        for(i = 0; i <len; i++)
        {
                j = i/ilen;
                if(j == 0)
                        m_color[i].setRgb(i,i,len-i-1);
                if(j == 1)
                        m_color[i].setRgb(len - len+i ,len -i -1,
                                                 len - i-1);
        }
//-------------------11,12:16-4 colorful from GBSYSrectangle
        int r,g,b,scl;
        int ii,ic;
        //NL = 4;

        scl =64;
        if(idx == 11 || idx == 12 )
        for(i = 0; i <len ; i++)
        {
                ii = i/4;
                ic = (i - ii*4)/2;
                if(idx == 11 || ii >= (len-1)/4)// for 11 or 12 upper end
                        ic = 0;

                i2rgb(ii +ic,&r,&b,&g);
                m_color[i].setRgb(r*scl,g*scl,b*scl);

    }

        //repaint();
        return 0;
}
int colorBar::plot(QPainter *p)
{
    int w,h,len;
    int i,idx,hh;
    QPen pen;
    QString str;

    w = iw;
    h = ih;
    len = BAR_COLORS;

    if( ih <=0 || iw <0 ) return -1;

    df = (float) len /(float)w;
    int x1,y1,x2,y2,ddx; //,ddy;

    hh = 10;
    x1 = hh;
    y1 = 0;
    y2 =0;
    x2 = iw-1;
    ddx = dx;
    //ddy = dy;
    if(! m_bVertical)
    {
        //ddy = ddx;
        y1 = x1;
        y2 = h-1;
        h = w;
    }
    else
    {

    }
// based on horizon:
    //qDebug()<< "this is in plot()\n";
    for(i = 0; i < h; i++)
    {
            //qDebug()<< i<< "this is in plot() 0\n";
        if(! m_bVertical)
        {
            x1 = i;
            x2 = i;
        }
        else
        {
            y1 = i;
            y2 = y1;
        }
        //idx = dxIdx2Idx(i*df,ddy*df);
        idx = dx2Idx(i);


//	idx =  df * (i -ddy);
//	if(idx >= len) idx = idx - len;
//	if(idx < 0) idx =  len + idx;
//	qDebug("i = %d,idx = %d,dy = %d,df = %f\n",i,idx,dy,df);

        pen.setColor(m_color[idx]);

        p->setPen(pen);
        p->drawLine(x1,y1,x2,y2);
    }
    //qDebug() << "1111111111111111";
    pen.setColor(m_color[128]);
    p->setPen(pen);
    if(m_bVertical)
    {
        p->drawLine(0,h/2,w,h/2);

    }
    else
    {
        p->drawLine(w/2,hh,w/2,h);
        str.sprintf("%d",m_iIdx);
        p->drawText(0,hh,str);
        str.sprintf("%d,%d-%d,%d",x0,y0,dx,dy);
        p->drawText(w/2,hh,str);

    }

    return 0;

}
void colorBar::i2rgb(int index,int *ri,int *bi,int *gi)
{
        int bm,gm;
        int r,g,b;
        int NL = 4;

        gm=NL*NL;
    bm=NL;
    //rm=1;

    g=index/gm;
    b=(index-g*gm)/bm;
    r=index-g*gm-b*bm;
    *ri=r;*bi=b;*gi=g;
}
int colorBar::dx2Idx(int i)
{
    //df:  colors per pixal in the color bar
    // dx£¬dy: pixals from th orignal(0,0);
    //dxidx: colors from the orignal (0,0) of dx
    //    return df*dy; //find idx
    // i: pixal index of the bar:
    int idx,len,dxidx;

    if(! m_bVertical)
        dxidx = df*dx;
    else
        dxidx = df*dy;

    idx = i*df - dxidx;

    len = BAR_COLORS;
    if(idx >= len) idx = idx - len;
    if(idx < 0) idx =  len + idx;
    return idx ;
}
/*
int colorBar::dxIdx2Idx(int i,int dxidx)
{
        int idx,len;
        idx = i - dxidx;
        len = BAR_COLORS;
        if(idx >= len) idx = idx - len;
        if(idx < 0) idx =  len + idx;

        return idx ;
}
*/

int colorBar::setDisplay(bool b)
{
        m_bDisplay = b;
        return 0;
}

int colorBar::setMove(bool b)
{
        m_bMove = b;
        return 0;
}

int colorBar::getColorIndex()
{
        return m_iIdx;
}

int colorBar::getColorTableIndex()
{
        return m_iColorTBIdx;
}
int colorBar::setVertical(bool b)
{
        m_bVertical = b;
        return 0;
}

void colorBar::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * e )
{

   if(e->modifiers()== Qt::ControlModifier)
   {
        if(e->button() == Qt::LeftButton )
        {
            dy = 0;
            dx = 0;
            m_iIdx = 0;
            update();
            view()->scene()->update();

        }
    }

}

void colorBar::mouseMoveEvent ( QGraphicsSceneMouseEvent * e )
{
    int x,y;
    QGraphicsItem::mouseMoveEvent(e);

    x = e->pos().x();
    y = e->pos().y();
    if(!m_bDisplay) return;
    if(!m_bMove) return;

/**
     when mouse button pressed and mouse move go this function ,
     because no focus ,so mouse move grabed by drawFrom.(when no buttom pressed)
     setMouseTracking ( bool enable ), works to enable in
*/
    if(e->modifiers()== Qt::ShiftModifier && e->buttons() == Qt::LeftButton)
    {
            dx = x - x0;
            dy = y - y0;
            if( abs(dx) >= iw || abs(dy) >= ih )
            {
                    dx = 0;
                    x0 = x;
                    dy = 0;
                    y0 = y;
            }
/*
            if(m_bVertical)
            {
                    m_iIdx = dx2Idx(dx);
            }
            else
            {
                    m_iIdx = dx2Idx(dy);
            }
            */
            //2013.9
            m_iIdx = dx2Idx(0);

            update();
            view()->scene()->update();
    }

}

void colorBar::mousePressEvent ( QGraphicsSceneMouseEvent * e )
{
   // QWidget::mousePressEvent(e);
    x0 = e->pos().x()-dx;
    y0 = e->pos().y()-dy;

    setColor(m_iColorTBIdx);

    if(e->modifiers()== Qt::ControlModifier)
    {
            if(e->button() == Qt::LeftButton )
            {
                    m_iColorTBIdx ++;
                    if(m_iColorTBIdx == m_iColorIdxMax) m_iColorTBIdx = 0;
                    setColor(m_iColorTBIdx);
 //                   qDebug() << "Left button down\n";

            }

            if(e->button() == Qt::RightButton )
            {
                    m_iColorTBIdx --;
                    if(m_iColorTBIdx < 0) m_iColorTBIdx = m_iColorIdxMax -1;
                    setColor(m_iColorTBIdx);
                    qDebug() << "Right button down\n";

            }
            update();
            view()->scene()->update();
    }

}
void colorBar::setSize ( int w,int h)
{
   ih = h;
   iw = w;
   return ;
}
QVariant  colorBar::itemChange ( GraphicsItemChange change, const QVariant & value )
{
    //int i;
    if (change == ItemSelectedChange)
    {
             // value is the new position.
             //i = 0;

    }
    if(isSelected())
    {
        m_iSelect = 1;
        //emit clicked();
        update();
    }
    else
    {
        m_iSelect = 0;
        update();
    }
         return QGraphicsItem::itemChange(change, value);
}
/**
void colorBar::mouseReleaseEvent ( QGraphicsSceneMouseEvent * e)
{

    //int i;
    if(e->button() == Qt::LeftButton )
    {

        //qDebug() << "Left button\n";
        //works
    }
    if(e->button() == Qt::RightButton )
    {

        //qDebug() << "Right button\n";
        //works
    }
}

void colorBar::keyPressEvent ( QKeyEvent * e)
{
    //just test in fact key is no use in this item
    //setFlag(QGraphicsItem::ItemIsFocusable);
    //items will accept key
    //
    if(e->key() == Qt::Key_Shift)
        m_keyShift = true;
        //qDebug() << "key Shift\n";
    if(e->key() == Qt::Key_Control)
        m_keyControl = true;
        //qDebug() << "key Control\n";
}

void colorBar::keyReleaseEvent ( QKeyEvent * e)
{
    if(e->key() == Qt::Key_Shift)
 *      m_keyShift = false; //qDebug() << "key Shift
 *      released\n";
    if(e->key() == Qt::Key_Control)
        m_keyControl = false;

       // qDebug() << "key Contral released\n";
}
*/


