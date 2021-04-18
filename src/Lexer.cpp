#include "Lexer.h"

const std::string alphanum_ =
    "abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
const std::string digits = "0123456789.";
const std::string spaces = " \t\r\n";

bool isTokenANumber(TokenType const& token) {
    switch (token) {
        case TokenType::NUMBER:
        case TokenType::VARIABLE:
        case TokenType::PARENTHESIS_CLOSE:
            return true;
            break;
        default:
            return false;
            break;
    }
}

Lexer::Lexer(std::string const& input) : _input(input) {
    TokenType lastToken = TokenType::INVALID;

    for (size_t i = 0; i < input.length(); i++) {
        if (spaces.find(input[i]) != std::string::npos) continue;

        auto ch = input[i];

        switch (ch) {
            case '+':
                if (isTokenANumber(lastToken))
                    this->push_back({TokenType::OP_PLUS, 4});
                else
                    this->push_back({TokenType::OP_POSITIVE, 0, "+"});

                break;
            case '-':
                if (isTokenANumber(lastToken))
                    this->push_back({TokenType::OP_MINUS, 4});
                else
                    this->push_back({TokenType::OP_NEGATIVE, 0, "-"});

                break;
            case '*':
                this->push_back({TokenType::OP_PRODUCT, 3});
                break;
            case '/':
                this->push_back({TokenType::OP_DIVIDE, 3});
                break;
            case '^':
                this->push_back({TokenType::OP_POWER, 0});
                break;
            case 'x':
                this->push_back({TokenType::VARIABLE});
                break;
            case '(':
                this->push_back({TokenType::PARENTHESIS_OPEN});
                break;
            case ')':
                this->push_back({TokenType::PARENTHESIS_CLOSE});
                break;
            default:
                std::string tokenStr;

                // check if token is a number

                while (i < input.length() &&
                       digits.find(input[i]) != std::string::npos)
                    tokenStr += input[i++];

                if (!tokenStr.empty()) {
                    this->push_back({TokenType::NUMBER, 10, tokenStr});
                    i--;
                    break;
                }

                // check if token is a function

                while (i < input.length() &&
                       alphanum_.find(input[i]) != std::string::npos)
                    tokenStr += input[i++];

                if (!tokenStr.empty()) {
                    this->push_back({TokenType::FUNCTION, 0, tokenStr});
                    i--;
                    break;
                }

                // unrecognized token

                this->push_back({TokenType::INVALID, 11, "???"});
                break;
        }
        lastToken = this->back().type;
    }
}