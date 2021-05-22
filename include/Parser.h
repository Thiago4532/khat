#ifndef _PARSER_H
#define _PARSER_H

#include <optional>
#include <string>
#include <vector>

#include "Lexer.h"

class Parser {
public:
    Parser() = default;
    Parser(Lexer const& lexer);
    double operator[](double x) const;

    Token const& getToken() const;

    static void simplify(Parser*& parser);

    double evaluate(double x) const;
    double evaluate(double x, double y) const;
    std::string eval() const;

private:
    Token _token;
    std::vector<Parser*> _children;
    Parser* _parent;
    int _kidIndex;
    std::optional<double> _double;
    static Parser* _build(std::vector<Token>& rpn);
};
#endif
