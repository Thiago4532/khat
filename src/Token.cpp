#include "Token.h"

const Token Token::OP_PLUS{ TokenType::OP_PLUS, 4 };
const Token Token::OP_MINUS{ TokenType::OP_MINUS, 4 };
const Token Token::OP_PRODUCT{ TokenType::OP_PRODUCT, 3 };
const Token Token::OP_DIVIDE{ TokenType::OP_DIVIDE, 3 };
const Token Token::OP_POWER{ TokenType::OP_POWER, 0 };
const Token Token::OP_POSITIVE{ TokenType::OP_POSITIVE, 0, "+" };
const Token Token::OP_NEGATIVE{ TokenType::OP_NEGATIVE, 0, "-" };
const Token Token::PARENTHESIS_OPEN{ TokenType::PARENTHESIS_OPEN };
const Token Token::PARENTHESIS_CLOSE{ TokenType::PARENTHESIS_CLOSE };
const Token Token::VARIABLE{ TokenType::VARIABLE };
const Token Token::VARIABLE_Y{ TokenType::VARIABLE_Y };
const Token Token::INVALID{ TokenType::INVALID, 11, "???" };

Token Token::number(std::string_view num) {
    return { TokenType::NUMBER, 10, std::string(num) };
}

Token Token::number(int num) {
    return { TokenType::NUMBER, 10, std::to_string(num) };
}

Token Token::number(double num) {
    return { TokenType::NUMBER, 10, std::to_string(num) };
}

Token Token::func(std::string_view name) {
    return { TokenType::FUNCTION, 0, std::string(name) };
}
