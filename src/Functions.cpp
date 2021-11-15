#include "Functions.h"

#include <cmath>

static double unary_plus(double x) { return x; }

static double unary_minus(double x) { return -x; }

std::unordered_map<std::string, double (*)(double)> Functions::unaries = {
    { "sin", sin },
    { "cos", cos },
    { "tan", tan },
    { "sqrt", sqrt },
    { "exp", exp },
    { "log", log },
    { "+", unary_plus },
    { "-", unary_minus }
};
