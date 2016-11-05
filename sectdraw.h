// sectDraw.h: interface for the sectDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_sectDraw_H__3E72C4DF_9A0E_4976_9A75_AE24D2C49856__INCLUDED_)
#define AFX_sectDraw_H__3E72C4DF_9A0E_4976_9A75_AE24D2C49856__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "drawframe.h"
#include "sectdata.h"
#include "sectpaint.h"
#include "drawframe.h"
#define GAIN_MAX 10
#define RMS_MAX 10
#define RMS_MIN 2
/// interface for draw section ;
/**
   example:\n
   1:construct it\n
   2:get ready for the data;\n
   2:setMaxMin\n
   3:setDataPara: trace length ,sample rate \n
   4:startDraw:startDraw:send the data to drawFrame\n
*/

class sectDraw : public drawFrame
{
       Q_OBJECT
public:

    sectDraw(QWidget *p);
    virtual ~sectDraw();
 
/**  @brief 设置数据参数,set the data parameter.
    @param sam samples in each trace
    @param si sample rate in ms
    @param headLen the header length of each trace in bytes
*/
    int setDataPara(int sam,int si,int headlen);
/** @brief 设置数据,开始绘图,start to draw the data  for this
 *         draw
 *         and
 *         give the data parameters:
	@param hd: header
	@param data: traces data
	@param trs: bunber of traces for this draw
	@param sam samples in each trace
	@param si sample rate in ms
	@param headLen the header length of each trace in bytes  

	*/
    int startDraw(int *hd,float  *data,int trs,int sam,float si,int headLen);
/**设置数据,开始绘图,start to draw the data  for this draw 
	@param hd: header
	@param data: traces data
	@param trs: bunber of traces for this draw
	*/
    int startDraw(int *hd,float  *data,int trs);
    int startDraw();
    /// 取绘图增益值。
    float getGain();
    /**  @brief 设置增益值.缺省2,取值2-10\n
     *   maximum is double the amplitude,、\n
     * g = pow((qreal)2,(qreal)Gain/GAIN_MAX); \n iy[j] = 
     * (y[j]-fmin)*g/(fmax-fmin) *step ; \n 
       */
    int setGain(float gain);
    ///设置RMS值:fmax = RMS *? normal = 5;
    void setRMS(float rms); 
    ///得到RMS值
    float getRMS(); 
    /// 设置用户绘图比率。 set  user scale in x ,y direction:(trs/CM,CM/S) to section scale
    void setUserScale(qreal x,qreal y);
    ///  得到用户绘图比率。get  user scale in x ,y (trs/CM,CM/S) from section scale
    void getUserScale(float &x,float &y);
    /// 设置可见区(的左上角)在画域的位置。set the drawing position:,viewport original point;
    void setPos(int x, int y);
    /// 得到可见区位置。get the drawing position:,viewport original point;
    void getPos(int &x, int &y);
    /// 得到绘图方式。DRAW_VD,DRAW_WA
    int getDrawType();
    /// top label display data;
    void setDisplayHeader(float *f);
    
    /// 设置比较方式。纵向/横向/切换。for compare mode : V,H,TURNOVER
    void setCompareMode(int mode);
    ///得到比较方式。
    int getCompareMode();
    /// 得到绘图类指针。showHeader signal:
    drawing * getDrawing();
    /// 设置拾取方式,0不拾取/1层位/2切除。set pick mode:
    void setPickMode(int);
    ///得到拾取方式。
    int getPickMode();
    /// 设置拾取位置,0不拾取/1波峰/2波谷/3零值
    void setPickTo(int);
    ///得到拾取位置。
    int getPickTo();
    ///设置AGC时窗
    void setAGC(int len);
    ///得到AGC时窗长度:ms
    int getAGC();
    ///设置带通滤波参数
    void setFilter(float f1,float f2,float f3,float f4);
    ///的带带通滤波参数
    void getFilter(float& f1,float &f2,float &f3,float &f4);
    ///实施滤波
    void procFilter();
    ///实施AGC
    void procAGC();
//protected:
    //sectPaint *m_comDrawing; //  we can get it from m_comView
   ///point to the compared view(sectDraw) object  
    sectDraw *m_comView;
/// data and paint
    sectPaint  *m_paint;
    sectData *data;
public slots:

    /// 设置绘图方式0:变密度,1:变面积set draw type VD or WA  
    int setDrawType(int type);
    /// 设置色标, chang the image color table index  
    int setColor(int);// index !=0;and reDraw
    /// 设置画域位置,set scrollbar  pos
    void slotMovedTo(int, int);
    /// 是否显示道头,show header dialog:
    void setShowHeader(bool);
    //bool getShowHeader();


signals:
    int sigMovedTo(int, int);
    /// emit signal only,
    /// do the display header work by the application.
    void sigShowHeader(int);




protected:
    void setDrawing(drawing *d);//new it ,and set 
    void setData(drawData *d) ;//  newit and set;
    void init();
    /**
     * 
     *  user scale to the scene scale, trs/CM ,cm/S.
     */
    float user2ScaleX(float x);
    float user2ScaleY(float y);
    void user2Scale(float x,float y,float& xx,float &yy);


/** key pressed event:
    C: change draw type :VD--WA
    ESC: zoom out
*/
    void keyPressEvent ( QKeyEvent * e ) ;
        /** main draw ing function */

    void resizeEvent ( QResizeEvent * );
    void mousePressEvent ( QMouseEvent * e );
    void mouseMoveEvent ( QMouseEvent * event );
    void mouseReleaseEvent ( QMouseEvent * e );
    void scrollContentsBy(int dx, int dy);



private:

    // template use
    int m_ic;
    bool bMovedBy;
    /// when signal from colorbar comes the painter avoid to creat image **/
    //bool m_bSetColor;
    colorBar *cb;// we have cb both in draw and paint
    bool getGainGlobal();
    int setGainGlobal(bool b);
    bool bShowHeader;
    int maxTrs;
    bool bRedraw;

    
};

#endif // !defined(AFX_sectDraw_H__3E72C4DF_9A0E_4976_9A75_AE24D2C49856__INCLUDED_)
 
/**
    2016.3:保留的用户的原始数据obuf,接口开辟了工作空间buf,
*/

