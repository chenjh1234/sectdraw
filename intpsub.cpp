// sectiamge.cpp: implementation of the sectiamge class.
//
//////////////////////////////////////////////////////////////////////

#include "intpsub.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

intpsub::intpsub()
{

}

intpsub::~intpsub()
{
 
}
void intpsub::lgcoef(float  *frc,float *o)
{
	float fm1,fm2,fp1,f;
	f=*frc;
	fm1=f-(float)1;
	fm2=f-(float)2;
	fp1=f+(float)1;
	o[0]=fm2*fm1*f/(float)(-6);
	o[1]=fm2*fm1*fp1/(float)2;
	o[2]=fm2*f*fp1/(float)(-2);
	o[3]=fm1*f*fp1/(float)6;
}
void intpsub::vlgint(int n,float *f,float *x0,float *x1,float *x2,float *x3,float *x)
{
	float *a0,*a1,*a2,*a3,*b,o[4];

	if(*f<=0.){for(a1=x1,b=x;b<x+n;a1++,b++)*b=*a1;return;}
	if(*f>=1.){for(a2=x2,b=x;b<x+n;a2++,b++)*b=*a2;return;}

	lgcoef(f,o);

	for(a0=x0,a1=x1,a2=x2,a3=x3,b=x;b<x+n;a0++,a1++,a2++,a3++,b++)
	{
		*b= *a0 * o[0]
		+ *a1 * o[1]
		+ *a2 * o[2]
		+ *a3 * o[3];
	}
}
void intpsub::vlgint(int n,float *f,double *x0,double *x1,double *x2,double *x3,double *x)
{
        double *a0,*a1,*a2,*a3,*b;
        float o[4];

        if(*f<=0.){for(a1=x1,b=x;b<x+n;a1++,b++)*b=*a1;return;}
        if(*f>=1.){for(a2=x2,b=x;b<x+n;a2++,b++)*b=*a2;return;}


        lgcoef(f,o);

        for(a0=x0,a1=x1,a2=x2,a3=x3,b=x;b<x+n;a0++,a1++,a2++,a3++,b++)
        {
                *b= *a0 * o[0]
                + *a1 * o[1]
                + *a2 * o[2]
                + *a3 * o[3];
        }
}

void intpsub::lginta(int n,float *x,float *y,int *indx,float *tbl)
{
	int i,j,k;
	int j0,j1,j2,j3;

	for(i=0,k=0;i<n;i++,k+=4)
	{
		j=indx[i]-1;
		j0 = j;
		if(j0 <0) j0 = 0;
		j1 = j+1;

		j2 = j+2;
		j3 = j+3;
		if(i==n-1) j3 = j2;


		y[i]=x[j0]*tbl[k]+x[j1]*tbl[k+1]+x[j2]*tbl[k+2]+
                                 x[j3]*tbl[k+3];

		/*
			y[i]=x[j]*tbl[k]+x[j+1]*tbl[k+1]+x[j+2]*tbl[k+2]+
                                 x[j+2]*tbl[k+3];
								 */
		
	}
}

void intpsub::lgintp(int n,float *x,float *y,float *tbl)
{
	int i,j;
	float r,f,fm2,fm1,fp1,f1,f2,f3,f4;

    for(i=0;i<n;i++)
    {
        r=tbl[i];
        j=(int)r;
        f=r-j;
        fm2=f-(float)2;
        fm1=f-(float)1;
        fp1=f+(float)1;
        f1=fm2*fm1*f/(float)(-6);
        f2=fm2*fm1*fp1/(float)2;
        f3=fm2*f*fp1/(float)(-2);
        f4=fm1*f*fp1/(float)6;
        y[i]=x[j-1]*f1+x[j]*f2+x[j+1]*f3+x[j+2]*f4;
    }
}

void intpsub::lgeqsi(int n,int ni,float *x,float *y)
{
	float fi,f,o[4],*a,*b;
	int i;
	for(a=x,b=y;a<x+n;a++,b+=ni)*b=*a;
	if(ni<2)return;
	fi=(float)1/(float) ni;
	f=fi;
	for(i=1;i<ni;i++,f+=fi)
	{
		lgcoef(&f,o);
		for(a=x,b=y+i;a<x+n-1;a++,b+=ni)
		{
			*b= *(a-1) * o[0]
			+ *(a)   * o[1]
			+ *(a+1) * o[2]
			+ *(a+2) * o[3];
		}
	}
}

void intpsub::intpy(float *x, float *y, int m,int n)
{
 
	int i,*indxa;
	float tbi,spd,*tba,*tbl,frc;

	int npy;
	npy = n;
	tbl = new float[n*4];
	indxa = new int[n*4];
	spd = (float)(m)/(float)(n);
	

	for(i=0,tbi=0,tba=tbl;i<npy;i++,tbi+=spd,tba+=4)
	{
        	    indxa[i] = (int) tbi;
        	    frc=tbi-indxa[i];
        	    lgcoef(&frc,tba);
	}
	lgintaa(x,y,m,n,indxa,tbl);

	delete []indxa;
	delete []tbl;
	
}
void intpsub::intpy(float *x, float *y, int m,int n,float startS)
{
 
	int i,*indxa;
	float tbi,spd,*tba,*tbl,frc;

	int npy;
	npy = n;
	tbl = new float[n*4+10];
	indxa = new int[n*4+10];
	spd = (float)(m)/(float)(n);
	
// startS <1 >0;
	//for(i=0,tbi=0,tba=tbl;i<npy;i++,tbi+=spd,tba+=4)
    for(i=0,tbi=startS,tba=tbl;i<npy;i++,tbi+=spd,tba+=4)
	{
        	    indxa[i] = (int) tbi;
        	    frc=tbi-indxa[i];
        	    lgcoef(&frc,tba);
	}
	lgintaa(x,y,m,n,indxa,tbl);

	delete []indxa;
	delete []tbl;
	
}

void intpsub::lgintaa(float *x, float *y, int m, int n, int *indx, float *tbl)
{
	int i,j,k;
	int j0,j1,j2,j3;

	for(i=0,k=0;i<n;i++,k+=4)
	{
		j=indx[i]-1;
		j0 = j;
		if(j0 <0) j0 = 0;
		j1 = j+1;
		j2 = j+2;
		j3 = j+3;
		if(j1 >= m) j1 = m -1;
		if(j2 >= m) j2 = m -1;
		if(j3 >= m) j3 = m -1;

		//qDebug("j+3=%d,%d,%d,%d\n",j+3,j3,k+3,i);

		y[i]=x[j0]*tbl[k]+x[j1]*tbl[k+1]+x[j2]*tbl[k+2]+
                                 x[j3]*tbl[k+3];

 
	}
}
