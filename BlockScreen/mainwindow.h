#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DiagramItem.h"
#include "DiagramScene.h"
#include "QtWidgets/qbuttongroup.h"

#include <QMainWindow>

class DiagramScene;

QT_BEGIN_NAMESPACE //Начало области видимости пространства имен Qt.
class QAction;
class QToolBox;
class QSpinBox;
class QComboBox;
class QFontComboBox;
class QButtonGroup;
class QLineEdit;
class QGraphicsTextItem;
class QFont;
class QToolButton;
class QAbstractButton;
class QGraphicsView;
QT_END_NAMESPACE //Конец области видимости пространства имен Qt.

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    //MainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {}


    void saveScene(bool);


private slots:
    void backgroundButtonGroupClicked(QAbstractButton *button);
    void buttonGroupClicked(QAbstractButton *button);
    void deleteItem();
    void pointerGroupClicked();
    void bringToFront();
    void sendToBack();
    void itemInserted(DiagramItem *item);
    void textInserted(QGraphicsTextItem *item);
    void currentFontChanged(const QFont &font);
    void fontSizeChanged(const QString &size);
    void textColorChanged();
    void itemColorChanged();
    void lineColorChanged();
    void textButtonTriggered();
    void fillButtonTriggered();
    void lineButtonTriggered();
    void handleFontChange();
    void itemSelected(QGraphicsItem *item);
    void about();

private:

    QGraphicsView* graphicsView;
   // QAction* saveAction;
    void createToolBox();
    void createActions();
    void createMenus();
    void createToolbars();
    void saveScene();
    QWidget *createBackgroundCellWidget(const QString &text,
                                        const QString &image);
    QWidget *createCellWidget(const QString &text,
                              DiagramItem::DiagramType type);
    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(const QString &image, QColor color);
    QIcon createColorIcon(QColor color);

    DiagramScene *scene;
    QGraphicsView *view;

    QAction *exitAction;
    QAction *addAction;
    QAction *deleteAction;
    QAction *saveAction;

    QAction *toFrontAction;
    QAction *sendBackAction;
    QAction *aboutAction;

    QMenu *fileMenu;
    QMenu *itemMenu;
    QMenu *aboutMenu;

    QToolBar *textToolBar;
    QToolBar *editToolBar;
    QToolBar *colorToolBar;
    QToolBar *pointerToolbar;


    QComboBox *sceneScaleCombo;
    QComboBox *itemColorCombo;
    QComboBox *textColorCombo;
    QComboBox *fontSizeCombo;
    QFontComboBox *fontCombo;

    QToolBox *toolBox;
    QButtonGroup *buttonGroup;
    QButtonGroup *pointerTypeGroup;
    QButtonGroup *backgroundButtonGroup;
    QToolButton *fontColorToolButton;
    QToolButton *fillColorToolButton;
    QToolButton *lineColorToolButton;
    QAction *boldAction;
    QAction *underlineAction;
    QAction *italicAction;
    QAction *textAction;
    QAction *fillAction;
    QAction *lineAction;
};



#endif // MAINWINDOW_H
