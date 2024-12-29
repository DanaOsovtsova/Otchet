#include "tabels.h"
#include "ui_tabels.h"
#include <QApplication>
#include <QTableView>
#include <QStandardItemModel>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QTableView>
#include <QAbstractTableModel>
#include <QTextStream>
#include <QFile>
#include <fstream>
#include <iostream>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <sstream>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

using namespace std;

class Model{
public:
    virtual void GetData(istream& in)=0;
    virtual void PutData(ostream& out)=0;
    virtual void Sort()=0;
    virtual int GetCount()=0;
};



struct Product{//лекарства MB1(name, data, section, count

    string Name;
    string Date;
    string Section;
    int Count;


    bool operator==(Product& a){ //перегрузка ==
        if(*this == a){
            return true;
        }
        else return false;

    }

    friend ostream& operator<<(ostream& out, const Product& a){ //перегрузка <<
        out << a.Name << " " <<a.Date << " " << a.Section <<" "<<a.Count;
        return out;
    }

    friend istream& operator>>(istream& in, Product& a){ //перегрузка >>
        in >> a.Name >> a.Date >> a.Section >> a.Count;
        return in;
    }
};

struct Customer {//Цены MB2 (name, date, price)

    string Name;
    string email;

    bool operator==(Customer&a){ //перегрузка ==
        if (*this ==a){
            return true;
        }
        else return false;
    }

    friend ostream& operator<<(ostream& out, const Customer& a){ //перегрузка <<
        out<<a.Name<<" "<<a.email;
        return out;
    }

    friend istream& operator>>(istream& in, Customer& a){
        in>> a.Name>>a.email;
        return in;
    }
};





class Products : public Model{ //Наследованный класс Лекарства

    int count;//колво эл s1
    Product*arr;

public:
    int counter(){
        return count;
    }
    //стандартные конструкторы и деструктор класса

    Products();//конструктор по умолчанию

    Products(int c);//конструктор c параметрами

    Products(const Products& other);//конструктор копирования

    ~Products();//деструктор

    Products& operator =(const Products& other){//перегоузка оператора присваивания
        this->count = other.count;
        if(this->arr!= nullptr){
            delete[]this->arr;
        }
        this->arr=new Product[other.count];
        for(int i = 0; i < other.count; i++){
            this->arr[i]=other.arr[i];
        }
        return *this;
    }

    Product operator[](int a){//перегрузка оператора индексации
        return arr[a];
    }

    int GetCount() override{//реализация виртуальной функции(возвращает значение count)
        return count;
    }

    //объявления виртуальных функций

    void GetData(istream&) override;
    void Sort() override;
    void PutData(ostream&) override;
    //void ToBin(ofstream&out, string output_bin_file);

};

int count_lines(istream&in){//для чтения строк из потока
    string s;
    int ct=0;//счетчик ct хранит количество прочитанных строк.
    while(getline(in, s)){
        ct++;
    }
    in.seekg(0, ios::beg);//устанавливает позицию указателя потока на начало с помощью seekg()
    return ct;
}

void Products::GetData(istream&in){

    ifstream in1("products.txt");
    count = count_lines(in1);
    arr = new Product[count];

    for(int i =0; i < count; i++){
        in >> arr[i];
    }
}

//Функция PutData() реализует другую виртуальную функцию из ABS_COMP. Она выводит все объекты S1,
//хранящиеся в массиве arr, в выходной поток out с переводом строки.

void Products::PutData(ostream&out){

    ofstream outFile("sortedProducts.txt");
    for(int i = 0; i < count; i++){
        out << arr[i] << endl;
    }
    outFile.close();
}

Products::Products(){//конструктор по умолчанию для класса Fruit
    count = 0; arr = nullptr;
}

Products::Products(int c){//конструктор с параметрами
    count = c; arr = new Product[count];
}

Products::Products(const Products& other){//конструктор копирования
    this->count=other.count;
    this->arr=new Product[count];
    for(int i = 0; i < other.count; i++){
        this->arr[i] = other.arr[i];
    }
}

