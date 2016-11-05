#ifndef MESSAGEPAD_H
#define MESSAGEPAD_H

#include <QGraphicsTextItem>
#include <QPainter>
#include <QDebug>
/// a panal at upper position in top ruler,for display test.
class messagePad : public QGraphicsTextItem
{

public:
    explicit messagePad(QGraphicsItem  *parent = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
   // QRectF boundingRect()const;

};

#endif // MESSAGEPAD_H
