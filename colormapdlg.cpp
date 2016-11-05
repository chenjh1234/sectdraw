#include "colormapdlg.h"

colorMapDlg::colorMapDlg(QWidget *w):QDialog(w)
{
    init();
}
void colorMapDlg::resizeEvent ( QResizeEvent *e )
{
    int x;
    x = e->size().width();
    drawColorBar();
}
void colorMapDlg::drawColorBar ()
{
    int w,h,i;
    w = v->viewport()->size().width();
    h = v->viewport()->size().height();
    v->setSceneRect(0,0,w,h);

 //colorBar:

    for (i = 0; i <num; i++)
    {

        cb[i]->setSize(w,h/num-1);
        cb[i]->setPos(0,i*(float(1.0*h/(1.0*num))));
    }
}
void colorMapDlg::init()
{
    int i;
    colorBar cc;
    num = cc.m_iMaps;
    r = new  QRadioButton *[num];
    cb = new colorBar *[num];

//lay out

    QGridLayout *lo;
    lo = new QGridLayout(this);
//pushbutton 3


    ok = new QPushButton("ok",this);
    apply = new QPushButton("apply",this);
    exit = new QPushButton("exit",this);

    lo->addWidget(ok,num,0,1,1);
    lo->addWidget(apply,num,1,1,1);
    lo->addWidget(exit,num,2,1,1);
// radioButton num



    for (i = 0; i <num; i++)
    {
        r[i] = new QRadioButton(this);
        //r[i]-
        lo->addWidget(r[i],i,2,1,1);
    }
//  qgraphicsView;

    v = new colorView(this);
    lo->addWidget(v,0,0,num,2);


    sc = new  QGraphicsScene(this);

    v->setScene(sc);
    int w,h;
    w = v->viewport()->size().width();
    h = v->viewport()->size().height();

   // v->setSceneRect(0,0,w,h);

 //colorBars:

    for (i = 0; i <num; i++)
    {
        cb[i] = new colorBar(0);
        sc->addItem(cb[i]);
        cb[i]->setColor(i);
        r[i]->setText(cb[i]->m_names[i]);

    }
     resize(200,200);
     //insure fist time show .the colorBar appeared
    // show();
     hide();
     drawColorBar ();
     connect(exit,SIGNAL(clicked()),this,SLOT(reject()));
     connect(ok,SIGNAL(clicked()),this,SLOT(accept()));
     connect(apply,SIGNAL(clicked()),this,SLOT(applySlot()));
     connect(v,SIGNAL(sig_mouse()),this,SLOT(mouseSlot()));
     for(i = 0; i <num;i++)
     {
         connect(r[i],SIGNAL(clicked()),this,SLOT(applySlot()));
     }
     apply->hide();
     ok->hide();
     setWindowTitle ("Select color table" );
    return;
}
void colorMapDlg::applySlot()
{
    int i;
    i = getIdx();
    index = i;
    emit sig_apply(i);
}
int colorMapDlg::getIdx()
{
    int i;
    for(i = 0; i <num; i++)
    {
        if(r[i]->isChecked()) return i;
    }
    return 0;
}
void colorMapDlg::mouseSlot()
{
    int i;
    for(i = 0; i <num;i++)
    {
        if(cb[i]->isSelected())
        {
            r[i]->setChecked(1);
            applySlot();
            return;
        }
    }
    return;
}

colorView::colorView ( QWidget *w):QGraphicsView(w)
{

}
// we canot use pressed here,oherwise the colorbar in view canot selected;
// mybe colorber selected use button release;
void colorView::mouseReleaseEvent ( QMouseEvent *e )
{
    QGraphicsView::mouseReleaseEvent(e);
    emit sig_mouse();
}
