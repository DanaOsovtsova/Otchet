#include "chisla.h"

chisla::chisla(): num1(0), num2(0) {}

chisla::chisla(int a, int b){
    num1=a;
    num2=b;
}

int chisla::nod(int a, int b){
    num1=a;
    num2=b;
    if (num1%num2 == 0)
        return num2;
    if (num2%num1 == 0)
        return num1;
    if (num1>num2)
        return nod(num1 % num2, num2);
    return nod(num1,num2%num1);
}

int chisla::nok(int a, int b){
    num1 = a;
    num2 = b;
    return (num1 * num2)/nod(num1, num2);
}
