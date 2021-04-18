#include "Precedence.h"

#include <stdexcept>

bool Precedence::isRightToLeft(int group) {
    // Lower group = Higher priority
    switch (group) {
        // Group 0: Functions, unary op., power || right-to-left associativity
        case 0:
            return true;

        // Group 3: Multiplication, Division || left-to-right associativity
        case 3:
            return false;

        // Group 4: Addition and Subtraction || left-to-right associativity
        case 4:
            return false;

        // Group 10: Number, Variables, etc || left-to-right associativity
        case 10:
            return false;

        // Group ?: Throw std::exception
        default:
            throw std::length_error("Invalid precedence group!");
    }
}

#include <iostream>
bool Precedence::compare(int left, int right) {
    if (left < right)
        return true;
    if (left > right)
        return false;

    return !isRightToLeft(left);
}

bool Precedence::compare(Token const& left, Token const& right) {
    return compare(left.precedenceGroup, right.precedenceGroup);
}