Products::~Products(){//деструктор
    delete[]arr;
}

void Products::Sort(){//сортирует массив arr в порядке возрастания по полю Name объектов S1
    sort(arr, arr+count, [](const Product& s1, const Product& s2){
        return s1.Name < s2.Name;
    });
}




void Tabels::on_checkBoxProducts_stateChanged(int arg1)
{
    Products M1;
    ifstream in1("products.txt");
    Model *A1;
    A1=&M1;
    A1->GetData(in1);


    //НЕОТСОРТИРОВАННЫЙ Products
    QStandardItemModel *model = new QStandardItemModel(this);

    ui->output1->setModel(model);

    //форматируем таблицу
    model->setRowCount(A1->GetCount()+1);//Устанавливает количество строк в этой модели в columns. Если это меньше, чем columnCount(),
    //данные в нежелательных строках отбрасываются.
    model->setColumnCount(4);//Устанавливает количество столбцов в этой модели в columns. Если это меньше, чем columnCount(),
    //данные в нежелательных столбцах отбрасываются.
    //по умолчанию пронумерованны

    //заполняем таблицу
    model->setData(model->index(0,0), "Name");
    model->setData(model->index(0,1), "Date");
    model->setData(model->index(0,2), "Section");
    model->setData(model->index(0,3), "Count");


    for(int i=1; i < A1->GetCount()+1; i++){
        model->setData(model->index(i,0), QString::fromStdString(M1[i-1].Name));
        model->setData(model->index(i,1), QString::fromStdString(M1[i-1].Date));
        model->setData(model->index(i, 2), QString::fromStdString(M1[i-1].Section));
        model->setData(model->index(i,3),M1[i-1].Count);
    }

    //для аккуратности
    ui->output1->resizeColumnsToContents();//Изменяет размеры всех столбцов
    ui->output1->resizeRowsToContents();//Изменяет размеры всех строк
    ui->output1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->output1->setMinimumSize(ui->output1->sizeHint());//Размер виджета не может быть изменен до меньшего размера,
    //чем минимальный размер виджета.

}


void Tabels::on_sortButton_clicked()
{
    Products M1;
    ifstream in1("products.txt");
    Model *A1;
    A1=&M1;
    A1->GetData(in1);

    //ОТСОРТИРОВАННЫЙ MB1
    QStandardItemModel *model2 = new QStandardItemModel(this);

    ui->output1->setModel(model2);
    A1->Sort();

    //форматируем таблицу Отсортированный MB1
    model2->setRowCount(A1->GetCount()+1);
    model2->setColumnCount(4);

    //заполняем таблицу Отсортированный MB1
    model2->setData(model2->index(0,0), "Название");
    model2->setData(model2->index(0,1), "Срок годности");
    model2->setData(model2->index(0,2), "Секция");
    model2->setData(model2->index(0,3), "Количество");

    for(int i=1; i < A1->GetCount()+1; i++){ //заполняем таблицу
        model2->setData(model2->index(i,0), QString::fromStdString(M1[i-1].Name));
        model2->setData(model2->index(i, 1), QString::fromStdString(M1[i-1].Date));
        model2->setData(model2->index(i, 2), QString::fromStdString(M1[i-1].Section));
        model2->setData(model2->index(i,3),M1[i-1].Count);
    }

    //для аккуратности
    ui->output1->resizeColumnsToContents();//Изменяет размеры всех столбцов
    ui->output1->resizeRowsToContents();//Изменяет размеры всех строк
    ui->output1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->output1->setMinimumSize(ui->output1->sizeHint());

}


