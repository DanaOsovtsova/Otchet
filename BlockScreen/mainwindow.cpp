#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include "Arrow.h"
#include "QMainWindow"

#include <QHBoxLayout>
#include <QButtonGroup>
#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QToolBox>
#include <QPushButton>

#include "Arrow.h"
#include "DiagramItem.h"
#include "DiagramScene.h"
#include "DiagramTextItem.h"
#include "mainwindow.h"

#include <QtWidgets>

const int InsertTextButton = 10;

MainWindow::MainWindow()
{
    //отвечают за настройку элементов пользовательского интерфейса,
    //таких как действия, панели инструментов и меню, для главного окна
    createActions();
    createToolBox();
    createMenus();

    //В этом блоке создается новый объект DiagramScene и назначается переменной scene.
    //Размер сцены устанавливается на 5000x5000. Затем устанавливаются три соединения
    //сигнал-слот между DiagramScene и классом MainWindow. Эти соединения позволяют
    //MainWindow реагировать на события, такие как вставка элемента, вставка текста и выбор элемента в диаграммной сцене.
    scene = new DiagramScene(itemMenu, this);
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(scene, &DiagramScene::itemInserted,
            this, &MainWindow::itemInserted);
    connect(scene, &DiagramScene::textInserted,
            this, &MainWindow::textInserted);
    connect(scene, &DiagramScene::itemSelected,
            this, &MainWindow::itemSelected);


    //В этом блоке создается горизонтальный компоновщик, добавляется в него виджет toolBox и QGraphicsView (который отображает DiagramScene),
    //затем этот компоновщик устанавливается на новый виджет QWidget. Наконец, этот QWidget задается в качестве центрального виджета MainWindow.
    createToolbars();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(toolBox);
    view = new QGraphicsView(scene);
    layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowTitle(tr("BlockScreen"));
    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::backgroundButtonGroupClicked(QAbstractButton *button)
// принимает в качестве аргумента указатель на объект QAbstractButton, представляющий нажатую кнопку.
{
    const QList<QAbstractButton *> buttons = backgroundButtonGroup->buttons();
    //список всех кнопок, принадлежащих группе backgroundButtonGroup. Этот список сохраняется в константную переменную buttons.
    for (QAbstractButton *myButton : buttons) {
        if (myButton != button)
            button->setChecked(false);//Если кнопка не равна нажатой, то для нее устанавливается состояние "не выбрана"
    }

    QString text = button->text();
    //в зависимости от текста устанавливается цвет фона для DiagramScene (scene)
    if (text == tr("Blue Grid"))
        scene->setBackgroundBrush(QColor(0, 51, 102)); // Синий цвет
    else if (text == tr("White Grid"))
        scene->setBackgroundBrush(QColor(255, 255, 255)); // Белый цвет
    else if (text == tr("Gray Grid"))
        scene->setBackgroundBrush(QColor(128, 128, 128)); // Серый цвет
    else
        scene->setBackgroundBrush(QColor(64, 64, 64)); // Темно-серый цвет

    //перерисовки сцены и представления (view) вызываются их методы update().
    scene->update();
    view->update();
}

void MainWindow::buttonGroupClicked(QAbstractButton *button)
{

    const QList<QAbstractButton *> buttons = buttonGroup->buttons();
    for (QAbstractButton *myButton : buttons) {
        if (myButton != button)
            button->setChecked(false);
    }


    const int id = buttonGroup->id(button); //получается идентификатор нажатой кнопки с помощью метода id()
    if (id == InsertTextButton) {//устанавливаются разные режимы
        scene->setMode(DiagramScene::InsertText);
    } else {
        scene->setItemType(DiagramItem::DiagramType(id));//тип вставляемого элемента устанавливается в соответствии с идентификатором кнопки.
        scene->setMode(DiagramScene::InsertItem);
    }
}

void MainWindow::deleteItem()//отвечает за удаление выбранных элементов из диаграммной сцены.
{
    QList<QGraphicsItem *> selectedItems = scene->selectedItems();//получает список всех выбранных графических элементов на сцене
    for (QGraphicsItem *item : std::as_const(selectedItems)) { //проходит по списку выбранных элементов в цикле
        if (item->type() == Arrow::Type) { // проверяет, является ли он стрелкой
            scene->removeItem(item); //удаляет стрелку из диаграммной сцены.
            Arrow *arrow = qgraphicsitem_cast<Arrow *>(item); //приводит указатель на графический элемент к указателю на объект стрелки.
            arrow->startItem()->removeArrow(arrow); //удаляет ссылку на эту стрелку из начального элемента, который она соединял
            arrow->endItem()->removeArrow(arrow); //удаляет ссылку на эту стрелку из конечного элемента, который она соединяла.
            delete item; //удаляет сам объект стрелки.
        }
    }

    //После обработки всех стрелок, функция снова получает список выбранных графических элементов
    selectedItems = scene->selectedItems();
    //вызывает метод removeArrows() этого элемента, чтобы удалить все стрелки, связанные с ним.
    for (QGraphicsItem *item : std::as_const(selectedItems)) {
        if (item->type() == DiagramItem::Type)
            qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
        scene->removeItem(item); //удаляет элемент из диаграммной сцены.
        delete item; //удаляет сам объект элемента
    }
}

void MainWindow::pointerGroupClicked()
{
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    // устанавливает режим диаграммной сцены в соответствии с выбранной кнопкой в группе
}

void MainWindow::bringToFront() //переместить наверх
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    const QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    for (const QGraphicsItem *item : overlapItems) {
        if (item->zValue() >= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
}

void MainWindow::sendToBack() //переместить вниз
{
    if (scene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = scene->selectedItems().first();
    const QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    for (const QGraphicsItem *item : overlapItems) {
        if (item->zValue() <= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
}

void MainWindow::itemInserted(DiagramItem *item)
{
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(int(item->diagramType()))->setChecked(false);
}

void MainWindow::textInserted(QGraphicsTextItem *)
{
    buttonGroup->button(InsertTextButton)->setChecked(false);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void MainWindow::currentFontChanged(const QFont &)
{
    handleFontChange();
}

void MainWindow::fontSizeChanged(const QString &)
{
    handleFontChange();
}



void MainWindow::textColorChanged() //когда выбираем цвет, иконка меняет цвет на этот цвет
{
    textAction = qobject_cast<QAction *>(sender());//делается для того, чтобы можно было получить данные, связанные с этим действием.
    QColor textColor = qvariant_cast<QColor>(textAction->data()); //предполагается, что цвет текста был сохранен в качестве данных действия.
    //Функция qvariant_cast<QColor>() извлекает этот цвет из данных действия.
    fontColorToolButton->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                                           .arg(textColor.red())
                                           .arg(textColor.green())
                                           .arg(textColor.blue()));
    //задаём для свойства background-color значение, представляющее выбранный цвет текста. Для этого мы используем форматирование строки,
    //где %1, %2 и %3 заменяются на значения красного, зелёного и синего компонентов цвета соответственно.
    textButtonTriggered();
}

void MainWindow::itemColorChanged()
{
    fillAction = qobject_cast<QAction *>(sender());
    QColor fillColor = qvariant_cast<QColor>(fillAction->data());
    fillColorToolButton->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                                           .arg(fillColor.red())
                                           .arg(fillColor.green())
                                           .arg(fillColor.blue()));
    fillButtonTriggered();
}

void MainWindow::lineColorChanged()
{
    lineAction = qobject_cast<QAction *>(sender());
    QColor lineColor = qvariant_cast<QColor>(lineAction->data());
    lineColorToolButton->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                                           .arg(lineColor.red())
                                           .arg(lineColor.green())
                                           .arg(lineColor.blue()));
    lineButtonTriggered();
}

void MainWindow::textButtonTriggered()
{
    scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
}

void MainWindow::fillButtonTriggered()
{
    scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
}

void MainWindow::lineButtonTriggered()
{
    scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
}

void MainWindow::handleFontChange()
{
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());

    scene->setFont(font);
}

void MainWindow::itemSelected(QGraphicsItem *item)
{
    DiagramTextItem *textItem =
        qgraphicsitem_cast<DiagramTextItem *>(item);

    QFont font = textItem->font();
    fontCombo->setCurrentFont(font);
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    boldAction->setChecked(font.weight() == QFont::Bold);
    italicAction->setChecked(font.italic());
    underlineAction->setChecked(font.underline());
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Diagram Scene"),
                       tr("The <b>Diagram Scene</b> example shows "
                          "use of the graphics framework."));
}


void MainWindow::createToolBox()
{
    // Создаем группу кнопок для элементов диаграммы
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
    connect(buttonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &MainWindow::buttonGroupClicked);

    // Создаем сетку для элементов диаграммы
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(createCellWidget(tr("Ромб"), DiagramItem::Conditional), 0, 0);
    layout->addWidget(createCellWidget(tr("Квадрат"), DiagramItem::Step), 0, 1);
    layout->addWidget(createCellWidget(tr("Параллелограмм"), DiagramItem::Io), 1, 0);

    // Создаем кнопку для ввода текста
    QToolButton *textButton = new QToolButton;
    textButton->setCheckable(true);
    buttonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")));
    textButton->setIconSize(QSize(60, 60)); // Увеличиваем размер иконки
    QGridLayout *textLayout = new QGridLayout;
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel(tr("Текст")), 1, 0, Qt::AlignCenter);
    QWidget *textWidget = new QWidget;
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget, 1, 1);

    // Настраиваем растяжение сетки
    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);

    // Создаем виджет для элементов диаграммы
    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(layout);
    itemWidget->setStyleSheet("QWidget { background-color: #2F2F2F; }"); // Меняем цвет фона

    // Создаем группу кнопок для фонов
    backgroundButtonGroup = new QButtonGroup(this);
    connect(backgroundButtonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &MainWindow::backgroundButtonGroupClicked);

    // Создаем сетку для фонов
    QGridLayout *backgroundLayout = new QGridLayout;
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"),
                                                           ":/images/kletkaGreen.jpeg"), 0, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"),
                                                           ":/images/kletka.jpeg"), 0, 1);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"),
                                                           ":/images/dots.jpeg"), 1, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"),
                                                           ":/images/lines.jpeg"), 1, 1);

    // Настраиваем растяжение сетки фонов
    backgroundLayout->setRowStretch(2, 10);
    backgroundLayout->setColumnStretch(2, 10);

    // Создаем виджет для фонов
    QWidget *backgroundWidget = new QWidget;
    backgroundWidget->setLayout(backgroundLayout);
    backgroundWidget->setStyleSheet("QWidget { background-color: #2F2F2F; }"); // Меняем цвет фона

    // Создаем панель инструментов
    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("Basic Flowchart Shapes"));
    toolBox->addItem(backgroundWidget, tr("Backgrounds"));
    toolBox->setStyleSheet("QToolBox::tab { background-color: #003366 }"); // Меняем цвет вкладок
}


