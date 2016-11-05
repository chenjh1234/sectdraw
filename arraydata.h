#ifndef ARRAYDATA_H
#define ARRAYDATA_H
#include "drawdata.h"
#include "QColor"
#include "QString"
#include "QMap"
#include "QPen"
#include "QBrush"
#include "drawdata.h"

#define DRAW_LINE 0
#define DRAW_AREA 1
#define DRAW_RECT 2
#define DRAW_NULL 3
//#define DATA_TYPE float
/// arrayData class

/**
     supply data class for arrayDraw, arraypaint.
*/
class arrayData : public drawData
{
public:
    /*! \brief Brief description.

     */

    arrayData();
    //~arrayData();
/// draw prameters struct
     struct M_DRAW
    {
            float *buf;
            int len,type,wide,fill;
            QColor color,bcolor;
    };
    /** buffer pointer to the data;
        the buf is just a point to caller data,we donot create memery space
    */
    float *buf; // for array
    /// length of the buffer:
    int m_iArrayLen;
    int m_iDrawType;// draw type
    int m_iDrawWide;
    int m_iDrawFill;
    int m_iDrawLen;
    /// draw object
    QPen m_pen;
    QBrush m_brush;
    QColor m_drawColor,m_brushColor;
    QString m_strDrawKey;


/// set current draw parameters ;
    int setDrawStyle(int type=0,QColor c=Qt::red,int wide=1,int fill=0,QColor cl =Qt::gray);
///  method to draw ruler in x direction
    qreal labMethodX(qreal x);
/// method to draw ruler in y direction
    qreal labMethodY(qreal y);
/// draw parameters struct
    typedef QMap<QString ,M_DRAW> M_DRAWMAP;
/// draw parameters m_draw
     M_DRAW m_draw;
/// map to store the Array draw parameters according to key
    M_DRAWMAP m_drawMAP;

    /**
      get  m_draw from map ,and set to current parameters;
      */
    M_DRAW getDrawFromMAP(QString key);
    /**
      save m_draw to MAP;
    */
    int saveDrawToMAP(QString key);
    /**
      get current draw parameters and save to m_draw;
    */
    M_DRAW getDraw();
private:
    int init();









};

#endif // ARRAYDATA_H