void Tabels::on_SaveTableChangesButton_clicked()
{
    // Открываем исходный файл на запись
    std::ofstream outFile("products.txt", std::ios::out | std::ios::trunc);
    if (!outFile.is_open())
    {
        QMessageBox::critical(this, "Error", "Unable to open the file for writing.");
        return;
    }

    // Получаем ссылку на таблицу
    QTableView* tableView = ui->output1;
    QAbstractItemModel* model = tableView->model();

    // Получаем индекс текущей выделенной ячейки
    QModelIndex currentIndex = tableView->currentIndex();
    int currentRow = currentIndex.row();
    int currentColumn = currentIndex.column();

    // Записываем изменения в исходный файл
    for (int row = 1; row < model->rowCount(); ++row)
    {
        // Получаем значения из полей таблицы
        QModelIndex nameIndex = model->index(row, 0);
        QModelIndex dateIndex = model->index(row, 1);
        QModelIndex sectionIndex = model->index(row, 2);
        QModelIndex countIndex = model->index(row, 3);

        QString name = model->data(nameIndex).toString();
        QString date = model->data(dateIndex).toString();
        QString section = model->data(sectionIndex).toString();
        QString count = model->data(countIndex).toString();

        // Записываем данные в файл
        outFile << name.toStdString() << " " << date.toStdString() << " " << section.toStdString() << " " << count.toStdString() << std::endl;
    }

    outFile.close();

    // Перемещаем курсор на последнюю выделенную ячейку
    tableView->setCurrentIndex(currentIndex);

    QMessageBox::information(this, "Success", "Changes saved to the file.");
}


void Tabels::on_pushButton_clicked()
{
    // Получаем значение из lineEditNewForTable
    QString newData = ui->lineEditNewForTable->text();

    // Добавляем новую строку в tableView
    QStringList newRow = newData.split(" ");
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->output1->model());
    int rowCount = model->rowCount();
    model->insertRow(rowCount);
    for (int i = 0; i < newRow.size(); i++) {
        model->setData(model->index(rowCount, i), newRow[i]);
    }

    // Запись в файл
    QFile file("products.txt");
    QMessageBox::information(this, "Success", "Changes saved to the file.");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << newData << "\n";
        file.close();
    } else {
        QMessageBox::warning(this, "Error", "Unable to open file");
    }

    // Очищаем lineEditNewForTable
    ui->lineEditNewForTable->clear();
}


void Tabels::on_searchButton_clicked()
{
    // Получаем значение из lineEditSearch
    QString searchText = ui->lineEditSearch->text();

    // Получаем ссылку на таблицу
    QTableView* tableView = ui->output1;
    QAbstractItemModel* model = tableView->model();


    // Сбрасываем стиль таблицы
    tableView->setStyleSheet("");

    // Ищем строки в таблице
    QStringList foundRows;
    for (int row = 0; row < model->rowCount(); row++) {
        for (int col = 0; col < model->columnCount(); col++) {
            QModelIndex index = model->index(row, col);
            QString cellText = model->data(index).toString();

            if (cellText.contains(searchText, Qt::CaseInsensitive)) {
                // Выделяем ячейку оранжевым цветом
                tableView->setStyleSheet(QString("QTableView::item:row(%1),column(%2) { background-color: orange; }").arg(row).arg(col));
                tableView->scrollTo(index);
                foundRows.append(QString::number(row + 1));
            }
        }
    }

    // Если строки не найдены, выводим сообщение
    if (foundRows.isEmpty()) {
        QMessageBox::information(this, "Not Found", "The item you searched for was not found.");
    } else {
        QString message = "The item was found in the following rows:\n" + foundRows.join(", ");
        QMessageBox::information(this, "Found", message);
    }
}

void Tabels::on_saveToTXTButton_clicked()
{
    QTableView* tableView = ui->output1;
    QAbstractItemModel* model = tableView->model();
    if (!model) {
        QMessageBox::warning(this, "Error", "No data to save.");
        return;
    }


    QString filePath = QFileDialog::getSaveFileName(this, "Save to TXT", "", "Text Files (*.txt)");
    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int row = 0; row < model->rowCount(); row++) {
            QStringList rowData;
            for (int col = 0; col < model->columnCount(); col++) {
                QModelIndex index = model->index(row, col);
                rowData << model->data(index).toString();
            }
            out << rowData.join("\t") << "\n";
        }
        file.close();
        QMessageBox::information(this, "Success", "Table saved to TXT file.");
    } else {
        QMessageBox::warning(this, "Error", "Failed to save table to TXT file.");
    }
}


