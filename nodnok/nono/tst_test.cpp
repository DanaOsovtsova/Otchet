#include <QtTest>
#include "../nodnok/chisla.h"

// add necessary includes here

class test : public QObject
{
    Q_OBJECT

public:
    test();
    ~test();

private slots:
    void test_nod();
    void test_nok();
};

test::test() {}

test::~test() {}

void test::test_nod() {
    chisla a;
    QCOMPARE(a.nod(2,3), 1);
    QCOMPARE(a.nod(-7,77), -7);
    QCOMPARE(a.nod(0,4), 4);
}
void test::test_nok() {
    chisla a;
    QCOMPARE(a.nok(2,3), 6);
    QCOMPARE(a.nok(-7,77), 77);
    QCOMPARE(a.nok(0,4), 0);
}

QTEST_APPLESS_MAIN(test)

#include "tst_test.moc"
