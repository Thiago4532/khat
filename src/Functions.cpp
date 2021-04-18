#include "Functions.h"

#include <cmath>

double unary_plus(double x) { return x; }

double unary_minus(double x) { return -x; }

std::unordered_map<std::string, double (*)(double)> Functions::unaries = {
    { "sin", sin },
    { "cos", cos },
    { "tan", tan },
    { "sqrt", sqrt },
    { "exp", exp },
    { "+", unary_plus },
    { "-", unary_minus }
};