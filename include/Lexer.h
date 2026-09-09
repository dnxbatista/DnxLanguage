// The lexer is resposible to make the tokens, if you dont know what is the tokens, open token.h
// or just search...

#ifndef DNXLANGUAGE_LEXER_H
#define DNXLANGUAGE_LEXER_H
#include "Token.h"
#include <string>
#include <vector>

class Lexer {
public:
    explicit Lexer(const std::string& source); // The explicit keyword is something related to buffer, idk

    std::vector<token> tokenize();

private:
    std::string source;
    size_t pos;

    char current() const;
    char peek() const;
    void advance();
    void skip_whitespace_and_comments();
    token read_number();
    token read_identifier_or_keyword();
    token read_single_char_token();
};

#endif //DNXLANGUAGE_LEXER_H