void Tabels::on_saveToBinButton_clicked()
{
    QTableView* tableView = ui->output1;
    QAbstractItemModel* model = tableView->model();
    if (!model) {
        QMessageBox::warning(this, "Error", "No data to save.");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, "Save to Binary", "", "Binary Files (*.bin)");
    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        for (int row = 0; row < model->rowCount(); row++) {
            for (int col = 0; col < model->columnCount(); col++) {
                QModelIndex index = model->index(row, col);
                out << model->data(index).toString();
            }
        }
        file.close();
        QMessageBox::information(this, "Success", "Table saved to binary file.");
    } else {
        QMessageBox::warning(this, "Error", "Failed to save table to binary file.");
    }
}


void Tabels::on_saveToJsonButton_clicked()
{
    QTableView* tableView = ui->output1;
    QAbstractItemModel* model = tableView->model();
    if (!model) {
        QMessageBox::warning(this, "Error", "No data to save.");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, "Save to JSON", "", "JSON Files (*.json)");
    if (filePath.isEmpty()) {
        return;
    }

    QJsonDocument doc;
    QJsonArray array;
    for (int row = 0; row < model->rowCount(); row++) {
        QJsonObject rowObj;
        for (int col = 0; col < model->columnCount(); col++) {
            QModelIndex index = model->index(row, col);
            rowObj.insert(QString::number(col), model->data(index).toString());
        }
        array.append(rowObj);
    }
    doc.setArray(array);

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        QMessageBox::information(this, "Success", "Table saved to JSON file.");
    } else {
        QMessageBox::warning(this, "Error", "Failed to save table to JSON file.");
    }
}





class Castomers: public Model{// Наследованный класс Цены

    int count;
    Customer *arr;

public:
    int counter(){
        return count;
    }
    //стандартные конструкторы и деструктор класса

    Castomers();//конструктор по умолчанию

    Castomers(int c);//конструктор с параметром

    Castomers(const Castomers& other);//конструктор копирования

    ~Castomers();//деструктор

    Castomers& operator =(const Castomers& other){//перегрузка оператора присваивания
        this->count = other.count;
        if(this->arr!= nullptr){
            delete[]this->arr;
        }
        this->arr=new Customer[other.count];
        for(int i = 0; i < other.count; i++){
            this->arr[i]=other.arr[i];
        }
        return *this;
    }

    Customer operator[](int a){//перегрузка оператора индексации
        return arr[a];
    }

    int GetCount() override{//реализация виртуальной функции(возвращает значение count)
        return count;
    }

    void GetData(istream&) override;
    void PutData(ostream&) override;
    void Sort() override;
};

void Castomers::GetData(istream&in){//читает данные из файла "input1.txt" и заполняет массив arr объектами типа S2
    ifstream in1("customers.txt");
    count = count_lines(in1);
    arr = new Customer[count];
    for(int i =0; i < count; i++){
        in >> arr[i];
    }
}

void Castomers::PutData(ostream&out){//выводит содержимое массива

    ofstream outFile("Castomers.txt");
    Sort();
    for(int i = 0; i < count; i++){
        out << arr[i] << endl;
    }

    outFile.close();
}


Castomers::Castomers(){//конструктор по умолчанию для класса

    count = 0; arr = nullptr;
}

Castomers::Castomers(int c){//конструктор с параметром для класса

    count = c; arr = new Customer[count];
}

Castomers::Castomers(const Castomers& other){//конструктор копирования для класса

    this->count=other.count;
    this->arr=new Customer[count];

    for(int i = 0; i < other.count; i++){
        this->arr[i] = other.arr[i];
    }
}

Castomers::~Castomers(){//деструктор
    delete[]arr;
}


