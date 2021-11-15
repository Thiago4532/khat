#include "Lexer.h"

const std::string alphanum_ = "abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
const std::string digits = "0123456789.";
const std::string spaces = " \t\r\n";

bool isTokenANumber(TokenType const& token) {
    switch (token) {
        case TokenType::NUMBER:
        case TokenType::VARIABLE:
        case TokenType::VARIABLE_Y:
        case TokenType::PARENTHESIS_CLOSE:
            return true;
            break;
        default:
            return false;
            break;
    }
}

#include <iostream>
Lexer::Lexer(std::string const& input)
    : _input(input) {
    TokenType lastToken = TokenType::INVALID;

    for (size_t i = 0; i < input.length(); i++) {
        if (spaces.find(input[i]) != std::string::npos)
            continue;

        auto ch = input[i];

        switch (ch) {
            case '+':
                if (isTokenANumber(lastToken))
                    this->push_back(Token::OP_PLUS);
                else
                    this->push_back(Token::OP_POSITIVE);

                break;
            case '-':
                if (isTokenANumber(lastToken))
                    this->push_back(Token::OP_MINUS);
                else
                    this->push_back(Token::OP_NEGATIVE);

                break;
            case '*':
                this->push_back(Token::OP_PRODUCT);
                break;
            case '/':
                this->push_back(Token::OP_DIVIDE);
                break;
            case '^':
                this->push_back(Token::OP_POWER);
                break;
            case 'x':
                this->push_back(Token::VARIABLE);
                break;
            case 'y':
                this->push_back(Token::VARIABLE_Y);
                break;
            case '(':
                this->push_back(Token::PARENTHESIS_OPEN);
                break;
            case ')':
                this->push_back(Token::PARENTHESIS_CLOSE);
                break;
            default:
                std::string tokenStr;
                // check if token is a number

                while (i < input.length() && digits.find(input[i]) != std::string::npos)
                    tokenStr += input[i++];

                if (!tokenStr.empty()) {
                    this->push_back(Token::number(tokenStr));
                    i--;
                    break;
                }

                // check if token is a function

                while (i < input.length() && alphanum_.find(input[i]) != std::string::npos)
                    tokenStr += input[i++];

                if (!tokenStr.empty()) {
                    this->push_back(Token::func(tokenStr));
                    i--;
                    break;
                }

                // unrecognized token

                this->push_back(Token::INVALID);
                break;
        }
        lastToken = this->back().type;
    }
}
