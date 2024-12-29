#include "QtCore/qobject.h"
#include "QtCore/qtmetamacros.h"
#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui {
class Mainapp;
}
QT_END_NAMESPACE

class Mainapp : public QMainWindow
{
    Q_OBJECT

public:
    Mainapp(QWidget *parent = nullptr);
    ~Mainapp();


private slots:
    void on_comboBox_activated(int index);
private:
    Ui::Mainapp *ui;
};