void Castomers::Sort(){//cортирует элементы массива arr по возрастанию значения поля Name объектов типа S2

    sort(arr, arr+count, [](const Customer& s1, const Customer& s2){
        return s1.Name < s2.Name;
    });
}




void Tabels::on_checkBoxCastumers_stateChanged(int arg1)
{

    Castomers M2;
    ifstream in2("customers.txt");
    Model *A2;
    A2=&M2;
    A2->GetData(in2);


    //НЕОТСОРТИРОВАННЫЙ Products
    QStandardItemModel *model1 = new QStandardItemModel(this);

    ui->output1_2->setModel(model1);

    //форматируем таблицу
    model1->setRowCount(A2->GetCount()+1);//Устанавливает количество строк в этой модели в columns. Если это меньше, чем columnCount(),
    //данные в нежелательных строках отбрасываются.
    model1->setColumnCount(2);//Устанавливает количество столбцов в этой модели в columns. Если это меньше, чем columnCount(),
    //данные в нежелательных столбцах отбрасываются.
    //по умолчанию пронумерованны

    //заполняем таблицу
    model1->setData(model1->index(0,0), "Name");
    model1->setData(model1->index(0,1), "Email");



    for(int i=1; i < A2->GetCount()+1; i++){
        model1->setData(model1->index(i,0), QString::fromStdString(M2[i-1].Name));
        model1->setData(model1->index(i,1), QString::fromStdString(M2[i-1].email));
    }

    //для аккуратности
    ui->output1->resizeColumnsToContents();//Изменяет размеры всех столбцов
    ui->output1->resizeRowsToContents();//Изменяет размеры всех строк
    ui->output1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->output1->setMinimumSize(ui->output1->sizeHint());//Размер виджета не может быть изменен до меньшего размера,
    //чем минимальный размер виджета.
}

void Tabels::on_sortButton_2_clicked()
{
    Castomers M2;
    ifstream in2("customers.txt");
    Model *A2;
    A2=&M2;
    A2->GetData(in2);

    //ОТСОРТИРОВАННЫЙ MB1
    QStandardItemModel *model3 = new QStandardItemModel(this);

    ui->output1_2->setModel(model3);
    A2->Sort();

    //форматируем таблицу Отсортированный MB1
    model3->setRowCount(A2->GetCount()+1);
    model3->setColumnCount(2);

    //заполняем таблицу Отсортированный MB1
    model3->setData(model3->index(0,0), "Name");
    model3->setData(model3->index(0,1), "Email");


    for(int i=1; i < A2->GetCount()+1; i++){ //заполняем таблицу
        model3->setData(model3->index(i,0), QString::fromStdString(M2[i-1].Name));
        model3->setData(model3->index(i, 1), QString::fromStdString(M2[i-1].email));

    }

    //для аккуратности
    ui->output1->resizeColumnsToContents();//Изменяет размеры всех столбцов
    ui->output1->resizeRowsToContents();//Изменяет размеры всех строк
    ui->output1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->output1->setMinimumSize(ui->output1->sizeHint());
}


void Tabels::on_SaveTableChangesButton_2_clicked()
{
    // Открываем исходный файл на запись
    std::ofstream outFile("customers.txt", std::ios::out | std::ios::trunc);
    if (!outFile.is_open())
    {
        QMessageBox::critical(this, "Error", "Unable to open the file for writing.");
        return;
    }

    // Получаем ссылку на таблицу
    QTableView* tableView = ui->output1_2;
    QAbstractItemModel* model = tableView->model();

    // Получаем индекс текущей выделенной ячейки
    QModelIndex currentIndex = tableView->currentIndex();
    int currentRow = currentIndex.row();
    int currentColumn = currentIndex.column();

    // Записываем изменения в исходный файл
    for (int row = 1; row < model->rowCount(); ++row)
    {
        // Получаем значения из полей таблицы
        QModelIndex nameIndex = model->index(row, 0);
        QModelIndex emailIndex = model->index(row, 1);

        QString name = model->data(nameIndex).toString();
        QString email = model->data(emailIndex).toString();

        // Записываем данные в файл
        outFile << name.toStdString() << " " << email.toStdString() << std::endl;
    }

    outFile.close();

    // Перемещаем курсор на последнюю выделенную ячейку
    tableView->setCurrentIndex(currentIndex);

    QMessageBox::information(this, "Success", "Changes saved to the file.");

}


