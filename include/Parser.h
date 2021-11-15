#ifndef _PARSER_H
#define _PARSER_H

#include <initializer_list>
#include <optional>
#include <string>
#include <vector>

#include "Lexer.h"

class ParserNode {
   private:
    ParserNode() = default;
    ParserNode(Lexer const& lexer);

    double evaluate(double x, double y = 0) const;
    std::string eval() const;
    static ParserNode* build(std::vector<Token>& rpn);
    // static void simplify(ParserTree*& parser);

    ParserNode(Token const& token, std::initializer_list<ParserNode*> children);
    ParserNode(int num);
    ParserNode(double num);

    static ParserNode* copy(ParserNode* p);
    static ParserNode* derivative(ParserNode* p, char var);

    Token _token;

    std::vector<ParserNode*> _children;
    std::optional<double> _double;

    friend class Parser;
};

class Parser {
   public:
    Parser(Lexer const& lexer) : tree(new ParserNode(lexer)) {}

    ~Parser() { delete tree; }

    double operator[](double x) const { return evaluate(x, 0); };
    Token const& getToken() const { return tree->_token; }

    // void simplify() { ParserTree::simplify(tree); };
    Parser derivative(char var) const {
        return ParserNode::derivative(tree, var);
    }
    Parser derivative() const { return ParserNode::derivative(tree, 'x'); }

    double evaluate(double x) const { return tree->evaluate(x, 0); }
    double evaluate(double x, double y) const { return tree->evaluate(x, y); }
    std::string eval() const { return tree->eval(); }

   private:
    Parser(ParserNode* tree) : tree(tree) {}
    ParserNode* tree;
};
#endif
