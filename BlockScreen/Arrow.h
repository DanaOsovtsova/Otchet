#ifndef ARROW_H
#define ARROW_H

#include "DiagramItem.h"

class Arrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    Arrow(DiagramItem *startItem, DiagramItem *endItem,
          QGraphicsItem *parent = nullptr);

    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void setColor(const QColor &color) { myColor = color; }
    DiagramItem *startItem() const { return myStartItem; }
    DiagramItem *endItem() const { return myEndItem; }

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

private:
    DiagramItem *myStartItem;
    DiagramItem *myEndItem;
    QPolygonF arrowHead;
    QColor myColor = Qt::black;
};

#endif // ARROW_H
