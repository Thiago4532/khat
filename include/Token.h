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

    static const Token OP_PLUS;
    static const Token OP_MINUS;
    static const Token OP_PRODUCT;
    static const Token OP_DIVIDE;
    static const Token OP_POWER;
    static const Token OP_POSITIVE;
    static const Token OP_NEGATIVE;
    static const Token PARENTHESIS_OPEN;
    static const Token PARENTHESIS_CLOSE;
    static const Token VARIABLE;
    static const Token VARIABLE_Y;
    static const Token INVALID;

    static Token number(int num);
    static Token number(double num);
    static Token number(std::string_view num);

    static Token func(std::string_view name);
};

#endif
