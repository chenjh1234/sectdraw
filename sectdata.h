// sectData.h: interface for the sectData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_sectData_H__9E73EF4A_8EC6_4755_92F8_DD14310C6A67__INCLUDED_)
#define AFX_sectData_H__9E73EF4A_8EC6_4755_92F8_DD14310C6A67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "drawdata.h"
#include <QGraphicsEllipseItem>
#include <math.h>
//drawing mode
#define DRAW_VD 0
#define DRAW_WA 1
#define DRAW_WW 2
#define DRAW_AA 3
// pick mode
#define PICK_MODE_NO 0
#define PICK_MODE_LAYER 1
#define PICK_MODE_MUTE 2
//pick to mode
#define PICK_TO_NO 0
#define PICK_TO_PEAK 1
#define PICK_TO_VALLEY 2
#define PICK_TO_CROSS 3


#define PICK_ITEM_WIDE 30
// compare
#define COMPARE_MODE_H 0
#define COMPARE_MODE_V 1
#define COMPARE_MODE_TURNOVER 2
#define COMPARE_MODE_NO 3

#define PICKDATA data->m_pickData
#define PI (3.141592653589793)
/**
 * pick : left mouse button 
 * remove pick :right mouse button 
 */
class pickData
{
public:
    pickData()
    {};
    ~pickData()
    {};

//-----------------------------------------------------

    ///knee point:idx ,time
    QMap<int, float> pickPoints;
    /// the Mark at knee point: idx, ellipse
    QMap<int, QGraphicsEllipseItem *> pickMarks;
    /// intplation pick time of this gather. it cmes from other gathers pick time.
    QPolygonF m_intpPolyTimes;

};
class sectData : public drawData
{
public:

    sectData();
    virtual ~sectData();

///   m_bTranspalency
    bool m_bTranspalency;
// when VD default ColorBar Index
//    int m_iVDColorType;

// data parameters:
/// data buffer for seismic data traces ,we donot new space;
    float *buf,*obuf;
/// header buffer for seismic data traces  ,we donot new space;
    int *head;
/// display header
    float *displayHeader;
    float m_MaxDisplayHeader;
/** data parameters:
    \param:sam:samples of a trace
    \param:trs:traces in the gather
    \param:headLen: number of th head word;
    \param:si: sample rate im ms;
    */
    int sam ;// is the samples;
    int trs,headLen;
    float si;//ms
/**
      ampiltude range of this gather data;
    */
    float fmax,fmin; // for data range of this section
    /// user rms prameter:fmax = m_RMS*m_rms;
    double m_rms;//fmax = m_RMS*m_rms;
    ///calculated data RMS of this gather;when startDraw()
    double m_RMS;// data RMS of this gather;
    /// draw WA,offset in x from the image left edge;
    int m_iDotOffsetX;
    int m_iDotOffsetY;
/// for compare mouse control:
    int  m_xCom,m_yCom;//mouse move position when compare section;
    bool bDrawTurnover;
    int m_compareMode;

/// draw paramwters:DRAW_VD,DRAW_WA,
    int m_iDrawType;
    int m_fGain;
    bool m_bGainGlobal;
// the turn to wiggle from plotVD,we remember the current Index og colorBar
    int m_iIndexColorTBBak;
/// method for calculat trace label number;
    qreal labMethodX(qreal x);
/// method for calculat time label number;
    qreal labMethodY(qreal y);
    /// calculate the data max,min
    void getMaxMin();
    ///calculate  data RMS max = 5*RMS
    double getDataRms();
    double getRMS();
    void setRMS(float m);

    int m_iAGCLen;//ms
    float m_f1,m_f2,m_f3,m_f4;
    enum PICK_MODE
    {
        PICK_NO,
        PICK_LAYER,
        PICK_MUTE
    }m_pickMode;

    enum PICK_TO
    {
        PICK_NOW,
        PICK_PEAK,
        PICK_VALLEY,
        PICK_CROSS
    }m_pickTo;

    pickData *m_pickData;
    /**
     * adjust pick to peak or valley or cross;
     * @param idx 
     * @param time0 
     * 
     * @return float 
     */
    float adjustPick(int idx, float time0);

      /**
   * Linear interpolation between knee points
   * @param numSamplesIn   Number of samples in input array
   * @param xin            Input samples
   * @param yin            Input sample values
   * @param numSamplesOut  Number of samples in input array
   * @param sampleIntOut   Output sample interval (must be in same unit as input samples xin)
   * @param yout           Output sample values, to be computed
   */
    static void linearInterpolation( int numSamplesIn, float const* xin, float const* yin,
                                   int numSamplesOut, float sampleIntOut, float* yout );
    /**
     * filter
     * @param data :data buffer
     * @param len ;samples
     * @param si sample rate
     * @param f1 high pass
     * @param f2 
     * @param f3 low pass
     * @param f4 
     * 
     * @return int 
     */
    int doFilter(float *data,int len,float si,float f1,float f2,float f3,float f4 );
    /** 
     * AGC 
     * @param data : data buffer
     * @param iwagc : agc window in samples
     * @param nt : samples:
     */
    void doAGC(float *data, int iwagc, int nt);
    void bfdesign (float fpass, float apass, float fstop, float astop,
                        int *npoles, float *f3db);
    void bfhighpass (int npoles, float f3db, int n, float p[], float q[]);
    void bflowpass (int npoles, float f3db, int n, float p[], float q[]);


private:
    void init();
    
    
};


#endif // !defined(AFX_sectData_H__9E73EF4A_8EC6_4755_92F8_DD14310C6A67__INCLUDED_)
