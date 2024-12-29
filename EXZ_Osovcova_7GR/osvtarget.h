#ifndef OSVTARGET_H
#define OSVTARGET_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class osvTarget;
}
QT_END_NAMESPACE

class osvTarget : public QMainWindow
{
    Q_OBJECT

public:
    osvTarget(QWidget *parent = nullptr);
    ~osvTarget();

private slots:
    void on_startButton_clicked();

    void on_searchButton_clicked();

    void on_search2Button_clicked();

    void on_actionSave_triggered();

    void on_actionExit_triggered();

   void on_pushButton_triggered();

private:
    Ui::osvTarget *ui;
};

#endif // OSVTARGET_H
