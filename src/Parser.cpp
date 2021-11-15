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

ParserTree::ParserTree(Lexer const& lexer) {
    _parent = nullptr;
    _kidIndex = -1;
    std::stack<Token> op_stack;
    std::vector<Token> rpn;  // reverse polish notation

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
                while (!op_stack.empty() &&
                       op_stack.top().type != TokenType::PARENTHESIS_OPEN) {
                    rpn.push_back(op_stack.top());
                    op_stack.pop();
                }

                if (op_stack.empty())
                    throw std::logic_error("unmatched parentesis.");

                op_stack.pop();
                break;
            case TokenType::INVALID:
                throw std::logic_error("Pelo menos é invalido!");
            default:  // operator or function
                while (!op_stack.empty() &&
                       Precedence::compare(op_stack.top(), token)) {
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

    ParserTree* parser = build(rpn);
    *this = *parser;
    delete parser;

    if (!rpn.empty()) throw std::runtime_error("bad input! (1)");
}

ParserTree* ParserTree::build(std::vector<Token>& rpn) {
    if (rpn.empty()) throw std::runtime_error("bad input! (2)");

    ParserTree* parser = new ParserTree();
    parser->_token = rpn.back();
    rpn.pop_back();

    switch (parser->_token.type) {
        case TokenType::OP_PLUS:
        case TokenType::OP_MINUS:
        case TokenType::OP_PRODUCT:
        case TokenType::OP_DIVIDE:
        case TokenType::OP_POWER:
            parser->_children.resize(2);
            parser->_children[1] = build(rpn);
            parser->_children[1]->_parent = parser;
            parser->_children[1]->_kidIndex = 1;
            parser->_children[0] = build(rpn);
            parser->_children[0]->_kidIndex = 0;
            parser->_children[0]->_parent = parser;
            break;

        case TokenType::NUMBER:  // leaf node
            if (!isNumber(*parser->_token.name))
                throw std::runtime_error("invalid double!");

            parser->_double = std::stod(*parser->_token.name);
            break;
        case TokenType::VARIABLE:  // leaf node
        case TokenType::VARIABLE_Y:
            break;

        default:  // function or unary operators
            // if (Functions::unaries.count(*_token.name) == 0
            //     throw std::runtime_error("invalid )function name!");

            parser->_children.resize(1);
            parser->_children[0] = build(rpn);
            break;
    }

    return parser;
}

double ParserTree::evaluate(double x, double y) const {
    switch (_token.type) {
        case TokenType::VARIABLE:
            return x;
        case TokenType::VARIABLE_Y:
            return y;
        case TokenType::NUMBER:
            return *_double;

        case TokenType::OP_PLUS:
            return _children[0]->evaluate(x, y) + _children[1]->evaluate(x, y);
        case TokenType::OP_MINUS:
            return _children[0]->evaluate(x, y) - _children[1]->evaluate(x, y);
        case TokenType::OP_PRODUCT:
            return _children[0]->evaluate(x, y) * _children[1]->evaluate(x, y);
        case TokenType::OP_DIVIDE:
            return _children[0]->evaluate(x, y) / _children[1]->evaluate(x, y);
        case TokenType::OP_POWER:
            return std::pow(_children[0]->evaluate(x, y),
                            _children[1]->evaluate(x, y));

        case TokenType::OP_POSITIVE:
        case TokenType::OP_NEGATIVE:
        case TokenType::FUNCTION:
            return Functions::unaries[*_token.name](
                _children[0]->evaluate(x, y));

        default:
            throw std::runtime_error("como?");
    }
}

void ParserTree::simplify(ParserTree*& parser) {
    for (auto& v : parser->_children) {
        simplify(v);
    }

    if (parser->_children.size() == 0) {
        return;
    }

    if (parser->_children.size() == 1 &&
        parser->_children[0]->_token.type == TokenType::NUMBER) {
        double x = parser->evaluate(*parser->_children[0]->_double);
        delete parser->_children[0];
        parser->_children.clear();
        parser->_token.type = TokenType::NUMBER;
        parser->_token.name = std::to_string(x);
        parser->_double = x;
        return;
    }

    if (parser->_children.size() == 2 &&
        parser->_children[0]->_token.type == TokenType::NUMBER &&
        parser->_children[1]->_token.type == TokenType::NUMBER) {
        double x = parser->evaluate(*parser->_children[0]->_double,
                                    *parser->_children[1]->_double);
        delete parser->_children[0];
        delete parser->_children[1];
        parser->_children.clear();
        parser->_token.type = TokenType::NUMBER;
        parser->_token.name = std::to_string(x);
        parser->_double = x;
        return;
    }

    switch (parser->_token.type) {
        case TokenType::OP_POWER: {
            if (parser->_children[1]->_token.type == TokenType::NUMBER &&
                parser->_children[1]->_double == 0) {
                delete parser->_children[0];
                delete parser->_children[1];
                parser->_children.clear();
                parser->_token.type = TokenType::NUMBER;
                parser->_token.name = "1";
                parser->_double = 1;

            } else if (parser->_children[1]->_token.type == TokenType::NUMBER &&
                       parser->_children[1]->_double == 1) {
                delete parser->_children[1];
                ParserTree *aux = parser->_children[0], *aux2 = parser;
                aux->_parent = parser->_parent;
                aux->_kidIndex = parser->_kidIndex;
                parser = aux;
                delete aux2;
            }
            break;
        }
        case TokenType::OP_PRODUCT: {
            if (parser->_children[1]->_token.type == TokenType::NUMBER &&
                (parser->_children[1]->_double == 0 ||
                 parser->_children[0]->_double == 0)) {
                delete parser->_children[0];
                delete parser->_children[1];
                parser->_children.clear();
                parser->_token.type = TokenType::NUMBER;
                parser->_token.name = "0";
                parser->_double = 0;

            } else if (parser->_children[1]->_token.type == TokenType::NUMBER &&
                       parser->_children[1]->_double == 1) {
                delete parser->_children[1];
                ParserTree *aux = parser->_children[0], *aux2 = parser;
                aux->_parent = parser->_parent;
                aux->_kidIndex = parser->_kidIndex;
                parser = aux;
                delete aux2;
            } else if (parser->_children[0]->_token.type == TokenType::NUMBER &&
                       parser->_children[0]->_double == 1) {
                delete parser->_children[0];
                ParserTree *aux = parser->_children[1], *aux2 = parser;
                aux->_parent = parser->_parent;
                aux->_kidIndex = parser->_kidIndex;
                parser = aux;
                delete aux2;
            }
            break;
        }

        case TokenType::OP_POSITIVE: {
            ParserTree *aux = parser->_children[0], *aux2 = parser;
            aux->_parent = parser->_parent;
            aux->_kidIndex = parser->_kidIndex;
            parser = aux;
            delete aux2;
            break;
        }

        default: {
            break;
        }
    }
}

std::string ParserTree::eval() const {
    std::string ans;
    switch (_token.type) {
        case TokenType::VARIABLE:
            return "x";
        case TokenType::VARIABLE_Y:
            return "y";
        case TokenType::NUMBER:
            return *_token.name;

        case TokenType::OP_PLUS:
            return _children[0]->eval() + "+" + _children[1]->eval();
        case TokenType::OP_MINUS:
            return _children[0]->eval() + "-" + _children[1]->eval();
        case TokenType::OP_PRODUCT:
            return _children[0]->eval() + "*" + _children[1]->eval();
        case TokenType::OP_DIVIDE:
            return _children[0]->eval() + "/" + _children[1]->eval();
        case TokenType::OP_POWER:
            return "pow(" + _children[0]->eval() + ", " + _children[1]->eval() +
                   ")";

        case TokenType::OP_POSITIVE:
        case TokenType::OP_NEGATIVE:
        case TokenType::FUNCTION:
            return *_token.name + "(" + _children[0]->eval() + ")";

        default:
            throw std::runtime_error("como?");
    }
}

