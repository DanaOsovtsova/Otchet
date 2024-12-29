#include "DiagramTextItem.h"
#include "DiagramScene.h"

// Конструктор класса DiagramTextItem, который наследуется от QGraphicsTextItem
DiagramTextItem::DiagramTextItem(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    // Устанавливаем флаги, чтобы текстовый элемент можно было перемещать и выделять
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

// Переопределение виртуальной функции itemChange из базового класса
QVariant DiagramTextItem::itemChange(GraphicsItemChange change,
                                     const QVariant &value)
{
    // Если состояние элемента изменилось (был выделен или снят с выделения)
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        // Испускаем сигнал selectedChange с текущим элементом в качестве аргумента
        emit selectedChange(this);
    // Возвращаем значение, полученное от базового класса
    return value;
}

// Переопределение виртуальной функции focusOutEvent из базового класса
void DiagramTextItem::focusOutEvent(QFocusEvent *event)
{
    // Устанавливаем флаг, чтобы отключить редактирование текста
    setTextInteractionFlags(Qt::NoTextInteraction);
    // Испускаем сигнал lostFocus с текущим элементом в качестве аргумента
    emit lostFocus(this);
    // Вызываем реализацию базового класса
    QGraphicsTextItem::focusOutEvent(event);
}

// Переопределение виртуальной функции mouseDoubleClickEvent из базового класса
void DiagramTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    // Если текстовый элемент находится в режиме Qt::NoTextInteraction
    if (textInteractionFlags() == Qt::NoTextInteraction)
        // Устанавливаем режим Qt::TextEditorInteraction, чтобы разрешить редактирование текста
        setTextInteractionFlags(Qt::TextEditorInteraction);
    // Вызываем реализацию базового класса
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}
