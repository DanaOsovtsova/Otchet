#include "osvtarget.h"
#include "ui_osvtarget.h"
#include <iostream>
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


using namespace std;

class ABS_Med{
public:
    virtual void GetMed(istream& in)=0;
    virtual void PutMed(ostream& out)=0;
    virtual void Sort()=0;
    virtual int GetCount()=0;
};

struct S1{//лекарства MB1(name, data, section, count

    string Name;
    string Date;
    string Section;
    int Count;


    bool operator==(S1& a){ //перегрузка ==
        if(*this == a){
            return true;
        }
        else return false;

    }

    friend ostream& operator<<(ostream& out, const S1& a){ //перегрузка <<
        out << a.Name << " " <<a.Date << " " << a.Section <<" "<<a.Count;
        return out;
    }

    friend istream& operator>>(istream& in, S1& a){ //перегрузка >>
        in >> a.Name >> a.Date >> a.Section >> a.Count;
        return in;
    }
};

struct S2{//Цены MB2 (name, date, price)

    string Name;
    string Date;
    string Price;

    bool operator==(S2&a){ //перегрузка ==
        if (*this ==a){
            return true;
        }
        else return false;
    }

    friend ostream& operator<<(ostream& out, const S2& a){ //перегрузка <<
        out<<a.Name<<" "<<a.Date<<" "<<a.Price;
        return out;
    }

    friend istream& operator>>(istream& in, S2& a){
        in>> a.Name>>a.Date>>a.Price;
        return in;
    }
};


class MB1 : public ABS_Med{ //Наследованный класс Лекарства

    int count;//колво эл s1
    S1 *arr;

public:
    int counter(){
        return count;
    }
    //стандартные конструкторы и деструктор класса

    MB1();//конструктор по умолчанию

    MB1(int c);//конструктор c параметрами

    MB1(const MB1& other);//конструктор копирования

    ~MB1();//деструктор

    MB1& operator =(const MB1& other){//перегоузка оператора присваивания
        this->count = other.count;
        if(this->arr!= nullptr){
            delete[]this->arr;
        }
        this->arr=new S1[other.count];
        for(int i = 0; i < other.count; i++){
            this->arr[i]=other.arr[i];
        }
        return *this;
    }

    S1 operator[](int a){//перегрузка оператора индексации
        return arr[a];
    }

    int GetCount() override{//реализация виртуальной функции(возвращает значение count)
        return count;
    }

    //объявления виртуальных функций

    void GetMed(istream&) override;
    void PutMed(ostream&) override;
    void Sort() override;
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

//Функция GetData() реализует виртуальную функцию из базового класса
//Она открывает файл "input.txt" для чтения, вызывает count_lines() для определения количества строк в файле,
//создает динамический массив arr нужного размера и затем последовательно считывает объекты S1 из входного потока in в этот массив.

void MB1::GetMed(istream&in){

    ifstream in1("input.txt");
    count = count_lines(in1);
    arr = new S1[count];

    for(int i =0; i < count; i++){
        in >> arr[i];
    }
}

//Функция PutData() реализует другую виртуальную функцию из ABS_COMP. Она выводит все объекты S1,
//хранящиеся в массиве arr, в выходной поток out с переводом строки.

void MB1::PutMed(ostream&out){

    ofstream outFile("sortedMB1.txt");
    for(int i = 0; i < count; i++){
        out << arr[i] << endl;
    }
    outFile.close();
}


MB1::MB1(){//конструктор по умолчанию для класса Fruit
    count = 0; arr = nullptr;
}

MB1::MB1(int c){//конструктор с параметрами
    count = c; arr = new S1[count];
}

MB1::MB1(const MB1& other){//конструктор копирования, который создает новый объект Fruit на основе существующего other.
    this->count=other.count;
    this->arr=new S1[count];
    for(int i = 0; i < other.count; i++){
        this->arr[i] = other.arr[i];
    }
}

MB1::~MB1(){//деструктор
    delete[]arr;
}

void MB1::Sort(){//сортирует массив arr в порядке возрастания по полю Name объектов S1
    sort(arr, arr+count, [](const S1& s1, const S1& s2){
        return s1.Name < s2.Name;
    });
}



class MB2: public ABS_Med{// Наследованный класс Цены

    int count;
    S2 *arr;

public:
    int counter(){
        return count;
    }
    //стандартные конструкторы и деструктор класса

    MB2();//конструктор по умолчанию

