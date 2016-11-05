// sectiamge.h: interface for the sectiamge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SECTIAMGE_H__8290B329_39FA_425C_95D1_D11915242AAE__INCLUDED_)
#define AFX_SECTIAMGE_H__8290B329_39FA_425C_95D1_D11915242AAE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "qapplication.h"
/// interpolation:functions used to plot SECTION.
/**
interpolation:functions used to plot SECTION.
    the method is : 4 point interplotation

*/
class intpsub  
{
public:
	

	intpsub();

	virtual ~intpsub();
/** void lgcoef(float*,float*);

	calculate the coefficient for 4 point interpolation:

	used by vlgint;

	frc：小于１的符点，是采样时，符点索引和整数索引的差。
	ｏ：算出的四个系数。四点插值的插值系数。

	@param frc: float <1; the difference of float index and int index;

	@param o:four cofficent for the four float
*/
        void lgcoef(float  *frc,float *o);

/**
	void vlgint(int n,float *f,float *x0,float *x1,float *x2,float *x3,float *x) ;
	横向上插值。每次只插一个值。.
	n:哪个样点，后面四个数组的索引值。.
	x0[n],x1[n],x2[n],x3[n].第一道，二道，三道，四道的样点值序列。.
	x:输出。X[n]，处在第二，三道之间。.
	f = i/dpt.<1的符点数：dots per trace;.
	插出：用4点插值，插出两道间的一道。.
	纵向上是n个像素。

	样点数为：m

	dps =(double) (n1)/ (double) (m-1),样点间的像素数。:dots per sampler;

	spd= 1.0/dps：像素间的样点数。符点。:samples per dots;
	
	interplate in x(traces) direction: give four trace,get each dot value bettwn 2,3 trace;

	@param n: index of the four float data(samples)
	@param f: i/dpt <1;float,
	@param x0,x1,x2,x3: four float.four traces;
	@param x: interplate result.

*/
        void vlgint(int n,float *f,double *x0,double *x1,double *x2,double *x3,double *x) ;
	void vlgint(int n,float *f,float *x0,float *x1,float *x2,float *x3,float *x) ;

/**  void lginta(int n,float *x,float *y,int *indx,float *tbl);
	纵向插值：

	n：y向像素点数。
	x：的对应该像素样点的值。n
	y：输出插值的结果的值。n
	Indx：数组，x的下标（索引）对应的数据道的样点的下标（索引）。
	tbl：数组，n*4,系数表格。

	interpolation in vertical(y or time) direction:

	@param n: samples
	@param x: x[n],input array
	@param y: y[n],output array 
	@param indx: index of x[n] ,to the samples index.
	@param tbl: n*4 factor table

*/
	void lginta(int n,float *x,float *y,int *indx,float *tbl);
	/**
	interplate in vertical(y or time) direction.

	@param n: samples
	@param m: samples in y
	@param x: x[n],input array
	@param y: y[m],output array 
	@param indx: index of x[n] ,to the samples index.
	@param tbl: n*4 factor table 
	*/
	void lgintaa(float *x,float *y,int m,int n,int *indx,float *tbl);
/**
	void lgintp(int n,float *x,float *y,float *tbl);
	纵向插值：

	n：y向像素点数。
	x：的对应该像素样点的值。n
	y：输出插值的结果的值。n
	tbl：x值下标（索引）对应的数据道的样点的下标（索引）的符点值。

	interpolation in y dirction: 

*/
	void lgintp(int n,float *x,float *y,float *tbl);
/**
	void lgeqsi(int n,int ni,float *x,float *y);
	纵向插值：

	将n个样点，插出ni *(n-1个样点。
	在x的两个样点之间插出ni样点。
	y的采样率为1/ni
	x的采样率为：1

	输入：x[n];
	输出：y[ni *(n-1)];

  interpolation in y direction:
  we know x[n],
  interplat: y[ni *(n-1)]; get ni sample between x[i]  and x[i +1];

*/
	void lgeqsi(int n,int ni,float *x,float *y);
/**
	纵向插值：

	已知：x[m]的值，m个。
	要求插出y[n],n个值。x,y的第一个值相同。

	 inteplation in y directions:

	we know x[0],x[1],x[2],.....x[m], intplate:y

	x[m]-->y[n]

*/
	void intpy(float *x,float *y,int m,int n);
    void intpy(float *x,float *y,int m,int n,float startS);// 0< startS <1,start fraction of the output
};

#endif // !defined(AFX_SECTIAMGE_H__8290B329_39FA_425C_95D1_D11915242AAE__INCLUDED_)
