#include <QCoreApplication>
#include <QtTest>
#include <cmath>
#include "../appFornum/sin.h"
#include "../appFornum/equation.h"
#include "../appFornum/prime.h"
#include "qtestcase.h"

class test : public QObject
{
    Q_OBJECT

public:
    test();
    ~test();

private slots:
    void test_Sin();
    void test_Prime();
    void test_Equation();
};

test::test() {}

test::~test() {}

void test::test_Sin() {
    QVERIFY(fabs(SIN(-1000,0.1).mySin()-sin(-1000*M_PI/180))<0.01);
    QVERIFY(fabs(SIN(0,0.1).mySin()-sin(0*M_PI/180))<0.01);
    QVERIFY(fabs(SIN(30,0.1).mySin()-sin(30*M_PI/180))<0.01);
    QVERIFY(fabs(SIN(-60,0.1).mySin()-sin(-60*M_PI/180))<0.01);
}

void test::test_Prime() {
    QCOMPARE(PRIME(90).primeDivisors()[3],5);
    QCOMPARE(PRIME(90).isPrime(),0);
    QCOMPARE(PRIME(23).isPrime(),1);
    int res=1;
    for(int i=0;i<PRIME(200000000).primeDivisors().size();i++)res*=PRIME(200000000).primeDivisors()[i];
    QCOMPARE(res,200000000);
    QCOMPARE(PRIME(90).primeDivisors()[2],3);
    QCOMPARE(PRIME(420).primeDivisors()[4],7);
    QCOMPARE(PRIME(23).primeDivisors()[0],1);
}

void test::test_Equation() {
    QCOMPARE(pow(EQUATION(1,3,-4).solveQuadratic()[0],2)+3*EQUATION(1,3,-4).solveQuadratic()[0]-4,0);
    QCOMPARE(pow(EQUATION(1,3,-4).solveQuadratic()[1],2)+3*EQUATION(1,3,-4).solveQuadratic()[1]-4,0);
    QCOMPARE(3*pow(EQUATION(3,-4,1).solveQuadratic()[0],2)+(-4)*EQUATION(3,-4,1).solveQuadratic()[0]+1,0);
    QCOMPARE(3*pow(EQUATION(3,-4,1).solveQuadratic()[1],2)+(-4)*EQUATION(3,-4,1).solveQuadratic()[1]+1,0);
    QCOMPARE(EQUATION(1,2,1).solveQuadratic()[0],-1);
    QCOMPARE(EQUATION(1,3,10).solveQuadratic()[0],0);
    QVERIFY(fabs(EQUATION(3,9,-7).solveQuadratic()[0]*EQUATION(3,9,-7).solveQuadratic()[1]+2.33)<0.01);
}

QTEST_MAIN(test)

#include "tst_test.moc"




