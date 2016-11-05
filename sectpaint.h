#ifndef SECTPAINT_H
#define SECTPAINT_H
#include "drawing.h"
#include "sectdata.h"

#include "sectplot.h"
#include "intpsub.h"
#include <QImage>
#include <QGraphicsLineItem>
#include <QPolygonF>
#include "colorbar.h"
/// paint the seismic data traces
/**
   implement the paint number for sectDraw;
  */
class sectPaint : public drawing
{
    //Q_OBJECT
public:
    sectPaint();
    ~sectPaint();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    //void paintP(QPainter *painter, qreal x1,qreal y1,qreal x2,qreal y2);
    void setData(drawData *d);
    sectData *data;
    colorBar *cb; // we have cb both in draw and paint

    /** image for current draw*/
    QImage *img; //for this drawing;
    /**point to compare drawing img; we used copyimage,but works not 
    well .   
     * this is a sign if we do compare 
        */
    QImage *imgCom;


    /** plot vd in image
    @param  w: wide of the image
    @param  h: height of the image
    @param  sttr: start trace of the buf.
    @param  trs: number of traces to draw;
    @param  stsam: start sample of the trace
    @param  sam: samples of the trace to draw 
    @param  bytes: bytes of the line in image; 
    */
    //int plotVD1(int w,int h,int sttr,int trs,int stsam,int sam);
    int plotVD(int w, int h, int sttr, int trs, int stsam, int sam);
    /** plot wiggle+ area in image
    @param  w: wide of the image
    @param  h: height of the image
    @param  sttr: start trace of the buf.
    @param  trs: number of traces to draw;
    @param  stsam: start sample of the trace
    @param  sam: samples of the trace to draw 
    @param  bytes: bytes of the line in image;  
    */
    //int plotWA1(int w,int h,int sttr,int trs,int stsam,int sam);
    int plotWA(int w, int h, int sttr, int trs, int stsam, int sam);
    //int plotWAA(int w,int h,int sttr,int trs,int stsam,int sam,float g);

    /// plot h line in image
    int plotHLine(int x, int y, int w, int h);

    //int setColorIndex(int idx);
    /// plot sect ,it will call plotVD or plotWA
    int plotSect(int w, int h, int sttr, int trs, int stsam, int sam);
//pick:==============================================
    /**
     * 
     */
    void plotPick(QPainter *painter, int w, int h, int sttr, int trs, int stsam, int sam);
    /**
     * calculate the time that interpolation from the knee point
     */

    float getPickTime(int idx); // one by one ,no useed now
                                /// paintPick use it to draw polygline
    QPolygonF getIntpPickTime(QMap<int, float> knee, QPolygonF p);
    /// interplation array from knee point,output num points;
    QPolygonF getIntpPickTime(QMap<int, float> knee, int num);
    /**
     * add point to map(PickPoint and pickMark; 
     * if the same idx ,replace it; (index)
     *  
     */
    void addPickPoint(int, float); // index
    /** add pick point with headword value;
    * when move data to the picked gather ,we use it to restore the 
    * pick point 
     * */
    void addPickPointHD(float, float);
    /**
    * add 2 points ,and remove all the points from sx to ex(index)
    *    
    */

    void addPickPoint(int sx, float sy, int ex, float ey);
    /**
        remove  point from map(PickPoint and pickMark; (head value) 
      *  
      */
    void removePickPoint(int);
    /**
      remove  points from s to e  not include s,e (index) 
    *   
    */
    void removePickPoint(int s, int e);
    /// remove all pick point;
    void clearPickPoints();
    /**
     test if the mouse point , is the point on  knee point when*  
     remove* knee point with mouse right button*, or replace pick*  
     with mouse left button *  
   *  
   */

    bool isPickPoint(int idx);
    /// move the pick item invisible;
    void setVisiblePickMarks(bool b);
    /// define the location the the exactly position of the trace;
    /// we define the trace is at the right of the mouse point.+1
    int reLocIndex(int idx);
    /** interplation with which head word,we use the diaplay head
        word of the sectDraw object,in the current  gather.
        */
    int getPickIntpHD(int idx);


private:
    /// plot class
    sectplot plt;
    /// interpolation class
    intpsub intp;

    /** set the image color table
    find the index of the color table ;do not reDraw
    */
    int setImageColor();
    int setComImageColor(int iw);
    /// mouse prassed x,y
    int xPressed,yPressed,sxPressed,syPressed;
    int xMove,yMove;
    int m_iButton;
    QGraphicsLineItem *m_line;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);

};

#endif // SECTPAINT_H