void Tabels::on_pushButton_2_clicked()
{
    // Получаем значение из lineEditNewForTable
    QString newData = ui->lineEditNewForTable_2->text();

    // Добавляем новую строку в tableView
    QStringList newRow = newData.split(" ");
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->output1_2->model());
    int rowCount = model->rowCount();
    model->insertRow(rowCount);
    for (int i = 0; i < newRow.size(); i++) {
        model->setData(model->index(rowCount, i), newRow[i]);
    }

    // Запись в файл
    QFile file("customers.txt");
    QMessageBox::information(this, "Success", "Changes saved to the file.");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << newData << "\n";
        file.close();
    } else {
        QMessageBox::warning(this, "Error", "Unable to open file");
    }

    // Очищаем lineEditNewForTable
    ui->lineEditNewForTable_2->clear();

}


void Tabels::on_saveToTXTButton_2_clicked()
{
    QTableView* tableView = ui->output1_2;
    QAbstractItemModel* model = tableView->model();
    if (!model) {
        QMessageBox::warning(this, "Error", "No data to save.");
        return;
    }


    QString filePath = QFileDialog::getSaveFileName(this, "Save to TXT", "", "Text Files (*.txt)");
    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int row = 0; row < model->rowCount(); row++) {
            QStringList rowData;
            for (int col = 0; col < model->columnCount(); col++) {
                QModelIndex index = model->index(row, col);
                rowData << model->data(index).toString();
            }
            out << rowData.join("\t") << "\n";
        }
        file.close();
        QMessageBox::information(this, "Success", "Table saved to TXT file.");
    } else {
        QMessageBox::warning(this, "Error", "Failed to save table to TXT file.");
    }
}


void Tabels::on_saveToBinButton_2_clicked()
{
    QTableView* tableView = ui->output1_2;
    QAbstractItemModel* model = tableView->model();
    if (!model) {
        QMessageBox::warning(this, "Error", "No data to save.");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, "Save to Binary", "", "Binary Files (*.bin)");
    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        for (int row = 0; row < model->rowCount(); row++) {
            for (int col = 0; col < model->columnCount(); col++) {
                QModelIndex index = model->index(row, col);
                out << model->data(index).toString();
            }
        }
        file.close();
        QMessageBox::information(this, "Success", "Table saved to binary file.");
    } else {
        QMessageBox::warning(this, "Error", "Failed to save table to binary file.");
    }

}


void Tabels::on_saveToJsonButton_2_clicked()
{
    QTableView* tableView = ui->output1_2;
    QAbstractItemModel* model = tableView->model();
    if (!model) {
        QMessageBox::warning(this, "Error", "No data to save.");
        return;
    }

    QString filePath = QFileDialog::getSaveFileName(this, "Save to JSON", "", "JSON Files (*.json)");
    if (filePath.isEmpty()) {
        return;
    }

    QJsonDocument doc;
    QJsonArray array;
    for (int row = 0; row < model->rowCount(); row++) {
        QJsonObject rowObj;
        for (int col = 0; col < model->columnCount(); col++) {
            QModelIndex index = model->index(row, col);
            rowObj.insert(QString::number(col), model->data(index).toString());
        }
        array.append(rowObj);
    }
    doc.setArray(array);

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        QMessageBox::information(this, "Success", "Table saved to JSON file.");
    } else {
        QMessageBox::warning(this, "Error", "Failed to save table to JSON file.");
    }
}



Tabels::Tabels(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Tabels)
{
    ui->setupUi(this);
}


Tabels::~Tabels()
{
    delete ui;
}






