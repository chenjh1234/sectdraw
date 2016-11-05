// btrplt.h: interface for the btrplt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BTRPLT_H__E5E7DC41_E9E1_4ABB_899F_811E90B4E26F__INCLUDED_)
#define AFX_BTRPLT_H__E5E7DC41_E9E1_4ABB_899F_811E90B4E26F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <Qt>
//#include <stdio.h>
/// plot interface from BGSYS
/**
        plot section  ,plot in pbs;
*/

class sectplot 
{
public:
    sectplot();
    virtual ~sectplot();
/// parameters for plot interface
    /**
    pbs struct ,a plot area discription data.
    wide ,height,number of bytes per line,data ,draw pixel.
    */
    struct pbs
    {
        int mx; //wide in dot
        int my; //height in dot
        int nbpl;// number of bytes per line
        unsigned char *a; // image buffer
        unsigned char pixel;//draw pix
    }pb;

    /// current x,y,bytes offset
    int ix1,ix2,yrs;

    /**
        初始化pb为指定像素。

        initialize pbs parameters:
    */

    void pltinit(int mx,int my,int nbpl,unsigned char *a,unsigned char pixel);

    /**
    （画剖面:曲线）在pb中绘图：每次绘一个点的x坐标（ifb中），y坐标增加一个像素。
        每次的起点为上次的终点。

    xs,ys坐标开始画线。
    Ibf[n],存放x坐标的数组。

    draw section: wiggle - wiggle
    xs,ys: start x,y
    ibf[n]: array containing x

    */
    int pltwiggle(int xs,int ys,int n,int *ibf);

    /**
    （画剖面:只画正值,正变面积:在pb中绘图：每次绘一个点的x坐标（ifb中），画完后y坐标增加一个像素。
        每次的起点为(xs，ys+i)，
        xs,ys坐标开始画线。
        Ibf[n],存放x坐标的数组。

        draw section: positive area.
        xs,ys: start x,y
        ibf[n]: array containing x
    */
    int pltpa(int xs,int ys,int n,int *ibf);

    /**
        （画剖面:只画负值,负变面积:在pb中绘图：每次绘一个点的x坐标（ifb中），画完后y坐标增加一个像素。
        每次的起点为(xs，ys+i)，
        xs,ys坐标开始画线。
        Ibf[n],存放x坐标的数组。

        draw section: minus area.
        xs,ys: start x,y
        ibf[n]: array containing x
    */
    int pltma(int xs,int ys,int n,int *ibf);

/**
    画剖面:变密度:
    在pb中绘图：每次绘一个点的x坐标（ifb中），画完后y坐标增加一个像素。（画剖面）		每次的起点为(xs，ys+i)
    pix[n]：为像素点颜色数组，对应ibf[n]中的x点坐标，每次绘图使用pix[n]中的颜色值。
    xs,ys坐标开始画线。
    Ibf[n],存放x坐标的数组。

    draw section: VD:  draw
    \param xs,ys: start x,y
    \param ibf[n]: array containing x。(amplitude)
    \param pix[n]: colors ,according to to x.
*/
    int pltvd(int xs,int ys,int n,int *ibf,unsigned char *pix);
/** just draw one veitical line use color pix[n]
*/
    int pltvd(int xs,int ys,int n,unsigned char *pix);

    /**
        用pix 在x方向上画横线，(ix1,yrs)-->(ix2,yrs)

        draw a horizon line :(ix1,yrs)-->(ix2,yrs)
        \param pix: pattern or color*/
    void hline(char pix);

/**
    申请空间，

    new the space;
    \param nbpl:number of bytes per line
    \param nl: number of lines
    */
    unsigned char * raster( int nbpl,int nl);
    /**	horizon line:(ix,iy)-->(ix+lg,iy)
    */
    void hline(int ix,int iy,int lg,
    int nbpl,int col,unsigned char *bits);
    /**
    horizon line:(ix,iy)-->(ix+lg,iy) use mask:bmask
    we can draw dash dot line
    */
    void hlined(int ix,int iy,int lg,
    int nbpl,int col,unsigned int bmask,unsigned char *bits);
    /**
    vertical line:(ix,iy)-->(ix,iy+lg)
    */
    void  vline(int ix,int iy,int lg,
    int nbpl,int col,unsigned char *bits);
    /**
    vertical line:(ix,iy)-->(ix,iy+lg)  use mask:bmask
    */
    void  vlined(int ix,int iy,int lg,
    int nbpl,int col,unsigned int bmask,unsigned char *bits);
    /**
    fill a rectangle: (ix,iy,ix+lx,iy+ly);
    */
    void  fillrect(int ix,int iy,int lx,int ly,
    int nbpl,int col,unsigned char *bits);


};

#endif // !defined(AFX_BTRPLT_H__E5E7DC41_E9E1_4ABB_899F_811E90B4E26F__INCLUDED_)