    MB2(int c);//конструктор с параметром

    MB2(const MB2& other);//конструктор копирования

    ~MB2();//деструктор

    MB2& operator =(const MB2& other){//перегрузка оператора присваивания
        this->count = other.count;
        if(this->arr!= nullptr){
            delete[]this->arr;
        }
        this->arr=new S2[other.count];
        for(int i = 0; i < other.count; i++){
            this->arr[i]=other.arr[i];
        }
        return *this;
    }

    S2 operator[](int a){//перегрузка оператора индексации
        return arr[a];
    }

    int GetCount() override{//реализация виртуальной функции(возвращает значение count)
        return count;
    }

    void GetMed(istream&) override;
    void PutMed(ostream&) override;
    void Sort() override;
};

void MB2::GetMed(istream&in){//читает данные из файла "input1.txt" и заполняет массив arr объектами типа S2
    ifstream in1("input1.txt");
    count = count_lines(in1);
    arr = new S2[count];
    for(int i =0; i < count; i++){
        in >> arr[i];
    }
}

void MB2::PutMed(ostream&out){//выводит содержимое массива

    ofstream outFile("sortedMB2.txt");
    Sort();
    for(int i = 0; i < count; i++){
        out << arr[i] << endl;
    }

    outFile.close();
}


MB2::MB2(){//конструктор по умолчанию для класса

    count = 0; arr = nullptr;
}

MB2::MB2(int c){//конструктор с параметром для класса

    count = c; arr = new S2[count];
}

MB2::MB2(const MB2& other){//конструктор копирования для класса

    this->count=other.count;
    this->arr=new S2[count];

    for(int i = 0; i < other.count; i++){
        this->arr[i] = other.arr[i];
    }
}

MB2::~MB2(){//деструктор
    delete[]arr;
}

void MB2::Sort(){//cортирует элементы массива arr по возрастанию значения поля Name объектов типа S2

    sort(arr, arr+count, [](const S2& s1, const S2& s2){
        return s1.Name < s2.Name;
    });
}

struct S3 {
    string Name;
    string Date;
    string Section;
    string Price;
    int Count;

    bool operator==(const S3& other) const {
        return Name == other.Name && Date == other.Date && Section == other.Section && Price == other.Price && Count == other.Count;
    }

    friend ostream& operator<<(ostream& os, const S3& rec) {
        os << rec.Name << " " << rec.Date << " " << rec.Section << " " << rec.Price << " " << rec.Count;
        return os;
    }

    friend istream& operator>>(istream& is, S3& rec) {
        is >> rec.Name >> rec.Date >> rec.Section >> rec.Price >> rec.Count;
        return is;
    }
};

class MB3: public MB1, public MB2 {
private:
    S3 * arr;
    int count;



public:
    MB3(){
        arr = nullptr;
         count = 0;
    }
    ~MB3(){//деструктор
        delete[]arr;
    }

void intersection (MB1& a, MB2& b){
    /*S3 k;
    arr = new S3[10];
    k.Name = "a";
    k.Date = "b";
    k.Section = "c";
    k.Count = 1;
    arr[0] = k;
    count = 1;*/

a.Sort();
b.Sort();

int i = 0, j = 0, p = 0;
//delete[] arr;
int acount = a.counter(), bcount = b.counter();
arr = new S3[acount + bcount];
qDebug() << acount << bcount;
 while(i < acount && j < bcount){
    S3 k;

if(a[i].Name < b[j].Name) i++;
else if (a[i].Name > b[j].Name) j++;

else {
k.Name = a[i].Name;
k.Date = a[i].Date;
k.Section = a[i].Section;
k.Count = a[i].Count;
arr[p] = k;
p++;
i++;
j++;
}

}
 count = p;

}




void SaveToFile(string filename){
        /*fstream out(filename);
        for (int i = 0; i < count; i++) {
            out << arr[i] << endl;
        }
        out.close();*/
    fstream out("output.txt");
    for (int i = 0; i < count; i++) {
        out << arr[i] << endl;
    }
    out.close();
    }


/*void SaveToFile(const string& filename) {
        ofstream out3(filename);
        for (int i = 0; i < count; i++) {
            out3 << arr[i] << endl;
        }
        out3.close();
    }*/


    const S3& operator[](int index) const {
        return arr[index];
    }

