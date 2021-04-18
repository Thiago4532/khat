#ifndef _LEXER_H
#define _LEXER_H

#include <string>
#include <vector>

#include "Token.h"

class Lexer : public std::vector<Token> {
public:
    Lexer(std::string const& input);

private:
    std::string _input;
};

#endif