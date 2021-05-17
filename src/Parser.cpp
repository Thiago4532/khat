#include "Parser.h"

#include <algorithm>
#include <cmath>
#include <stack>
#include <stdexcept>

#include "Functions.h"
#include "Precedence.h"
#include "aux.h"

bool isNumber(std::string const& str) {
    return std::count(str.begin(), str.end(), '.') <= 1;
}

Parser::Parser(Lexer const& lexer) {
    std::stack<Token> op_stack;
    std::vector<Token> rpn; // reverse polish notation

    for (auto const& token : lexer) {
        switch (token.type) {
            case TokenType::NUMBER:
            case TokenType::VARIABLE:
            case TokenType::VARIABLE_Y:
                rpn.push_back(token);
                break;
            case TokenType::PARENTHESIS_OPEN:
                op_stack.push(token);
                break;
            case TokenType::PARENTHESIS_CLOSE:
                while (!op_stack.empty() && op_stack.top().type != TokenType::PARENTHESIS_OPEN) {
                    rpn.push_back(op_stack.top());
                    op_stack.pop();
                }

                if (op_stack.empty())
                    throw std::logic_error("unmatched parentesis.");

                op_stack.pop();
                break;
            case TokenType::INVALID:
                throw std::logic_error("Pelo menos é invalido!");
            default: // operator or function
                while (!op_stack.empty() && Precedence::compare(op_stack.top(), token)) {
                    rpn.push_back(op_stack.top());
                    op_stack.pop();
                }
                op_stack.push(token);
                break;
        }
    }

    // close all parenthesis and push to rpn
    while (!op_stack.empty()) {
        if (op_stack.top().type != TokenType::PARENTHESIS_OPEN)
            rpn.push_back(op_stack.top());
        op_stack.pop();
    }

    this->_build(rpn);
    if (!rpn.empty())
        throw std::runtime_error("bad input! (1)");
}

void Parser::_build(std::vector<Token>& rpn) {
    if (rpn.empty())
        throw std::runtime_error("bad input! (2)");

    _token = rpn.back();
    rpn.pop_back();

    switch (_token.type) {
        case TokenType::OP_PLUS:
        case TokenType::OP_MINUS:
        case TokenType::OP_PRODUCT:
        case TokenType::OP_DIVIDE:
        case TokenType::OP_POWER:
            this->_children.resize(2);
            this->_children[1]._build(rpn);
            this->_children[0]._build(rpn);
            break;

        case TokenType::NUMBER: // leaf node
            if (!isNumber(*_token.name))
                throw std::runtime_error("invalid double!");

            _double = std::stod(*_token.name);
            break;
        case TokenType::VARIABLE: // leaf node
        case TokenType::VARIABLE_Y:
            break;

        default: // function or unary operators
            // if (Functions::unaries.count(*_token.name) == 0
            //     throw std::runtime_error("invalid )function name!");

            this->_children.resize(1);
            this->_children[0]._build(rpn);
            break;
    }
}

double Parser::evaluate(double x) const {
    return evaluate(x, 0);
}

double Parser::evaluate(double x, double y) const {
    switch (_token.type) {
        case TokenType::VARIABLE:
            return x;
        case TokenType::VARIABLE_Y:
            return y;
        case TokenType::NUMBER:
            return *_double;

        case TokenType::OP_PLUS:
            return _children[0].evaluate(x, y) + _children[1].evaluate(x, y);
        case TokenType::OP_MINUS:
            return _children[0].evaluate(x, y) - _children[1].evaluate(x, y);
        case TokenType::OP_PRODUCT:
            return _children[0].evaluate(x, y) * _children[1].evaluate(x, y);
        case TokenType::OP_DIVIDE:
            return _children[0].evaluate(x, y) / _children[1].evaluate(x, y);
        case TokenType::OP_POWER:
            return std::pow(_children[0].evaluate(x, y), _children[1].evaluate(x, y));

        case TokenType::OP_POSITIVE:
        case TokenType::OP_NEGATIVE:
        case TokenType::FUNCTION:
            return Functions::unaries[*_token.name](_children[0].evaluate(x, y));

        default:
            throw std::runtime_error("como?");
    }
}

std::string Parser::eval() const {
    std::string ans;
    switch (_token.type) {
        case TokenType::VARIABLE:
            return "x";
        case TokenType::VARIABLE_Y:
            return "y";
        case TokenType::NUMBER:
            return *_token.name;

        case TokenType::OP_PLUS:
            return _children[0].eval() + "+" + _children[1].eval();
        case TokenType::OP_MINUS:
            return _children[0].eval() + "-" + _children[1].eval();
        case TokenType::OP_PRODUCT:
            return _children[0].eval() + "*" + _children[1].eval();
        case TokenType::OP_DIVIDE:
            return _children[0].eval() + "/" + _children[1].eval();
        case TokenType::OP_POWER:
            return "pow(" + _children[0].eval() + ", " + _children[1].eval() + ")";

        case TokenType::OP_POSITIVE:
        case TokenType::OP_NEGATIVE:
        case TokenType::FUNCTION:
            return *_token.name + "(" + _children[0].eval() + ")";

        default:
            throw std::runtime_error("como?");
    }
}

double Parser::operator[](double x) const { return evaluate(x, 0); };
Token const& Parser::getToken() const { return this->_token; }

std::vector<Parser> const& Parser::getChildren() const {
    return this->_children;
}