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

    std::vector<Token> tokenize();

private:
    std::string source;
    size_t pos;

    char current() const;
    char peek() const;
    void advance();
    void skip_whitespace_and_comments();
    Token read_number();
    Token read_identifier_or_keyword();
    Token read_single_char_token();
};

#endif //DNXLANGUAGE_LEXER_H
