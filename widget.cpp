#include "widget.h"
#include "ui_widget.h"
#include "drawframe.h"
#include <QDebug>
#include <QGridLayout>
#include "../gridata/gridataadi.h"
#include "../segdata/qsegyADI.h"

#include "../segdata/qseg2Read.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //testArray();
     testSect();
     //testSegy();
    //testSegd();
   // testSeg2();


}

Widget::~Widget()
{
    delete ui;
}
void Widget::testSect()
{
    QGridLayout *ly;
    ly = new QGridLayout(this);
    sgv = new sectDraw(this);
    ly->addWidget(sgv);

 //data:
    griData g;
    int i;
    char filen[256];
    strcpy(filen,"d:\\data\\xj.s175.g0shot10");
    strcpy(filen,"e:\\segb\\FLD_BP8122710173.GRI");
    i = g.open_read(filen,2,3000);
    qDebug("i = %d\n",i);
    int num;
    num = g.ptr_group[0].num;
    int *head;
    float *trace;
    num = 96*2;

    head = new int[128*num];
    trace = new float[g.sam*num];


    for(i = 0; i <num ;i++)
    {
        g.read_a_trace(head +128*i,trace+g.sam*i);
    }

    //sgv->setDrawType(DRAW_WA);
    sgv->setGain(99);
    qDebug()<<"max.min="<< num<<g.sam;
    sgv->setMaxMin(0,0,num,g.sam);
    sgv->setColorBar(1);
    sgv->setDataPara(g.sam,g.si,128);
    sgv->setScale( 1,1);
    sgv->setDrawType(DRAW_VD);
    //sgv->setDrawType(DRAW_WA);
    sgv->startDraw(head,trace,num);
    sgv->show();

}

void Widget::testArray()
{
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
    gv->setScale(5,5);
    gv->startDraw(f,200,key);//first array

    gv->setDrawStyle(0,2,1,0);
    gv->startDraw(f1,200,key2);// second array

    gv->setDrawStyle(1,1,1,0);
    gv->startDraw(f1,50,"");//3rd array,len = 50
    gv->show();
    //delete f;
}
void Widget::testSegd()
{
    QGridLayout *ly;
    ly = new QGridLayout(this);
    sgv = new sectDraw(this);
    ly->addWidget(sgv);

 //data:

    int i;
    char filen[256];
    strcpy(filen,"d:\\data\\app.sgy");
    strcpy(filen,"d:\\diskE\\segd_data\\190.dat");
    strcpy(filen,"d:\\data\\segd\\PDO92AMZ1-00427.SEGD.TIF8");
    qDebug() << "file = " <<filen;
    QString s;
    s = filen;
    i = g.InitTape(s);
    qDebug("i = %d\n",i);
    qDebug()<< "trs In widget ="<< g.m_iTrs << g.m_iAux << g.m_iTrsAll;
   // exit(1);
    if(i <=0 )
        exit(1);

    int num,sam,si;
    num = g.m_iTrs;
    //num = 120;
    sam = g.m_iSamples;
    si = g.m_iSI/1000;
    int *head;
    float *trace;
     num = 1000;

    head = new int[128*num];
    trace = new float[sam*num];
    qDebug()<< "trs In widget ="<< g.m_iTrs<<g.m_iAux<<g.m_iTrsAll;
    qDebug() << "para = "<< sam<<si<<g.m_iTrs <<g.m_iAux <<g.m_iTrsAll <<g.m_iShotsInFile;


    for(i = 0; i <num ;i++)
    {

         g.ReadTrace(trace+sam*i,sam);
         //qDebug()<< "read trace" << i<< g.m_icShot;
         if(g.m_icShot >=4)
         {
             num = i;
             qDebug() << "num = " << num;
             break;
         }
    }

/*
    int j;
    for(i = 0; i <num;i++)
        for(j =0;j <g.sam;j++)
            trace[i*g.sam + j] = j;
*/
    sgv->setMaxMin(0,0,num,sam);
    sgv->setColorBar(1);
    sgv->setDataPara(sam,si,128);
    sgv->startDraw(head,trace,num);
    sgv->show();

}
void Widget::testSegy()
{
    QGridLayout *ly;
    ly = new QGridLayout(this);
    sgv = new sectDraw(this);
    ly->addWidget(sgv);

 //data:
    QSegyADI g;
    int i;
    char filen[256];
    strcpy(filen,"d:\\data\\app.sgy");
    strcpy(filen,"d:\\data\\xj.s175.y0shot10");
    strcpy(filen,"d:\\data\\segd\\segd.sgy");
    strcpy(filen,"e:\\segb\\FLD_BP8122710173.SGY");
    i = g.openRead(filen);
    qDebug("i = %d\n",i);
    int num,sam,si;
    num = g.m_iAllTraces;
    num = 96*3;
    sam = g.m_iSamples;
    si = g.m_iSI/1000;
    int *head;
    float *trace;

    head = new int[128*num];
    trace = new float[sam*num];


    for(i = 0; i <num ;i++)
    {
         g.readTrace(head +128*i,trace+sam*i);
        //g.ReadTrace(trace+sam*i,sam);
    }
/*
    int j;
    for(i = 0; i <num;i++)
        for(j =0;j <g.sam;j++)
            trace[i*g.sam + j] = j;
*/
    sgv->setMaxMin(0,0,num,sam);
    sgv->setColorBar(1);
    sgv->setDataPara(sam,si,128);
    sgv->startDraw(head,trace,num);

    sgv->show();

}

