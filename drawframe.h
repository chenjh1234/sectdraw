#ifndef DRAWFRAME_H
#define DRAWFRAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTransform>
#include <QRectF>
#include <rulerframe.h>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QString>
#include <QList>
#include <QResizeEvent>
#include <QMatrix>
#include <QDebug>
#include <QWindow>
#include <QScreen>

#include "drawscene.h"
#include "drawdata.h"
#include "drawing.h"
#include "messagepad.h"
#include "colorbar.h"
#include "colormapdlg.h"
#define LABEL_WIDE 50
#define OUTLOOK_TIMES 2

/// base class for drawFrame interface , virtual class

/** \brief
    绘图类库基类\n
    不能实例化,只能继承\n
    virtual void setDrawing(drawing *d) = 0;\n
    virtual void setData(drawData *d) = 0;\n

  
    To use it:\n
    1: You can subclass this class.\n
    2: create  drawing or its desendants ,implement setDrwing .\n
    3: create  drawData or its desendants ,implement setData.\n
    4: implement the paint method in drawing class;\n
  */
class drawFrame : public QGraphicsView
{
public:
    drawFrame(QWidget * parent);

/// 设置边框的宽度,set the ruler width;left,right,top,bottom
    void setRuler(int l,int r,int t,int b);
/// 设置绘图区的最大最小值范围。set the range rectangle of the scene or the data
    void setMaxMin(qreal minx,qreal miny,qreal maxx,qreal maxy);
/// 设置绘图比例 set  scale in x ,y direction,make the m_bFitWin false;
    void setScale(qreal x,qreal y);

///  得到绘图比例。get  scale in x ,y direction
    void getScale(qreal &x,qreal &y);
///  得到绘图比例。get  scale in x ,y direction
    void getScale(float &x,float &y);
///  得到X方向的绘图比例。get  scale in x ,y direction
    float getScaleX();

///  得到Y方向绘图比例。get  scale in x ,y direction
    float getScaleY();
/**
 * \brief 
    置坐标系,0:原点在屏幕的左上角。1:原点在右下角。设setCoordinate(intc);\n
 *  #define RIGHT_DOWN 0 :the original point at * the top left
 *   corner\n
 *  * #define RIGHT_UP 1 :the original * * point at the bottom *
 *      left corner\n * * *
*/
    void setCoordinate(int c);
/** 
 * \brief 设置网格。\n 
    
    \param x1,y1: is the short ruler line interval in x y direction\n
    \param x2,y2: is the longer ruler line interval in x y direction\n
        and also draw the drawing grid line if m_bGrid is true;
*/
    void setGrid(qreal x1,qreal x2,qreal y1,qreal y2);//0,0,0,0 is the omit
    /**
    for ruler:
   \param  x1: is the short ruler line interval in x y direction\n
   \param x2: is the longer ruler line interval in x y direction\n
       and also draw the drawing grid line if m_bGrid is true;\
    */
    void setGrid(qreal x1,qreal x2);//y is the same;
/// 是否画网格线。if the drawing area draw grid line or not\n
    void setGrid(bool b);//is m_draw draw the grid;
    bool getGrid();
/**
  设置色标条,0:setColorBar  ;
      id = 0: no color bar;COLORBAR_NO\n
      id = 1:horizon;COLORBAR_BT\n
      id = 2:vertical;COLORBAR_RT\n
*/
    void setColorBar(int id);

/// 设置边框中的鼠标光标线。if the cursor position line appear or not in the ruler
    void setPosLine(bool b);
/// 置顶部边框信息。display message to top or bottom ruler frame.
    void setTopMessage ( QString msg );
/// 置底部边框信息。
    void setBottomMessage ( QString msg );
/// 设置绘图比例,使画图区域和窗口吻合.不使用滚动条了
    void setFitWin();
/// automaticly control the outlook window;
    void isOutlook();
/// 是否显示画域框。
    void setOutlook(bool b);
    bool getOutlook();
    /// repaint the drawing area
    void reDraw();

protected:
    /// scene of the view
    drawScene *sc;//scene of the view;
    /// drawData object
    drawData *data;// draweData ,we donot new it, its subcalss will new it;
    /** main panting area,donot creat it in this class
     its descendants will construct it;
    */
    drawing *m_drawing;// the main panting area,we donot new it;
    /// ruler wide
    int m_leftW,m_rightW,m_topW,m_bottomW;// label wide in pixals;
    /// ruler object
    rulerFrame *leftR,*rightR,*topR,*bottomR;// label class
    /// message panel,in top ruler,
    messagePad *textMsgTop,*textMsgBottom;// massage class,

