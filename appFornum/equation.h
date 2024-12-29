#ifndef EQUATION_H
#define EQUATION_H
#include <QVector>
class EQUATION{
private:
    double A, B, C;
public:
    EQUATION(){
        A=0;
        B=0;
        C=0;
    };
    EQUATION(double a, double b, double c){
        A=a;
        B=b;
        C=c;
    };
    ~EQUATION(){};
    QVector<double> solveQuadratic() {
        QVector<double> roots;
        double discriminant = B * B - 4 * A * C;

        if (discriminant > 0) {
            double root1 = (-B + sqrt(discriminant)) / (2 * A);
            double root2 = (-B - sqrt(discriminant)) / (2 * A);
            roots.push_back(root1);
            roots.push_back(root2);
        } else if (discriminant == 0) {
            double root = -B / (2 * A);
            roots.push_back(root);
            roots.push_back(root);
        } else if (discriminant < 0) {
            roots.push_back(0);
            roots.push_back(0);
        }

        return roots;
    }

};

#endif // EQUATION_H