void Widget::testSeg2()
{
    QGridLayout *ly;
    ly = new QGridLayout(this);
    sgv = new sectDraw(this);
    ly->addWidget(sgv);

 //data:
    QSeg2Read g;
    int i;
    char filen[256];
    strcpy(filen,"d:\\data\\app.sgy");
    strcpy(filen,"d:\\diskE\\segd_data\\190.dat");
    strcpy(filen,"d:\\data\\seg2\\8001.dat");
    QString s;
    s = filen;
    QStringList list;
    list.append(s);
    i = g.setFiles(list);
    g.startRead();
    g.openFile();
    i = g.readFileHeader();


    qDebug("i = %d\n",i);
    //if(i <=0 )
    //    exit(1);

    int num,sam,si;

    num = g.m_iNumberOfTraces;
    //num = 120;
    sam = g.m_iSamples;
    si = 2;
    int *head;
    float *trace;

    head = new int[128*num];
    trace = new float[sam*num];


    for(i = 0; i <num ;i++)
    {

         g.readATrace(i,trace+i*sam);

    }
/*
    int j;
    for(i = 0; i <num;i++)
        for(j =0;j <g.sam;j++)
            trace[i*g.sam + j] = j;
*/
    sgv->setMaxMin(0,0,num,sam);
    sgv->setColorBar(1);
    sgv->setDataPara(sam,si,128);
    sgv->startDraw(head,trace,num);
    sgv->show();

}
/** @example   
void Widget::testSect()\n
{\n
    QGridLayout *ly;\n
    ly = new QGridLayout(this);\n
    sgv = new sectDraw(this);\n
    ly->addWidget(sgv);\n
\n
 //data:\n
    griData g;\n
    int i;\n
    char filen[256];\n
    strcpy(filen,"d:\\data\\xj.s175.g0shot10");\n
    strcpy(filen,"e:\\segb\\FLD_BP8122710173.GRI");\n
    i = g.open_read(filen,2,3000);\n
    qDebug("i = %d\n",i);\n
    int num;\n
    num = g.ptr_group[0].num;\n
    int *head;\n
    float *trace;\n
    num = 96*2;\n
\n
    head = new int[128*num];\n
    trace = new float[g.sam*num];\n
\n   
    for(i = 0; i <num ;i++)\n
    {\n
        g.read_a_trace(head +128*i,trace+g.sam*i);\n
    }\n
\n
    ////sgv->setDrawType(DRAW_WA);\n
    sgv->setGain(09);\n
    qDebug()<<"max.min="<< num<<g.sam;\n
    sgv->setMaxMin(0,0,num,g.sam);\n
    sgv->setColorBar(1);\n
    sgv->setDataPara(g.sam,g.si,128);\n
    sgv->setScale( 1,1);\n
    sgv->setDrawType(DRAW_VD);\n
    ////sgv->setDrawType(DRAW_WA);\n
    sgv->startDraw(head,trace,num);\n
    sgv->show();\n
\n
}\n
*/