   /// colorBar object
    colorBar *m_colorBar;//colorBar class
    int  m_iDPIx,m_iDPIy;//DPI of the device ,resolution;
    /// is the cursor position red line in ruler appears or not
    bool m_bPosLine;
// screen:
    QScreen *con;
    QWindow win;
    float dotsPerInchX,dotsPerInchY;
    float cmsPerInch;



/// color bar maps select dialog
    colorMapDlg *cmap;
/// A pure virtual member: set drawing object
    virtual void setDrawing(drawing *d) = 0;
/// A pure virtual member: set drawData object
    virtual void setData(drawData *d) = 0;


/** put the ruler geometry to drawData;\n
    we use it when the ruler geometry changed;
    */
    void updateRuler();

/** put the viewport() wide and hight to drawData;\n
    we use it when resize (the viewport geometry changed);
    */
    void updateFrame();

/** put the Scene geometry to drawData;\n
    we use it when the  Scene changed;
    */
    void updateScene();

/** Reposition Ruler\n
      we use it when  resize ,scrollbar positionchanged;
    */
    void posRuler();
/** reposition Main drawing area\n
    we use it when  resize ,scrollbar;
        */

    void posDraw();
    /**
      make drawing area fit to window; get scale and set scene size;\n
      we use it in resize normally;
      */
    void autoZ();
    void init();
/**
  map Length  in x y,direction
*/
    qreal mapToSceneX(qreal x);
    qreal mapToSceneY(qreal x);
    qreal mapFromSceneX(qreal x);
    qreal mapFromSceneY(qreal y);
    void mouseMoveEvent ( QMouseEvent * event );
    void resizeEvent ( QResizeEvent * event);
    //  void focusInEvent ( QFocusEvent * event );
    //void focusOutEvent ( QFocusEvent * event );
/**
     reposition the ruler and drawing object when scrollbar changed
     */
    void scrollContentsBy ( int dx, int dy );
private:
/**
    internal use .move the position line when the mouse moving.
  */
    void  setRulerLine ( int x,int y );
/** internal use .hide or show the ruler line:*/
    void  setRulerLine ( bool r );

    void setMaxMin();
    bool m_bFitWin;
};

#endif // DRAWFRAME_H
/**
 \mainpage drawFrame 框架简介
  提供绘制地震剖面二维图形的组件：
  基础框架组件：drawFrame。\n
  绘制地震剖面组件：sectDarw。\n
  绘制二维数组组件：arrayDraw。\n
  发展其他应用组件：可以继承 drawFrame , drawing ,drawData 类，进行开发。\n
  drawFrame :接口类\n
  drawing   :处理绘图相关\n
  drawData  :处理数据相关\n
 
  \section 术语 术语
  基础框架组件：drawFrame
  应用框架组件：继承基础框架组件，开发完成的应用绘图组件。

  \section  基础框架  基础框架组件：drawFrame
  有边框，画图区组成。
  \subsection 边框 
     四个边框：上下左右，主要是标注画图区的坐标值。\n
     边框上有：刻度（短，长），长刻度有刻度标记，标出坐标值。\n
     上边框：有信息区，显示两行的信息。\n
     下边框：有信息区，显示一行的信息。\n
     下边框上有：色标，色标可以切换和拖动调整。切换:ctrl+鼠标,调整:shift+拖动\n
     红色光标线：标记光标的位置。\n

  \subsection 画图区 
    主要用来绘制所描述的数据，是框架的核心区域。\n
    网格线：可绘制网格线
    继承drawing类，完成绘制功能。\n
  \subsection 功能 
    缩放功能。
    提供：边框，光标线，色标，网格线，颜色，填充，参数等。

  \section  sectDraw 基础框架组件：sectDraw
  绘制地震剖面。\n
 
  \section  arrayDraw  基础框架组件：arrayDraw
 
  绘制二维数组图形\n

 */
