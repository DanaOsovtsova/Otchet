#include "DiagramScene.h"
#include "Arrow.h"
#include <QGraphicsScene>
#include <QTextCursor>
#include <QList>

#include "diagramscene.h"
#include "arrow.h"

#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>


DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;
    myItemType = DiagramItem::Step;
    line = nullptr;
    textItem = nullptr;
    myItemColor = Qt::white;
    myTextColor = Qt::black;
    myLineColor = Qt::black;
}

void DiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    if (isItemChange(Arrow::Type)) {
        Arrow *item = qgraphicsitem_cast<Arrow *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}

void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if (isItemChange(DiagramTextItem::Type)) {
        DiagramTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
    }
}

void DiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    if (isItemChange(DiagramItem::Type)) {
        DiagramItem *item = qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
        item->setBrush(myItemColor);
    }
}

void DiagramScene::setFont(const QFont &font)
{
    myFont = font;

    if (isItemChange(DiagramTextItem::Type)) {
        QGraphicsTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        if (item)
            item->setFont(myFont);
    }
}

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void DiagramScene::setItemType(DiagramItem::DiagramType type)
{
    myItemType = type;
}

void DiagramScene::editorLostFocus(DiagramTextItem *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // Если нажата не левая кнопка мыши, то ничего не делаем
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    // Инициализируем указатель на DiagramItem
    DiagramItem *item;
    // В зависимости от режима (myMode) выполняем разные действия
    switch (myMode) {
        case InsertItem:
        // Создаём новый DiagramItem, устанавливаем его кисть и добавляем на сцену
        item = new DiagramItem(myItemType, myItemMenu);
        item->setBrush(myItemColor);
        addItem(item);
        // Устанавливаем позицию нового элемента в то место, где был клик мышью
        item->setPos(mouseEvent->scenePos());
        // Испускаем сигнал об добавлении нового элемента
        emit itemInserted(item);
        break;
    case InsertLine:
        if (line) { // Если линия уже существует
            // Завершаем линию
            line->setLine(QLineF(lineStartPos, mouseEvent->scenePos()));
            line = nullptr; // Сбрасываем указатель на линию
            // Испускаем сигнал о завершении линии
           //emit itemInserted(line);
        } else {
            // Запоминаем начальную точку линии
            lineStartPos = mouseEvent->scenePos();
            // Создаём новую линию от начальной точки до текущей позиции
            line = new QGraphicsLineItem(QLineF(lineStartPos, mouseEvent->scenePos()));
            // Устанавливаем цвет линии
            line->setPen(QPen(myLineColor, 2));
            // Добавляем линию на сцену
            addItem(line);
        }
        break;

    case InsertText:
        // Создаём новый DiagramTextItem, устанавливаем его шрифт и режим редактирования текста
        textItem = new DiagramTextItem();
        textItem->setFont(myFont);
        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        // Устанавливаем z-индекс текстового элемента, чтобы он был поверх других элементов
        textItem->setZValue(1000.0);
        // Подключаем сигналы потери фокуса и изменения выделения
        connect(textItem, &DiagramTextItem::lostFocus,
                this, &DiagramScene::editorLostFocus);
        connect(textItem, &DiagramTextItem::selectedChange,
                this, &DiagramScene::itemSelected);
        // Добавляем текстовый элемент на сцену, устанавливаем его цвет и позицию
        addItem(textItem);
        textItem->setDefaultTextColor(myTextColor);
        textItem->setPos(mouseEvent->scenePos());
        // Испускаем сигнал о вставке текстового элемента
        emit textInserted(textItem);
    default:
        ;
    }
    // Вызываем обработчик базового класса
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // Если режим - рисование линии и линия уже создана
    if (myMode == InsertLine && line != nullptr) {
        // Обновляем конечную точку линии на позицию курсора
        QLineF newLine(lineStartPos, mouseEvent->scenePos());
        line->setLine(newLine);
    }
    // Если режим - перемещение элементов
    else if (myMode == MoveItem) {
        // Вызываем обработчик базового класса
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}


void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // Если линия существует и режим - рисование линии
    if (line != nullptr && myMode == InsertLine) {
        // Получаем список элементов, находящихся в начальной и конечной точках линии
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        QList<QGraphicsItem *> endItems = items(line->line().p2());

        // Проверяем, что в начальной и конечной точках есть как минимум один элемент
        if (!startItems.isEmpty() && !endItems.isEmpty()) {
            // Проверяем, что первые элементы в списках - типа DiagramItem
            if (startItems.first()->type() == DiagramItem::Type && endItems.first()->type() == DiagramItem::Type) {
                // Приводим первые элементы списков к типу DiagramItem
                DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
                DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.first());

                // Проверяем, что это разные элементы
                if (startItem != endItem) {
                    // Создаём новую стрелку, связывающую эти два DiagramItem-а
                    // Создаём новую стрелку, связывающую эти два DiagramItem-а
                    Arrow *arrow = new Arrow(startItem, endItem);
                    arrow->setColor(myLineColor);
                    startItem->addArrow(arrow);
                    endItem->addArrow(arrow);
                    arrow->setZValue(-1000.0);
                    this->addItem(arrow);
                    arrow->updatePosition();
                }
            }
        }

        // Удаляем линию из сцены и очищаем указатель
        removeItem(line);
        delete line;
        line = nullptr;
    }

    // Вызываем обработчик базового класса
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}



bool DiagramScene::isItemChange(int type) const
{
    // Получаем список выделенных элементов на сцене
    const QList<QGraphicsItem *> items = selectedItems();
    // Создаём лямбда-функцию, которая проверяет, является ли элемент типа type
    const auto cb = [type](const QGraphicsItem *item) { return item->type() == type; };
    // Ищем в списке выделенных элементов хотя бы один, соответствующий типу type
    return std::find_if(items.begin(), items.end(), cb) != items.end();
}

