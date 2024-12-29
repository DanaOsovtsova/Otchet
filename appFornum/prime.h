#ifndef PRIME_H
#define PRIME_H
#include <QVector>

class PRIME{
private:
    unsigned Num;
public:
    PRIME(){
        Num=1;
    };
    PRIME(unsigned n){
        Num=n;
    }
    ~PRIME(){};
    bool isPrime() {
        if (Num <= 1) {
            return false;
        }
        for (unsigned int i = 2; i * i <= Num; ++i) {
            if (Num % i == 0) {
                return false;
            }
        }
        return true;
    }

    QVector<unsigned int> primeDivisors() {
        QVector<unsigned int> divisors;
        if(isPrime()){
            divisors.push_back(1);;
            return divisors;
        }
        for (unsigned int i = 2; i * i <= Num; ++i) {
            while (Num % i == 0) {
                divisors.push_back(i);
                Num /= i;
            }
        }
        if (Num > 1) {
            divisors.push_back(Num);
        }
        return divisors;
    }
};

#endif // PRIME_H
