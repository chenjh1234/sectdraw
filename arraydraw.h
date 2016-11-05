#ifndef ARRAYDRAW_H
#define ARRAYDRAW_H
#include "drawframe.h"
#include "arraydata.h"
#include "arraypaint.h"
/// draw  data in an array or arrays;
/**
arrayDraw:

Function:
---------

        Draw arrays datas in the widget.The data can be qreal.\n
        We can draw more array use diffrent draw parameters\n
        X coordinate is the index of the array:idx:0,1,2,3,4,....\n
        Y coordinate is he data values:y[idx]\n
        The point is (x,y) = (index,buf[index])\n
        use setScale(x,y) to define the XY unit ,defaut is 1 pixal.\n
\n
Draw Style:\n
-----------\n
        We draw the data use one of the draw style:\n
                DRAW_NULL: draw nothing\n
                DRAW_LINE: link the point with line.\n
                DRAW_AREA: link the point with line.and link the point to X axial vertically.\n
                DRAW_RECT: draw a histogram,top left conor is the point,wide is 0.9 actual interval .\n
\n
Mandatory user interface functions:\n
---------------------------------\n
\n
        setMaxMin(maxx,minx,Maxy,Miny)\n
                Set the data range for All the data.\n
                We call this function before We first time call StartDraw();\n

        startDraw(buf,len,key);\n
                give the data to the Widget and start to draw.\n
                We can call this interface more than one times with different data and differnt key.\n
                If we call the function with the same key,the later data will replace the earlier.\n
                before we call the function each time,We can set the Draw style with SetDrawStyle.\n
                        (or SetMeta,SetSymbol. to set the Meta,and Symbol style)\n
                We have to call SetMaxMin before we SartDraw().\n
   example:
   1:new it,get ready for the data;
   2:setMaxMin
   3:startDraw,startDraw:send the data to drawFrame
*/
class arrayDraw : public drawFrame
{
public:
    arrayDraw(QWidget *w);
    //void ~arrayDraw(QWidget *w);
    ~arrayDraw();

/** set draw style:LINE,AREA,RECT,NULL,we select one of the draw type.
    c: draw pen color
    wide: pen width
    fill: when draw RECT ,fill brush style:0-15
*/
    int setDrawStyle(int type,QColor c = Qt::red,int wide =1,int fill =0,QColor b = Qt::gray);
    /** set draw style:LINE,AREA,RECT,NULL,we select one of the draw type.
        c: draw pen color
        wide: pen width
        fill: when draw RECT ,fill brush style:0-15*/

    int setDrawStyle(int type=0,int c=0,int wide =1,int fill =0,int b=2);
/**
    We use it in StartDraw or if we want to change draw style ,after setDrawStyle();
    it will save the draw style to MAP;
*/
    int reDrawStyle(QString key ="");

/**
    start to draw:
    user can call this function more times.
    if we draw differnce data,we can use differnce key.
    if we renew the data,we can use the same key.
    this function will register the draw,  stuff to MAP;
*/
    //int startDraw(qreal *buf,int len,QString key = "");
    int startDraw(float *buf,int len,QString key = "");

/**
    delete the array Draw with key.
    it will delete all the things From the MAP(LIST) according to the key,
*/
    int deleteDraw(QString key);

protected:
    arrayData *data;
    arrayPaint *m_paint;
    void setDrawing(drawing *d);
    void setData(drawData *d) ;

   // void mouseMoveEvent ( QMouseEvent * event );
    void resizeEvent ( QResizeEvent * event );
   // void scrollContentsBy ( int dx, int dy );
    void mousePressEvent ( QMouseEvent * e );
private:

     void init();
};

#endif // ARRAYDRAW_H
/**
  2011.11.10: arrayDraw ,more arrays with diffrent draw parameters
  */