void MainWindow::createActions()
{
    // Кнопка "Вперёд"
    toFrontAction = new QAction(QIcon(":/images/arrow_up.png"), tr("Наверх"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Переместить элемент вперёд"));
    connect(toFrontAction, &QAction::triggered, this, &MainWindow::bringToFront);

    // Кнопка "Вперёд"
    /*toFrontAction = new QAction(QIcon(":/images/arrow_up.png"), tr("Вперёд"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Переместить элемент вперёд"));
    connect(toFrontAction, &QAction::triggered, this, &MainWindow::bringToFront);*/

    // Кнопка "Назад"
    sendBackAction = new QAction(QIcon(":/images/arrow_down.png"), tr("Вниз"), this);
    sendBackAction->setShortcut(tr("Ctrl+T"));
    sendBackAction->setStatusTip(tr("Переместить элемент назад"));
    connect(sendBackAction, &QAction::triggered, this, &MainWindow::sendToBack);

    // Кнопка "Удалить"
    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("&Удалить"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Удалить элемент из диаграммы"));
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteItem);


    // Создаем действие для сохранения сцены в файл
    saveAction = new QAction(QIcon(":/images/save.png"), tr("Сохранить сцену"), this);
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setStatusTip(tr("Сохранить текущую сцену в файл"));

    connect(saveAction, &QAction::triggered, [this]() {
        this->saveScene();
    });

    // Создаем действие "Выход"
    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, &QAction::triggered, this, &QApplication::quit);

    // Кнопка "Жирный"
    boldAction = new QAction(QIcon(":/images/bold.png"), tr("Жирный"), this);
    boldAction->setCheckable(true);
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, &QAction::triggered, this, &MainWindow::handleFontChange);

    // Кнопка "Курсив"
    italicAction = new QAction(QIcon(":/images/italic.png"), tr("Курсив"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, &QAction::triggered, this, &MainWindow::handleFontChange);

    // Кнопка "Подчёркнутый"
    underlineAction = new QAction(QIcon(":/images/underline.png"), tr("Подчёркнутый"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, &QAction::triggered, this, &MainWindow::handleFontChange);

    // Кнопка "О программе"
    aboutAction = new QAction(QIcon(":/images/about.png"), tr("О программе"), this);
    aboutAction->setShortcut(tr("F1"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
}


void MainWindow::saveScene()
{
    // Показываем диалог для выбора файла
    QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить сцену"), "",
                                                    tr("Файлы изображений (*.png)"));
    if (!fileName.isEmpty())
    {
        // Проверяем, что файл имеет расширение .png
        if (!fileName.endsWith(".png", Qt::CaseInsensitive))
        {
            fileName += ".png";
        }

        // Создаем изображение сцены
        QImage sceneImage(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
        sceneImage.fill(Qt::transparent);

        // Рисуем сцену на изображение
        QPainter painter(&sceneImage);
        scene->render(&painter);

        // Сохраняем изображение в файл
        if (sceneImage.save(fileName, "PNG"))
        {
            // Отображаем сообщение об успешном сохранении
            QMessageBox::information(this, tr("Сохранение сцены"),
                                     tr("Сцена успешно сохранена в файл: %1").arg(fileName));
        }
        else
        {
            // Отображаем сообщение об ошибке сохранения
            QMessageBox::critical(this, tr("Ошибка сохранения сцены"),
                                  tr("Не удалось сохранить сцену в файл: %1").arg(fileName));
        }
    }
}


void MainWindow::createMenus()
{
    // Устанавливаем стиль для меню
    QString menuStyle = "QMenu { "
                        "    background-color: #333333; "
                        "    color: #FFFFFF; "
                        "    font-size: 16px; "
                        "    font-family: 'Arial'; "
                        "    border: 2px solid #003366; "
                        "    border-radius: 8px; "
                        "}"
                        "QMenu::item:selected { "
                        "    background-color: #003366; "
                        "}";

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->setStyleSheet(menuStyle);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();

    // Создаем отдельную кнопку "Выход" (Exit)
    QAction* exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, &QAction::triggered, this, &QApplication::quit);
    fileMenu->addAction(exitAction);

    itemMenu = menuBar()->addMenu(tr("&Item"));
    itemMenu->setStyleSheet(menuStyle);
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);

    aboutMenu = menuBar()->addMenu(tr("&Help"));
    aboutMenu->setStyleSheet(menuStyle);
    aboutMenu->addAction(aboutAction);
}

void MainWindow::createToolbars() //визуал
{
    // Edit Toolbar
    editToolBar = addToolBar(tr("Actions"));
    editToolBar->setStyleSheet("QToolBar { background-color: #f0f0f0; }");
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);

    // Font Toolbar
    textToolBar = addToolBar(tr("Font"));
    textToolBar->setStyleSheet("QToolBar { background-color: #003366; }");
    fontCombo = new QFontComboBox();
    connect(fontCombo, &QFontComboBox::currentFontChanged, this, &MainWindow::currentFontChanged);
    textToolBar->addWidget(fontCombo);

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator *validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, &QComboBox::currentTextChanged, this, &MainWindow::fontSizeChanged);
    textToolBar->addWidget(fontSizeCombo);

    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);

    // Color Toolbar
    colorToolBar = addToolBar(tr("Color"));
    colorToolBar->setStyleSheet("QToolBar { background-color: #003366; }");

    // Кнопка выбора цвета текста
    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setText("Text Color");
    fontColorToolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    fontColorToolButton->setAutoRaise(true);
    connect(fontColorToolButton, &QToolButton::clicked, this, &MainWindow::textButtonTriggered);
    colorToolBar->addWidget(fontColorToolButton);

    // Кнопка выбора цвета заливки
    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::darkGray));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setText("Fill Color");
    fillColorToolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    fillColorToolButton->setAutoRaise(true);
    connect(fillColorToolButton, &QToolButton::clicked, this, &MainWindow::fillButtonTriggered);
    colorToolBar->addWidget(fillColorToolButton);

    // Кнопка выбора цвета линии
    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::gray));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setText("Line Color");
    lineColorToolButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    lineColorToolButton->setAutoRaise(true);
    connect(lineColorToolButton, &QToolButton::clicked, this, &MainWindow::lineButtonTriggered);
    colorToolBar->addWidget(lineColorToolButton);

    // Pointer Toolbar
    pointerToolbar = addToolBar(tr("Pointer"));
    pointerToolbar->setStyleSheet("QToolBar { background-color: #f0f0f0; }");

    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    pointerButton->setText("Выбрать");

    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));
    linePointerButton->setText("Нарисовать линию");

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &MainWindow::pointerGroupClicked);

    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);

    // Создаем панель инструментов
    QToolBar* toolBar = addToolBar(tr("Toolbar"));
    toolBar->addAction(exitAction);
    // Добавляем кнопку "Выход" в правый край панели инструментов
    toolBar->addSeparator(); // Добавляем разделитель, чтобы отделить кнопку "Выход"
    toolBar->addWidget(new QWidget(this)); // Добавляем "растягивающийся" виджет, чтобы кнопка отодвинулась вправо
    toolBar->addAction(exitAction);

}




