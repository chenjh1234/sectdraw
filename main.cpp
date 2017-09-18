#include <QApplication>
#include "widget.h"
#include "../gridata/gridataadi.h"
int testGri()
{
    griData g;
    int i;
    char filen[256];
    strcpy(filen,"d:\\data\\xj.s175.g0shot10");
    i = g.open_read(filen,2,3000);
    qDebug("i = %d\n",i);
    int num;
    num = g.listGroup[0].num;
    int *head;
    float *trace;
    head = new int[128*num];
    trace = new float[g.getSamples()*num];

    for(i = 0; i <num ;i++)
    {
        g.read_a_trace(head +128*i,trace+g.getSamples()*i);
    }
    return 0;

}

int main(int argc, char *argv[])
{
   // testGri();
   // exit(1);
    QApplication a(argc, argv);
    Widget w;
    w.resize(500,500);
    w.show();

    return a.exec();
}
