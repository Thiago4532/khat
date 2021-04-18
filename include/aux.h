#ifndef _AUX_H
#define _AUX_H

#include <iostream>

#include "Token.h"

extern std::ostream& operator<<(std::ostream& out, Token const& t);

#endif