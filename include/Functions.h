#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include <string>
#include <unordered_map>

namespace Functions {

extern std::unordered_map<std::string, double (*)(double)> unaries;

}

#endif