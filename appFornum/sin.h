#ifndef SIN_H
#define SIN_H
#include <cmath>
class SIN {
private:
    double Arg;
    double eps;
public:
    SIN(){
        Arg=0;
        eps=0.1;
    };
    SIN(double x, double n){
        Arg=x;
        while(Arg<-360)Arg+=360;
        while(Arg>360)Arg-=360;
        eps=n;
    }
    ~SIN(){}
    double mySin() {
        Arg = Arg * M_PI / 180.0; // Преобразуем аргумент в радианы
        double term = Arg;
        double result = term;
        int n = 1;

        while (fabs(term) > eps) {
            term *= (-1) * Arg * Arg / ((2 * n) * (2 * n + 1));
            result += term;
            n++;
        }

        return result;
    }
};

#endif // SIN_H