QWidget *MainWindow::createBackgroundCellWidget(const QString &text, const QString &image)
{
    // Создаем новую кнопку QToolButton
    QToolButton *button = new QToolButton;

    // Устанавливаем текст на кнопке
    button->setText(text);

    // Устанавливаем значок на кнопке, используя переданное изображение
    button->setIcon(QIcon(image));

    // Устанавливаем размер значка на 50x50 пикселей
    button->setIconSize(QSize(50, 50));

    // Делаем кнопку "переключаемой" (может быть установлена или снята)
    button->setCheckable(true);

    // Добавляем кнопку в группу кнопок backgroundButtonGroup
    backgroundButtonGroup->addButton(button);

    // Создаем новую сетчатую компоновку QGridLayout
    QGridLayout *layout = new QGridLayout;

    // Добавляем кнопку в сетчатую компоновку, центрируя ее по горизонтали
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);

    // Добавляем новую метку QLabel с текстом в сетчатую компоновку, центрируя ее
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    // Создаем новый виджет
    QWidget *widget = new QWidget;

    // Устанавливаем сетчатую компоновку для виджета
    widget->setLayout(layout);

    // Возвращаем созданный виджет
    return widget;
}

QWidget *MainWindow::createCellWidget(const QString &text, DiagramItem::DiagramType type)
{
    // Создаем новый элемент диаграммы DiagramItem с указанным типом и контекстным меню itemMenu
    DiagramItem item(type, itemMenu);

    // Получаем значок, соответствующий типу диаграммы
    QIcon icon(item.image());

    // Создаем новую кнопку QToolButton
    QToolButton *button = new QToolButton;

    // Устанавливаем значок на кнопку
    button->setIcon(icon);

    // Устанавливаем размер значка на 50x50 пикселей
    button->setIconSize(QSize(50, 50));

    // Делаем кнопку "переключаемой" (может быть установлена или снята)
    button->setCheckable(true);

    // Добавляем кнопку в группу кнопок buttonGroup, связывая ее с типом диаграммы
    buttonGroup->addButton(button, int(type));

    // Создаем новую сетчатую компоновку QGridLayout
    QGridLayout *layout = new QGridLayout;

    // Добавляем кнопку в сетчатую компоновку, центрируя ее по горизонтали
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);

    // Добавляем новую метку QLabel с текстом в сетчатую компоновку, центрируя ее
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    // Создаем новый виджет
    QWidget *widget = new QWidget;

    // Устанавливаем сетчатую компоновку для виджета
    widget->setLayout(layout);

    // Возвращаем созданный виджет
    return widget;
}

