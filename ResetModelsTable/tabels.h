#ifndef TABELS_H
#define TABELS_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui {
class Tabels;
}
QT_END_NAMESPACE

class Tabels : public QMainWindow
{
    Q_OBJECT

public:
    Tabels(QWidget *parent = nullptr);
    ~Tabels();

private slots:


    void on_checkBoxProducts_stateChanged(int arg1);

    void on_sortButton_clicked();

    void on_checkBoxCastumers_stateChanged(int arg1);

    void on_sortButton_2_clicked();

    void on_SaveTableChangesButton_clicked();

    void on_pushButton_clicked();

    void on_searchButton_clicked();

    void on_saveToTXTButton_clicked();

    void on_saveToBinButton_clicked();

    void on_saveToJsonButton_clicked();

    void on_SaveTableChangesButton_2_clicked();

    void on_pushButton_2_clicked();

    void on_saveToTXTButton_2_clicked();

    void on_saveToBinButton_2_clicked();

    void on_saveToJsonButton_2_clicked();

private:
    Ui::Tabels *ui;
};


#endif // TABELS_H
