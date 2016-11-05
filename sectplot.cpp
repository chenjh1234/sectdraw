// btrplt.cpp: implementation of the btrplt class.
//
//////////////////////////////////////////////////////////////////////

#include "sectplot.h"
//#include "qt.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

sectplot::sectplot()
{
    ix1 = 0;
    ix2 = 0;
    yrs = 0;

}

sectplot::~sectplot()
{

}


void sectplot::pltinit(int mx,int my,int nbpl,unsigned char *a,unsigned char pixel)
{
	int bmpb,npx;

	npx= mx;
	bmpb = 4;
	nbpl=((npx+bmpb-1)/bmpb)*bmpb;

	pb.mx=mx;
	pb.my=my;
	

	pb.nbpl=nbpl;
	pb.a =a;
	pb.pixel=pixel;
	return;
}


int sectplot::pltwiggle(int xs,int ys,int n,int *ibf)
{
	int x1,x2;
	int *ib,*b;
    if(pb.a==NULL)return(-1);
    ib=ibf;
    if(ys<0)ib-=ys;ys=0;
    if(ys+n-1>pb.my)n=pb.my-ys-1;
    x1=*ib;
    yrs=ys*pb.nbpl;
    for(b=ib+1;b<ib+n;b++)
    {
        x2=*b;
        if(x1==x2)
        {
            ix1=xs+x1;
            ix2=ix1;
            hline(pb.pixel);
            yrs+=pb.nbpl;
            hline(pb.pixel);
            continue;
        }
        if(x2<x1)
        {
            ix2=xs+x1;
            ix1=ix2-((x1-x2)>>1);
            hline(pb.pixel);
            yrs+=pb.nbpl;
            ix2=ix1-1;
            ix1=xs+x2;
            hline(pb.pixel);
        }
        else
        {
            ix1=xs+x1;
            ix2=ix1+((x2-x1)>>1);
            hline(pb.pixel);
            yrs+=pb.nbpl;
            ix1=ix2+1;
            ix2=xs+x2;
            hline(pb.pixel);
        }
        x1=x2;
    }
    return(0);
}

int sectplot::pltpa(int xs,int ys,int n,int *ibf)
{
	int *ib,*b;
    if(pb.a==NULL)return(-1);
    ib=ibf;
    if(ys<0)ib=ib-ys;ys=0;
    if(ys+n-1>pb.my)n=pb.my-ys-1;
    yrs=ys*pb.nbpl;
    ix1=xs;
    for(b=ib;b<ib+n;b++)
    {
        if(*b>=0)
        {
            ix2=ix1+*b;
            hline(pb.pixel);
        }
        yrs+=pb.nbpl;
    }
    return(0);

}

int sectplot::pltma(int xs,int ys,int n,int *ibf)
{
	int *ib,*b;
    if(pb.a==NULL)return(-1);
    ib=ibf;
    if(ys<0)ib=ib-ys;ys=0;
    if(ys+n-1>pb.my)n=pb.my-ys-1;
    yrs=ys*pb.nbpl;
    ix2=xs;
    for(b=ib;b<ib+n;b++)
    {
        if(*b<=0)
        {
            ix1=ix2+*b;
            hline(pb.pixel);
        }
        yrs+=pb.nbpl;
    }
    return(0);

}

