#include "drawdata.h"

drawData::drawData()
{
    init();
}
void drawData::init()
{
   m_frame.height =0;
   m_frame.width =0;

   m_ruler.bottomW =40;
   m_ruler.topW =40;
   m_ruler.leftW =40;
   m_ruler.rightW =40;
   //m_ruler.step1 =5;
   //m_ruler.step2 =50;

   m_scene.x =0;
   m_scene.y =0;
   m_scene.height =0;
   m_scene.width =0;

   minX = minY =0;
   maxX = maxY =100;
   m_bStart = false;
   m_bOutlook = false;
   m_bOutlookNO = false;
   m_xygrid.grid =false;
   m_xygrid.x1 = 0;//scene data step of the grid
   m_xygrid.x2 = 0;
   m_xygrid.y1 = 0;
   m_xygrid.y2 = 0;

   m_xygrid.ix1 = 5;//pixal step of the gridx
   m_xygrid.ix2 = 50;
   m_xygrid.iy1 = 5;
   m_xygrid.iy2 = 50;

   //m_fScaleX =1;
   //m_fScaleY =1;
   //m_fDataScale =1;

   m_coordinate = RIGHT_DOWN;

}
QString drawData::printStr(int i,int len)
{
        int j;
        j = len;
        QString str;
        str.sprintf("%d",i);
        return str;
}

QString drawData::printStr(float f,int len)
{
        QString str;
        switch(len)
        {
        case 0:
                str.sprintf("%.0f",f);
                break;
        case 1:
                str.sprintf("%.1f",f);
                break;
        case 2:
                str.sprintf("%.2F",f);
                break;
        case 3:
                str.sprintf("%.3f",f);
                break;
        case 4:
                str.sprintf("%.4f",f);
                break;
        case 5:
                str.sprintf("%.5f",f);
                break;
        default:
                str.sprintf("%.6f",f);
                break;
        }
//	str.sprintf("%g",f);
        return str;
}
void drawData::setDataPrecision(int x, int y)
{
        if(y <0)
        {
                if(x >=0)
                {
                        m_iDataPrecision = x;
                        m_iDataPrecisionX = x;
                        m_iDataPrecisionY = x;
                }
        }
        else
        {
                if( x>=0 )
                {
                        m_iDataPrecisionX = x;
                        m_iDataPrecisionY = y;
                }
        }
}
Qt::BrushStyle drawData::getBrushStyle(int i)
{

        Qt::BrushStyle s;

        switch(i)
        {
        case 0:
                s = Qt::NoBrush;
                break;
        case 1:
                s = Qt::SolidPattern;
                break;
        case 2:
                s = Qt::Dense1Pattern;
                break;
        case 3:
                s = Qt::Dense2Pattern;
                break;
        case 4:
                s = Qt::Dense3Pattern;
                break;
        case 5:
                s = Qt::Dense4Pattern;
                break;
        case 6:
                s = Qt::Dense5Pattern;
                break;
        case 7:
                s = Qt::Dense6Pattern;
                break;
        case 8:
                s = Qt::Dense7Pattern;
                break;
        case 9:
                s = Qt::HorPattern;
                break;
        case 10:
                s = Qt::VerPattern;
                break;
        case 11:
                s = Qt::CrossPattern ;
                break;
        case 12:
                s = Qt::BDiagPattern;
                break;
        case 13:
                s = Qt::FDiagPattern;
                break;
        case 14:
                s = Qt::DiagCrossPattern;
                break;
        default:
                s = Qt::SolidPattern;
                break;
        }
        return s;
}
Qt::PenStyle drawData::getPenStyle(int i)
{

        Qt::PenStyle s;

        switch(i)
        {
        case 0:
                s = Qt::SolidLine;
                break;
        case 1:
                s = Qt::DashLine;
                break;
        case 2:
                s = Qt::DotLine;
                break;
        case 3:
                s = Qt::DashDotLine;
                break;
        case 4:
                s = Qt::DashDotDotLine;
                break;
        case 5:
                s = Qt::MPenStyle;
                break;
        default :
                s = Qt::SolidLine;

        }

        return s;
}
/**
    Qt::white	3	 White (#ffffff)
    Qt::black	2	 Black (#000000)
    Qt::red	7	 Red (#ff0000)
    Qt::darkRed	13	 Dark red (#800000)
    Qt::green	8	 Green (#00ff00)
    Qt::darkGreen	14	 Dark green (#008000)
    Qt::blue	9	 Blue (#0000ff)
    Qt::darkBlue	15	 Dark blue (#000080)
    Qt::cyan	10	 Cyan (#00ffff)
    Qt::darkCyan	16	 Dark cyan (#008080)
    Qt::magenta	11	 Magenta (#ff00ff)
    Qt::darkMagenta	17	 Dark magenta (#800080)
    Qt::yellow	12	 Yellow (#ffff00)
    Qt::darkYellow	18	 Dark yellow (#808000)
    Qt::gray	5	 Gray (#a0a0a4)
    Qt::darkGray	4	 Dark gray (#808080)
    Qt::lightGray	6	 Light gray (#c0c0c0)
    Qt::transparent	19	a transparent black value (i.e., QColor(0, 0, 0, 0))
    Qt::color0	0	0 pixel value (for bitmaps)
    Qt::color1	1	1 pixel value (for bitmaps)
    See also QColor
*/
QColor drawData::getPenColor(int i)
{
        QColor s;
        //qDebug() << "getPen=" << i;

        switch(i)
        {
        case 0:
                s = Qt::black;
                break;
        case 1:
                s = Qt::red;
                break;
        case 2:
                s = Qt::green;
                break;
        case 3:
                s = Qt::blue;
                break;
        case 4:
                s = Qt::cyan;
                break;
        case 5:
                s = Qt::magenta;
                break;
        case 6:
                s = Qt::yellow;
                break;
        default :
                s = Qt::gray;

        }

        return s;
}
QString drawData::prTime(QString node)
{
        QString str;
        str = QTime::currentTime().toString("hh:mm:ss:zzz");
        str  = str + "  : " +node;
        return str;
        //qDebug() << str;
}
