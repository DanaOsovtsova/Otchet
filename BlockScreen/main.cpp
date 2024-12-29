#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w; // Вызываем конструктор с родительским виджетом
    w.show();
    return a.exec();
}
