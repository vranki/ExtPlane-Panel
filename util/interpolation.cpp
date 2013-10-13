#include "interpolation.h"

double Interpolation::linear(double x1, double x2, double y1, double y2, double x)
{
    if(x2==x1) return y1;
    double mu = (x - x1) / (x2 - x1);
    double result = y1 + mu * (y2 - y1);
    return result;
}