int sectplot::pltvd(int xs,int ys,int n,int *ibf,unsigned char *pix)
{
	int *ib,*b;
	unsigned char *px,*p;
    if(pb.a==NULL)return(-1);
    ib=ibf;
    px=pix;
    if(ys<0)
    {
   		ib-=ys;
        px-=ys;
		ys=0;
    }
    if(ys+n-1>pb.my)n=pb.my-ys-1;
    yrs=ys*pb.nbpl;
    for(b=ib,p=px;b<ib+n;b++,p++)
    {
        if(*b>=0)
        {
			ix1=xs;
            ix2=ix1+*b;
            hline(*p);
        }
		else
        {
			ix2=xs;
            ix1=ix2+*b;
            hline(*p);
        }
        yrs+=pb.nbpl;
    }
    return(0);

}
int sectplot::pltvd(int xs,int ys,int n,unsigned char *pix)
{
 
	int i;
	unsigned char *px,*p;
    if(pb.a==NULL)return(-1);
 
    px=pix;
    if(ys<0)
    {
        px-=ys;
		ys=0;
    }
    if(ys+n-1>pb.my)n=pb.my-ys-1;
    yrs=ys*pb.nbpl;
    for(i=0,p=px;i<n;p++,i++)
    {
         
		ix1=xs;
        ix2=ix1;
        hline(*p);
         
        yrs+=pb.nbpl;
    }
    return(0);

}
void sectplot::hline(char pix)
{
	unsigned char *as,*ae,*a,*sb;
    if(ix1<0) ix1=0;
    if(ix1>pb.mx) ix1=pb.mx -1;

    if(ix2 < 0) ix2 = 0;// 2015.8.26 add
	if(ix2>= pb.mx)
		ix2=pb.mx -1;// insure cannot over
    if(yrs > (pb.my-1) * pb.nbpl )
		yrs = (pb.my-1) * pb.nbpl;

    sb=pb.a+yrs;
    as=sb+ix1;
    ae=sb+ix2;
    for(a=as;a<=ae;a++)
		*a=pix;
}
unsigned char * sectplot::raster( int nbpl,int nl)
{
	unsigned char *ras;
	int i;
	int white = 0;
	int mem;
    mem=nbpl*nl;
    if((ras= new  unsigned char[mem])==NULL)return(NULL);
    for(i=0;i<mem;i++)ras[i]=white; 
    return(ras);
}  
void sectplot::hline(int ix,int iy,int lg,
		int nbpl,int col,unsigned char *bits)
{
	int i,ist;
    if(lg==0)return;
    if(lg<0)
    {
        lg=-lg;
		ix=ix-lg+1;
    }
    ist=iy*nbpl+ix;
    for(i=ist;i<ist+lg;i++) bits[i]=col;
}

void sectplot::hlined(int ix,int iy,int lg,
		int nbpl,int col,unsigned int bmask,unsigned char *bits)
{
	int i,ist,bc;
	unsigned int bm=0x80000000;
    if(lg==0)return;

    if(lg<0)
    {
        lg=-lg;
		ix=ix-lg+1;
    }

    bc=ix%32;
    ist=iy*nbpl+ix;
    for(i=ist;i<ist+lg;i++)
    {
	 
		if(((bm>>bc) & bmask)!=0)
			bits[i]=col;

// 	qDebug("i = %d\n",i);

		bc++;
		if(bc==32)bc=0;
    }
}

void  sectplot::vline(int ix,int iy,int lg,
		int nbpl,int col,unsigned char *bits)
{
	int i,j,ist;
    if(lg==0)return;
    if(lg<0)
    {
        lg=-lg;
		iy=iy-lg+1;
    }
    ist=iy*nbpl+ix;

    for(i=0,j=ist;i<lg;i++,j+=nbpl)
		bits[j]=col;
}

void  sectplot::vlined(int ix,int iy,int lg,
		int nbpl,int col,unsigned int bmask,unsigned char *bits)
{
	int i,j,ist,bc;
	unsigned int bm=0x80000000;
    if(lg==0)return;
    if(lg<0)
    {
        lg=-lg;
		iy=iy-lg+1;
    }

    bc=iy%32;
    ist=iy*nbpl+ix;

    for(i=0,j=ist;i<lg;i++,j+=nbpl)
    {
        if(((bm>>bc) & bmask)!=0)
                bits[j]=col;
        bc++;
        if(bc==32)	bc=0;
    }
}

void  sectplot::fillrect(int ix,int iy,int lx,int ly,
        int nbpl,int col,unsigned char *bits)
{
int i,j,ist;
    for(j=iy;j<iy+ly;j++)
    {
	ist=j*nbpl+ix;
	for(i=ist;i<ist+lx;i++)bits[i]=col;
    }
}
