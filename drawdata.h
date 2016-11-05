#ifndef DRAWDATA_H
#define DRAWDATA_H
#include <Qt>
#include <QString>
#include <QColor>
#include <QTime>
#include <QDebug>

#define Z_TOP 5
#define Z_TOP_1 4

/* coordinate of the view ,Right is the posive x direction,down is the posive y direction
  (0,0) is the topleft corner
  */
#define RIGHT_DOWN 0

/* coordinate of the view ,Right is the posive x direction,UP is the posive y direction
  (0,0) is the bottomleft corner
  */
#define RIGHT_UP 1

///  base class for the data , virtual class
/**
     supply data for drawFrame,drawing,rulerframe and its desendents;\n
     two virtual method:\n
        virtual qreal labMethodX(qreal x)=0;\n
        virtual qreal labMethodY(qreal x)=0;\n
  */
class drawData
{
public:
    drawData();
    /// parameters for the ruler wide:
    struct RULERDATA
    {
        int topW,bottomW,leftW,rightW;//wide of the ruler label
        //int step1,step2; //step1: short line interval;step2:long line interval and mark the label ,
    }m_ruler;
    /// parameters for the viewport wide and height
    struct FRAMEDATA
    {
        //viewport wide and height
        int width,height;
    }m_frame;
    /// parameters for the Scene rectangle
    struct SCENEDATA
    {
        int x,y,width,height;
    }m_scene;
    /// parameters for the Grid  in 2 dirction and 2 type
    struct XYGRID
    {//draw grid for ruler and m_draw items:
        // setGrid change the grid parameters if x1==0,
        //we use default
        qreal x1,x2,y1,y2;//scene data step of the grid
        // the default grid parameter:
        int ix1,ix2,iy1,iy2;//pixal step of the gridx
        bool grid;//true if draw grid in m_draw;
    }m_xygrid;


    /**return the currenttime  : msg   */
    QString prTime(QString msg );
    /** get the pen style ,give a int return a Qt::Style=0-6*/
    Qt::PenStyle getPenStyle(int i);
    /** get the pen style ,give a int return a Qt::Style=0-15*/
    Qt::BrushStyle getBrushStyle(int i);
    /// get pen color
    QColor getPenColor(int i);

    /** print string len :float precision :fracture */
    QString printStr(float f,int len =0);
    /** print string len :float precision :fracture */
    QString printStr(int i,int len = 0);

    /** set data precision for float data*/
    void setDataPrecision(int x,int y=-1);
    /** A pure virtual member: horizon label number calculate*/
    virtual qreal labMethodX(qreal x)=0;
    /** A pure virtual member: vertical label number calculate*/
    virtual qreal labMethodY(qreal x)=0;

/**
  range of the data;
  */
    qreal minX,minY,maxX,maxY;

   ///   start to open data;
    bool m_bStart;
    bool m_bOutlook;// automatic outlook
    bool m_bOutlookNO;// reject outlook

/** coordinate system
      RIGHT_DOWN 0
      RIGHT_UP 1
:*/
    int m_coordinate;
//private:
    void init();
    ///  for float precision
    int m_iDataPrecision ;
    int m_iDataPrecisionX;
    int m_iDataPrecisionY;

    /** scale: pixals beween the next two data;
        data scale: multiplyed by   :default = 1.0
        real x = x*m_fScaleX*m_fDataScale
        we never used it now
*/
    //qreal m_fScaleX,m_fScaleY,m_fDataScale;


};

#endif // DRAWDATA_H