    void print(){
        for(int i = 0 ; i < count; i++){
            qDebug() << arr[i].Name;
            qDebug() << "sfdaf";
        }
    }
};




osvTarget::osvTarget(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::osvTarget)
{
    ui->setupUi(this);
}


osvTarget::~osvTarget()
{
    delete ui;
}



void osvTarget::on_startButton_clicked()
{
    MB1 M1;
    ifstream in1("input.txt");
    ABS_Med *A1, *A2;
    A1=&M1;
    A1->GetMed(in1);
    //A1->Sort();



    //НЕОТСОРТИРОВАННЫЙ MB1
    QStandardItemModel *model = new QStandardItemModel(this);

    ui->tableView->setModel(model);

    //форматируем таблицу
    model->setRowCount(A1->GetCount()+1);//Устанавливает количество строк в этой модели в columns. Если это меньше, чем columnCount(),
    //данные в нежелательных строках отбрасываются.
    model->setColumnCount(4);//Устанавливает количество столбцов в этой модели в columns. Если это меньше, чем columnCount(),
    //данные в нежелательных столбцах отбрасываются.
    //по умолчанию пронумерованны

    //заполняем таблицу
    model->setData(model->index(0,0), "Название");
    model->setData(model->index(0,1), "Срок годности");
    model->setData(model->index(0,2), "Секция");
    model->setData(model->index(0,3), "Количество");


    for(int i=1; i < A1->GetCount()+1; i++){
        model->setData(model->index(i,0), QString::fromStdString(M1[i-1].Name));
        model->setData(model->index(i,1), QString::fromStdString(M1[i-1].Date));
        model->setData(model->index(i, 2), QString::fromStdString(M1[i-1].Section));
        model->setData(model->index(i,3),M1[i-1].Count);
    }

    //для аккуратности
    ui->tableView->resizeColumnsToContents();//Изменяет размеры всех столбцов
    ui->tableView->resizeRowsToContents();//Изменяет размеры всех строк
    ui->tableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableView->adjustSize();
    ui->tableView->setMinimumSize(ui->tableView->sizeHint());//Размер виджета не может быть изменен до меньшего размера,
    //чем минимальный размер виджета.




    //ОТСОРТИРОВАННЫЙ MB1
    QStandardItemModel *model2 = new QStandardItemModel(this);

    ui->tableView_2->setModel(model2);
    A1->Sort();

    //форматируем таблицу Отсортированный MB1
    model2->setRowCount(A1->GetCount()+1);
    model2->setColumnCount(2);

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
    ui->tableView_2->resizeColumnsToContents();//Изменяет размеры всех столбцов
    ui->tableView_2->resizeRowsToContents();//Изменяет размеры всех строк
    ui->tableView_2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableView_2->adjustSize();
    ui->tableView_2->setMinimumSize(ui->tableView_2->sizeHint());


    MB2 M2;
    A2=&M2;
    ifstream in2("input1.txt");
    A2->GetMed(in2);



    //НЕОТСОРТИРОВАННЫЙ MB2

    QStandardItemModel *model3 = new QStandardItemModel(this);

    ui->tableView_3->setModel(model3);

    //форматируем таблицу
    model3->setRowCount(A2->GetCount()+1);
    model3->setColumnCount(3);

    //заполняем таблицу Неотсортированный MB2
    model3->setData(model3->index(0,0), "Название");
    model3->setData(model3->index(0,1), "Срок годности");
    model3->setData(model3->index(0,2), "Цена");

    for(int i=1; i < A2->GetCount()+1; i++){
        model3->setData(model3->index(i,0), QString::fromStdString(M2[i-1].Name));
        model3->setData(model3->index(i,1), QString::fromStdString(M2[i-1].Date));
        model3->setData(model3->index(i, 2), QString::fromStdString(M2[i-1].Price));
    }

    //для аккуратности
    ui->tableView_3->resizeColumnsToContents();
    ui->tableView_3->resizeRowsToContents();
    ui->tableView_3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableView_3->adjustSize();
    ui->tableView_3->setMinimumSize(ui->tableView_3->sizeHint());
    A2->Sort();



    //ОТСОРТИРОВАННЫЙ MB2

    QStandardItemModel *model4 = new QStandardItemModel(this);

    ui->tableView_4->setModel(model4);

    //форматируем таблицу
    model4->setRowCount(A2->GetCount()+1);
    model4->setColumnCount(3);

     //заполняем таблицу Отсортированный Bag
    model4->setData(model4->index(0,0), "Название");
    model4->setData(model4->index(0,1), "Срок годности");
    model4->setData(model4->index(0,2), "Цена");

    for(int i=1; i < A2->GetCount()+1; i++){
        model4->setData(model4->index(i,0), QString::fromStdString(M2[i-1].Name));
        model4->setData(model4->index(i,1), QString::fromStdString(M2[i-1].Date));
        model4->setData(model4->index(i, 2), QString::fromStdString(M2[i-1].Price));
    }

    //для аккуратности
    ui->tableView_4->resizeColumnsToContents();
    ui->tableView_4->resizeRowsToContents();
    ui->tableView_4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableView_4->adjustSize();
    ui->tableView_4->setMinimumSize(ui->tableView_4->sizeHint());
}


void osvTarget::on_searchButton_clicked()
{
    MB1 M1;
    ifstream in1("input.txt");
    MB2 M2;
    ifstream in2("input1.txt");
    M1.GetMed(in1);
    M2.GetMed(in2);
    bool flag = false;
    string answer;

    // Получаем введенную дату из поля ввода
    string searchDate = ui->search->text().toStdString();

    // Разбиваем дату на месяц и год
    int searchMonth = std::stoi(searchDate.substr(0, 2));
    int searchYear = std::stoi(searchDate.substr(3));

    for (int i = 0; i < M1.GetCount(); i++) {
        // Разбиваем дату из записи на месяц и год
        int recordMonth = std::stoi(M1[i].Date.substr(0, 2));
        int recordYear = std::stoi(M1[i].Date.substr(3));

        // Сравниваем даты
        if (recordYear > searchYear || (recordYear == searchYear && recordMonth >= searchMonth)) {
            flag = true;
            answer += M1[i].Name + " " + M1[i].Date + "\n";
        }
    }

    if (flag == true) {
        QMessageBox::information(nullptr, "Продукт найден", QString::fromStdString(answer));
    }
    else {
        QMessageBox::warning(nullptr, "Увы!", "Товар не найден");
    }
}

void osvTarget::on_search2Button_clicked()
{
    MB1 M1;
    ifstream in1("input.txt");
    MB2 M2;
    ifstream in2("input1.txt");
    M1.GetMed(in1);
    M2.GetMed(in2);
    bool flag = false;
    string answer;

    string searchDate = ui->search2->text().toStdString();

    // Разбиваем дату на месяц и год
    int searchMonth = std::stoi(searchDate.substr(0, 2));
    int searchYear = std::stoi(searchDate.substr(3));

    for (int i = 0; i < M2.GetCount(); i++) {
        // Разбиваем дату из записи на месяц и год
        int recordMonth = std::stoi(M2[i].Date.substr(0, 2));
        int recordYear = std::stoi(M2[i].Date.substr(3));

        // Сравниваем даты
        if (recordYear > searchYear || (recordYear == searchYear && recordMonth >= searchMonth)) {
            flag = true;
            answer += M2[i].Name + " " + M2[i].Date + " " + M2[i].Price + "\n";
        }
    }

    if (flag == true) {
        QMessageBox::information(nullptr, "Продукты найдены", QString::fromStdString(answer));
    }
    else {
        QMessageBox::warning(nullptr, "Увы!", "Товары не найдены");
    }
}


void osvTarget::on_actionSave_triggered() //сохранение отсортипованных массивов в текстовые файлы
{
    MB1 M1;
    ifstream in1("input.txt");
    MB2 M2;
    ifstream in2("input1.txt");
    M1.GetMed(in1);
    M2.GetMed(in2);
    M1.Sort();
    M2.Sort();
    ofstream out1("sortedMB1.txt");
    M1.PutMed(out1);
    ofstream out2("sortedMB2.txt");
    M2.PutMed(out2);
    in1.close();
    in2.close();
    qDebug() << "sfdaf";


    QMessageBox::information(nullptr, "Информация", "Отсортированный массив MB1 записан в файл sortedMB1.txt, Отсортированный массив MB2 записан в файл sortedMB2.txt");
}

void osvTarget::on_pushButton_triggered(){
MB1 M1;
ifstream in1("input.txt");
MB2 M2;
ifstream in2("input1.txt");
M1.GetMed(in1);
M2.GetMed(in2);
M1.Sort();
M2.Sort();

MB3 M3;
M3.intersection(M1,M2);
M3.print();
ofstream out3("output.txt");
M3.SaveToFile("output.txt");
qDebug() << "sfdaf";
}

void osvTarget::on_actionExit_triggered()
{
    this->close();
}



