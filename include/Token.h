#ifndef _TOKEN_H
#define _TOKEN_H
#include <optional>
#include <string>

enum class TokenType {
    NUMBER,
    OP_PLUS,
    OP_MINUS,
    OP_PRODUCT,
    OP_DIVIDE,
    OP_POWER,
    OP_POSITIVE,
    OP_NEGATIVE,
    PARENTHESIS_OPEN,
    PARENTHESIS_CLOSE,
    FUNCTION,
    VARIABLE,
    VARIABLE_Y,
    INVALID,
};

struct Token {
    TokenType type;
    int precedenceGroup = 10;
    std::optional<std::string> name;
};

#endif