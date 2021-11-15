#ifndef _PARSER_H
#define _PARSER_H

#include <optional>
#include <string>
#include <vector>

#include "Lexer.h"

class ParserTree {
private:
    ParserTree() = default;
    ParserTree(Lexer const& lexer);

    double evaluate(double x, double y = 0) const;
    std::string eval() const;
    static ParserTree* build(std::vector<Token>& rpn);
    static void simplify(ParserTree*& parser);

    Token _token;
    std::vector<ParserTree*> _children;
    ParserTree* _parent;
    int _kidIndex;
    std::optional<double> _double;


    friend class Parser;
};

class Parser {
public:
    Parser(): tree(new ParserTree) {}
    Parser(Lexer const& lexer): tree(new ParserTree(lexer)) {  }

    ~Parser() { delete tree; }

    double operator[](double x) const { return evaluate(x, 0); };
    Token const& getToken() const { return tree->_token; }

    void simplify() { ParserTree::simplify(tree); } ;

    double evaluate(double x) const { return tree->evaluate(x, 0); }
    double evaluate(double x, double y) const { return tree->evaluate(x, y); }
    std::string eval() const { return tree->eval(); }

private:
    ParserTree* tree;
};
#endif
