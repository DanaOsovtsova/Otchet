#include "mainapp.h"
#include "ui_mainapp.h"

#include <iostream>
#include <cmath>
#include "sin.h"
#include "prime.h"
#include "equation.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <QStandardItemModel>
#include <QMessageBox> //для вывода сообщений
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QSizePolicy>
#include <QTableView>
#include <QDataStream>
#include <QStringList>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>


using namespace std;



Mainapp::Mainapp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Mainapp)
{
    ui->setupUi(this);

}

Mainapp::~Mainapp()
{
    delete ui;
}


void Mainapp::on_comboBox_activated(int index)
{
    switch (index) {
    case 0: {

        QDialog* dialog = new QDialog(this);
        dialog->setWindowTitle("PRIME");
        dialog->setMinimumSize(100, 80);

        QVBoxLayout* layout = new QVBoxLayout(dialog);

        // Add a QLineEdit for user input
        QLabel* inputLabel1 = new QLabel("Введите число:", dialog);
        layout->addWidget(inputLabel1);
        QLineEdit* userInput1 = new QLineEdit(dialog);
        layout->addWidget(userInput1);

        // Add a QLineEdit for result output
        QLabel* resultLabel = new QLabel("Результат:", dialog);
        layout->addWidget(resultLabel);
        QLineEdit* resultOutput = new QLineEdit(dialog);
        resultOutput->setReadOnly(true);
        layout->addWidget(resultOutput);

        // Add a button to perform calculations
        QPushButton* calculateButton = new QPushButton("Calculate", dialog);
        layout->addWidget(calculateButton);

        // Connect the calculateButton to a slot that performs the calculation
        connect(calculateButton, &QPushButton::clicked, this, [userInput1, resultOutput](){
            QString inputText1 = userInput1->text();
            unsigned int N = (unsigned)inputText1.toInt();

            PRIME Num(N);
            QVector<unsigned int> divisors = Num.primeDivisors();

            QString resultText;
            for (int i = 0; i < divisors.size(); ++i) {
                resultText += QString::number(divisors[i]);
                if (i < divisors.size() - 1) {
                    resultText += " ";
                }
            }

            resultOutput->setText(resultText);
        });

        QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        layout->addWidget(buttonBox);

        connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

        if (dialog->exec() == QDialog::Accepted) {
            // Handle the acceptance of the dialog
            // ...
        }
    }

        case 1: {
                    QDialog* dialog = new QDialog(this);
                    dialog->setWindowTitle("SIN");
                    dialog->setMinimumSize(80, 60);

                    QVBoxLayout* layout = new QVBoxLayout(dialog);


                    QLabel* inputLabel1 = new QLabel("Введите угол в градусах и точность:", dialog);
                    layout->addWidget(inputLabel1);
                    QLineEdit* userInput1 = new QLineEdit(dialog);
                    layout->addWidget(userInput1);
                    QLineEdit* userInput2 = new QLineEdit(dialog);
                    layout->addWidget(userInput2);

                    // Add a QLabel for result output
                    QLabel* resultLabel = new QLabel("По Тейлору:", dialog);
                    layout->addWidget(resultLabel);
                    QLineEdit* resultOutput = new QLineEdit(dialog);
                    resultOutput->setReadOnly(true);
                    layout->addWidget(resultOutput);

                    QLabel* standardResultLabel = new QLabel("Стандартная функция:", dialog);
                    layout->addWidget(standardResultLabel);
                    QLineEdit* resultOutput1 = new QLineEdit(dialog);
                    resultOutput1->setReadOnly(true);
                    layout->addWidget(resultOutput1);

                    QPushButton* calculateButton = new QPushButton("Calculate", dialog);
                    layout->addWidget(calculateButton);

                    // Connect the calculateButton to a slot that performs the calculation
                    // ...

                    connect(calculateButton, &QPushButton::clicked, this, [userInput1, userInput2, resultOutput, resultOutput1] {
                        QString inputText1 = userInput1->text();
                        QString inputText2 = userInput2->text();
                        int inputArg = inputText1.toInt();
                        double inputEps = inputText2.toDouble();

                        SIN Sinx(inputArg, inputEps);
                        QString res = QString::number(Sinx.mySin());
                        resultOutput->setText(res);

                        // Calculate standard sine and update resultOutput1
                        double standardSin = std::sin(inputArg * M_PI / 180.0);
                        resultOutput1->setText(QString::number(standardSin));
                    });

                    // ...



                    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
                    layout->addWidget(buttonBox);

                    connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
                    connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

                    if (dialog->exec() == QDialog::Accepted) {
                        // Handle the acceptance of the dialog
                        // ...
                    }
                }



    case 2: {

        QDialog* dialog = new QDialog(this);
        dialog->setWindowTitle("EQUATION");
        dialog->setMinimumSize(100, 80);

        QVBoxLayout* layout = new QVBoxLayout(dialog);

        // Add a QLineEdit for result output
        QLabel* inputLabel1 = new QLabel("Введите коэффиценты:", dialog);
        layout->addWidget(inputLabel1);
        QLineEdit* userInput1 = new QLineEdit(dialog);
        layout->addWidget(userInput1);
        QLineEdit* userInput2 = new QLineEdit(dialog);
        layout->addWidget(userInput2);
        QLineEdit* userInput3 = new QLineEdit(dialog);
        layout->addWidget(userInput3);
        QLabel* resultLabel = new QLabel("Корни:", dialog);
        layout->addWidget(resultLabel);
        QLineEdit* resultOutput = new QLineEdit(dialog);
        resultOutput->setReadOnly(true);
        layout->addWidget(resultOutput);
        QLineEdit* resultOutput1 = new QLineEdit(dialog);
        resultOutput->setReadOnly(true);
        layout->addWidget(resultOutput1);

        // Add some buttons or other widgets for user interaction
        QPushButton* calculateButton = new QPushButton("Calculate", dialog);
        layout->addWidget(calculateButton);

        // Connect the calculateButton to a slot that performs the calculation
        connect(calculateButton, &QPushButton::clicked, this, [userInput1,userInput2,userInput3,resultOutput,resultOutput1]() {
            QString inputText1 = userInput1->text();
            QString inputText2 = userInput2->text();
            QString inputText3 = userInput3->text();
            // Perform calculations using inputText and update the resultOutput
            // For example:
            double Coeff1 = inputText1.toDouble();
            double Coeff2 = inputText2.toDouble();
            double Coeff3 = inputText3.toDouble();

            EQUATION Equ(Coeff1,Coeff2,Coeff3);
            QString res = QString::number(Equ.solveQuadratic()[0]);
            QString res1 = QString::number(Equ.solveQuadratic()[1]);
            resultOutput->setText(res);
            resultOutput1->setText(res1);
        });


        QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        layout->addWidget(buttonBox);

        connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

        if (dialog->exec() == QDialog::Accepted) {
            // Handle the acceptance of the dialog
            // ...
        }
        break;
    }}}
