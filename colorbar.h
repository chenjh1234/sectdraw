#ifndef COLORBAR_H
#define COLORBAR_H
//#include <QObject>
#include "drawitem.h"
#include "QPainter"
#include "QGraphicsSceneMouseEvent"
#include "QKeyEvent"
#include <QVariant>

#define COLOR_TABLES 13
#define BAR_COLORS 256
#define COLORBAR_NO 0
#define COLORBAR_BT 1
#define COLORBAR_RT 2
/// color bar
/**
  usally used in draw sections in Varirnt Density mode.
   LeftButton pressed and moveing: color map index rotate;
   Shift + RightButton pressed: color table changed -1;
   Shift + LeftButton pressed: color table changed +1;
   Contral +  RL DoubleClick: color map index reset to 0;

  */

class colorBar : public drawItem
{
    //Q_OBJECT
public:
    colorBar(QGraphicsItem  *parent = 0);

    QColor *m_color;///< color point to the table;
    QString *m_names;///< color table names
    int m_iMaps;  ///< color tables in the class;

///   set the color table index
    int setColor(int i);
    int getColor();// get color table index;
/// set colorBar wide,height;
/**
  we have to set it if we change the size.
*/
    void setSize(int w,int h);
/// set the colorbar vertical or horizon
    int setVertical(bool b);
/// enable or disable to move the bar color
    int setMove(bool b);
/// display the bar or not
    int setDisplay(bool b);

///   find the index of the color in current table
  /**
   \param x  pixal index in color bar widget;
   \return index of the color in current color table
  */
    int dx2Idx(int x);
/// set color table ,and redraw the scene
    void setColorSlot(int idx);



/** mouse functions:
            LeftButton pressed mouse move: color table index rotate;
            Shift + RightButton pressed: colortable changed -1;
            Shift + LeftButton pressed: colortable changed +1;
            Contral +  RL DoubleClick: color table index reset to 0;

*/
//signals:
//    void clicked();
     int m_iIdx;   ///< current idx of the color Table  (from 0);
protected:
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
    void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    QVariant  itemChange ( GraphicsItemChange change, const QVariant & value );
//    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
 //   void keyPressEvent ( QKeyEvent * event );
 //   void keyReleaseEvent ( QKeyEvent * event );


private:

    //
    int iw,ih;//bar wide and height,we
    bool m_bVertical;// is a vertical bar or horizon bar
    int x0,y0,dx,dy;//pressed x,y,distance to x0,y0
    int m_iColorIdxMax;//
    float df;//colors per pixal in color bar
    QPen oldPen;
    bool m_bMove;//true :make the color color moveable
    bool m_bDisplay;//true,make the colorbar appear
    bool m_keyShift,m_keyControl;
//


    int m_iColorTBIdx; ///< current color table idx
    int m_iSelect; ///< if selected;

    void init();
/** actual shape of the color bar
    */
    int plot(QPainter *p); ///< in paint number
    int getColorTableIndex(); ///< index of color table in maps;
    int getColorIndex(); ///< get index from current color table
    void i2rgb(int index,int *ri,int *bi,int *gi);
    QRectF boundingRect(void)const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget);
    QPainterPath shape()const;

};

#endif // COLORBAR_H