QMenu *MainWindow::createColorMenu(const char *slot, QColor defaultColor) //создает и возвращает QMenu объект, содержащий набор цветов, которые можно использовать
    //для настройки элементов в интерфейсе.
{
    //список доступных цветов и соответствующих им локализованных названий
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")
          << tr("yellow");

    QMenu *colorMenu = new QMenu(this);
    colorMenu->setStyleSheet("background-color: rgba(0, 51, 102, 0.6);");
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this); //Устанавливается текстовое название цвета
        action->setData(colors.at(i));//Устанавливаются данные действия (цвет)
        action->setIcon(createColorIcon(colors.at(i))); //Создается и устанавливается иконка для действия (с помощью вызова createColorIcon())
        connect(action, SIGNAL(triggered()), this, slot); //Подключается сигнал triggered()
        colorMenu->addAction(action); //обавляется действие в меню
        if (colors.at(i) == defaultColor)
            colorMenu->setDefaultAction(action); //Если текущий цвет совпадает с defaultColor, то действие устанавливается как действие по умолчанию в меню
    }
    return colorMenu;
}

QIcon MainWindow::createColorToolButtonIcon(const QString &imageFile, QColor color) //добавить иконку цвета
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    return QIcon(pixmap);
}

QIcon MainWindow::createColorIcon(QColor color) //рисуем маленькие окошки с цветом в цветах
{
    QPixmap pixmap(30, 30);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 30, 30), color);

    return QIcon(pixmap);
}
