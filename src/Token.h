#ifndef DNXLANGUAGE_TOKEN_H
#define DNXLANGUAGE_TOKEN_H
#include <string>

enum class TokenType {
    // --> Literals
    IntLint,
    Identifier,

    // --> Keywords
    Var,
    If,
    Else,
    While,
    Print,

    // --> Operators
    Plus,
    Minus,
    Star,
    Slash,
    Equal,
    Greater,
    Less,

    // --> Delimiters
    LParen,
    RParen,
    LBrace,
    RBrace,
    Semicolon,

    // --> Only Special
    EndOfFile,
    Unknown
};

struct Token {
    TokenType type;
    std::string text; // Raw text
    std::string value; // for IntLint/Identifier

    Token(TokenType token_type, const std::string& txt, const std::string& val = "")
        : type(token_type), text(txt), value(val) {}
};

#endif //DNXLANGUAGE_TOKEN_H
