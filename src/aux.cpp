#include "aux.h"

using namespace std;

ostream& operator<<(ostream& out, Token const& t) {
    string valor;
    switch (t.type) {
        case TokenType::NUMBER:
            valor = "NUMBER";
            break;
        case TokenType::OP_PLUS:
            valor = "OP_PLUS";
            break;
        case TokenType::OP_MINUS:
            valor = "OP_MINUS";
            break;
        case TokenType::OP_PRODUCT:
            valor = "OP_PRODUCT";
            break;
        case TokenType::OP_DIVIDE:
            valor = "OP_DIVIDE";
            break;
        case TokenType::OP_POWER:
            valor = "OP_POWER";
            break;
        case TokenType::OP_POSITIVE:
            valor = "OP_POSITIVE";
            break;
        case TokenType::OP_NEGATIVE:
            valor = "OP_NEGATIVE";
            break;
        case TokenType::PARENTHESIS_OPEN:
            valor = "PARENTHESIS_OPEN";
            break;
        case TokenType::PARENTHESIS_CLOSE:
            valor = "PARENTHESIS_CLOSE";
            break;
        case TokenType::FUNCTION:
            valor = "FUNCTION";
            break;
        case TokenType::VARIABLE:
            valor = "VARIABLE";
            break;
        case TokenType::INVALID:
            valor = "INVALID";
            break;
    }

    if (t.name.has_value())
        out << "{" << *t.name << ", " << valor << "}";
    else
        out << "{" << valor << "}";
    return out;
}