/**
arrayDraw:

Function:
---------

        Draw arrays datas in the widget.The data can be qreal.
        We can draw more array use diffrent draw parameters
        X coordinate is the index of the array:idx:0,1,2,3,4,....
        Y coordinate is he data values:y[idx]
        The point is (x,y) = (index,buf[index])
        use setScale(x,y) to define the XY unit ,defaut is 1 pixal.

Draw Style:
-----------
        We draw the data use one of the draw style:
                DRAW_NULL: draw nothing
                DRAW_LINE: link the point with line.
                DRAW_AREA: link the point with line.and link the point to X axial vertically.
                DRAW_RECT: draw a histogram,top left conor is the point,wide is 0.9 actual interval .

Mandatory user interface functions:
---------------------------------

        setMaxMin(maxx,minx,Maxy,Miny)
                Set the data range for All the data.
                We call this function before We first time call StartDraw();

        startDraw(buf,len,key);
                give the data to the Widget and start to draw.
                We can call this interface more than one times with different data and differnt key.
                If we call the function with the same key,the later data will replace the earlier.
                before we call the function each time,We can set the Draw style with SetDrawStyle.
                        (or SetMeta,SetSymbol. to set the Meta,and Symbol style)
                We have to call SetMaxMin before we SartDraw().
Draw Symbol or Meta:
-------------------

        We can also draw symbol or meta if we use SetMeta or SetSymbol functions.
        We draw the symbol or meta in the point.
        Meta: the performance is slow when use meta.it will create metas numbers as many as the array size.
                  If you have large number of data,be cautious to use of meta.
                  Meta is more easy when you want to trace the meta sambol  or select meta.
        Symbol:the performance is fast.
        The style of symbol or meta:
                SYMBOL_NULL: draw nothing;
                SYMBOL_CROSS: draw a cross,the point is the center.
                SYMBOL_CIRCLE: draw a circle,the point is the center.
                SYMBOL_RECT: draw a rectangle,the point is the center.
Grid:
-----

        the widget can draw a grid background:
        SetGrid(flag,color,style);
        flag !=0 The Grid displayed.

Scale:
------
        setScale(float sx,float sy)
        sx=0,sy =0: the data range fit to window size.
        fx: pixals between 1 data in x direction.
        fx: pixals between 1 data in y direction.

The Coordinate system:
---------------------

        We can set coordinat system with:
                SetCoordinate(int s);
                s: RIGHT_UP,RIGHT_DOWN

        RIGHT_UP:

                y
                |
                |
                |_________x

        RIGHT_DOWN:
                ___________x
                |
                |
                |
                y

Annotation:
-----------
        we can set four label wide with SetLabelWide.

        SetLabelWide(top,buttom,left,right);
        0: means :no that label;


Typycal use:exsample:
----------------------

        declare the class:

        #include "arawArray.h"

        //in your function:
                df = new arawArray();
        // prepare your data:  example: buf[500];
        // set the data value range:(cannot omit:)
                df.setMaxMin(maxx,minx,maxy,miny);
        //draw a array:
                df.startDraw(buf,len);
        //draw more  arrays:
                df.startDraw(buf1,len,"first data");
                df.startDraw(buf1,len,"second data");
                df.startDraw(buf2,len,"third data");
                .......



*/
/** ************************************************************
full EXSAMPLE OF arawArray

    QGridLayout *ly;
    ly = new QGridLayout(this);
    gv = new arrayDraw(this);
    ly->addWidget(gv);
    gv->setMaxMin(0,0,200,200);
    int i;
    qreal *f,*f1;
    f = new qreal[200];//first array
    f1 = new qreal[200];// 2th array

    for(i = 0; i <200;i++)
    {
        f[i] = i;
        f1[i] = i*1.23+5;
    }
    QString key,key2;
    key = "1";
    key2= "2";
    gv->setColorBar(1);
    gv->setCoordinate(RIGHT_UP);

    gv->setDrawStyle(0,1,1,0);
    gv->setScale(3,3);
    gv->startDraw(f,200,key);//first array
    gv->setDrawStyle(0,2,1,0);
    gv->startDraw(f1,200,key2);// second array
    gv->setDrawStyle(1,1,1,0);
    gv->startDraw(f1,50,"");//3rd array,len = 50
    gv->show();
*/
