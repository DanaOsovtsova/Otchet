#include <QGraphicsPolygonItem>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QVector>
#include <QPixmap>
#include <QPainter>

#include "Arrow.h"
#include "QtWidgets/qgraphicsscene.h"
#include "DiagramItem.h"
#include "DiagramScene.h"

class Arrow;

DiagramItem::DiagramItem(DiagramType diagramType, QMenu *contextMenu,
                         QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent), myDiagramType(diagramType)
    , myContextMenu(contextMenu)
{
    QPainterPath path;
    switch (myDiagramType) {
    case StartEnd:
        path.moveTo(200, 50);
        path.arcTo(150, 0, 50, 50, 0, 90);
        path.arcTo(50, 0, 50, 50, 90, 90);
        path.arcTo(50, 50, 50, 50, 180, 90);
        path.arcTo(150, 50, 50, 50, 270, 90);
        path.lineTo(200, 25);
        myPolygon = path.toFillPolygon();
        break;
    case Conditional:
        myPolygon << QPointF(-100, 0) << QPointF(0, 100)
                  << QPointF(100, 0) << QPointF(0, -100)
                  << QPointF(-100, 0);
        break;
    case Step:
        myPolygon << QPointF(-100, -100) << QPointF(100, -100)
                  << QPointF(100, 100) << QPointF(-100, 100)
                  << QPointF(-100, -100);
        break;
    default:
        myPolygon << QPointF(-120, -80) << QPointF(-70, 80)
                  << QPointF(120, 80) << QPointF(70, -80)
                  << QPointF(-120, -80);
        break;
    }
    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}


void DiagramItem::removeArrow(Arrow *arrow)
{
    arrows.removeAll(arrow);
}

void DiagramItem::removeArrows()
{
    const auto arrowsCopy = arrows;
    for (Arrow *arrow : arrowsCopy) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void DiagramItem::addArrow(Arrow *arrow)
{
    arrows.append(arrow);
}

QPixmap DiagramItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(myPolygon);

    return pixmap;
}

void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
//Очищает текущее выделение на сцене.
//Выделяет текущий элемент DiagramItem.
//Отображает контекстное меню myContextMenu в позиции курсора.
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        for (Arrow *arrow : std::as_const(arrows))
            arrow->updatePosition();
    }

    return value;
}
