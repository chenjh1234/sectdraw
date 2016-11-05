#-------------------------------------------------
#
# Project created by QtCreator 2011-08-12T08:34:11
#
#-------------------------------------------------


TARGET = Draw
TEMPLATE = app

CONFIG += debug
INCLUDEPATH += .
QT += widgets
QT += testlib

SOURCES += main.cpp\
        widget.cpp \
    drawframe.cpp \
    rulerframe.cpp \
    drawdata.cpp \
    drawscene.cpp \
    drawing.cpp \
    arraydraw.cpp \
    arraydata.cpp \
    arraypaint.cpp \
    messagepad.cpp \
    drawitem.cpp \
    colorbar.cpp \
    sectplot.cpp \
    intpsub.cpp \
    sectDraw.cpp \
    sectData.cpp \
    sectpaint.cpp \
    ../gridata/gridataadi.cpp \
    colormapdlg.cpp \
    ../segdata/QTapeIO.cpp \
    ../segdata/QSegyADI.cpp \
    ../segdata/QSegCommon.cpp \
    ../segdata/QSeg2Write.cpp \
    ../segdata/QSeg2Read.cpp \
    ../segdata/QSeg2.cpp \
    ../segdata/QSegdRead.cpp \
    ../segdata/segd2gri.cpp

HEADERS  += widget.h \
    drawframe.h \
    rulerframe.h \
    drawdata.h \
    drawscene.h \
    drawing.h \
    arraydraw.h \
    arraydata.h \
    arraypaint.h \
    messagepad.h \
    drawitem.h \
    colorbar.h \
    intpsub.h \
    sectplot.h \
    sectData.h \
    sectDraw.h \
    sectpaint.h \
    ../gridata/gridataadi.h \
    colormapdlg.h \
    ../segdata/QTapeIO.h \
    ../segdata/QSegyADI.h \
    ../segdata/QSegCommon.h \
    ../segdata/QSeg2Read.h \
    ../segdata/QSeg2.h \
    ../segdata/QSeg2Write.h \
    ../segdata/QSegdRead.h \
    ../segdata/segd2gri.h

FORMS    += widget.ui
