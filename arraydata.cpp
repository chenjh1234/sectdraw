#include "arraydata.h"

arrayData::arrayData():drawData()
{
}
int arrayData::init()
{
        m_iArrayLen = 0;
        buf = NULL;
        m_iDrawType = 0;
        return 0;
}
int arrayData::setDrawStyle(int type,QColor c,int wide,int fill,QColor cc)
{
        m_iDrawType = 0;
        m_iDrawType = type;
        m_iDrawWide = wide;
        m_drawColor = c;
        m_iDrawFill= fill;
        m_brushColor = cc;

        m_pen.setColor(c);
        m_pen.setWidth(wide);

        //Qt::BrushStyle s;
        //qDebug() << "color=" << cc<<c;
        
        m_brush.setStyle(getBrushStyle(fill));
        m_brush.setColor(cc);
        return type;
 }


arrayData::M_DRAW arrayData::getDraw()
{
        m_draw.buf = buf;
        m_draw.color = m_drawColor;
        m_draw.bcolor = m_brushColor;
        m_draw.fill = m_iDrawFill;
//	m_draw.key = m_iDrawKey;
        m_draw.len = m_iDrawLen;
        m_draw.type = m_iDrawType;
        m_draw.wide = m_iDrawWide;

        return m_draw;
}
int arrayData::saveDrawToMAP(QString key)
{


        m_draw = getDraw();
        m_drawMAP.insert(key,m_draw);
        return 0;
}

arrayData::M_DRAW arrayData::getDrawFromMAP(QString key)
{
        M_DRAWMAP::iterator it;

        it = m_drawMAP.find(key);
        m_draw = it.value();
        m_iDrawLen = m_draw.len;
        m_iDrawFill = m_draw.fill;
        m_iDrawType = m_draw.type;
        m_iDrawWide = m_draw.wide;
        m_strDrawKey = key;
        buf = m_draw.buf;
        m_drawColor = m_draw.color;
        m_brushColor = m_draw.bcolor;
        return m_draw;
}
qreal arrayData::labMethodX(qreal x)
{
    return x;
}
qreal arrayData::labMethodY(qreal x)
{
    return x;
}

