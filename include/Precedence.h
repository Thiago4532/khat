#ifndef _PRECEDENCE_H
#define _PRECEDENCE_H

#include "Token.h"

namespace Precedence {

// Return true if the group has right-to-left associativity
bool isRightToLeft(int group);

// Return true if left is solved before right
bool compare(int left, int right);
bool compare(Token const& left, Token const& right);

}  // namespace Precedence

#endif