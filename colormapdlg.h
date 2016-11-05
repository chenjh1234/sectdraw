#ifndef COLORMAPDLG_H
#define COLORMAPDLG_H
#include <QDialog>
#include <QPushButton>
#include <QRadioButton>
#include <QGridLayout>
#include "colorbar.h"
#include <QGraphicsView>
#include <QGraphicsScene>

/// view for colorMapDlg

class colorView :public QGraphicsView
{
    Q_OBJECT
public:
    colorView(QWidget *w);
    void mouseReleaseEvent( QMouseEvent *e );
signals:
    /// emit when mouse released in the view
    void sig_mouse();


};

/// dialog to select color table in color bar
class colorMapDlg :public QDialog
{
    Q_OBJECT

public:
    colorMapDlg(QWidget *w);

private:
    int getIdx();
    colorView *v;
    QGraphicsScene  *sc;
    QPushButton *ok,*exit,*apply;
    QRadioButton **r;
    colorBar **cb;
    /// current index;
    int index;
    /// number of colorBar;
    int num;
    void drawColorBar ();
    void resizeEvent ( QResizeEvent *e );
    void init();

public slots:
/** click the radio button,
    get the checked radiobutton,
        and emit sig_apply signal
*/
    void applySlot();
/**
  clicked a colorbar,
    get the selected colorbar ,
    check the and radiobutton ,
    and emit sig_apply signal
*/
    void mouseSlot();
signals:
    /// emit when button pressed,
    void sig_apply(int id);

};

#endif // COLORMAPDLG_H
