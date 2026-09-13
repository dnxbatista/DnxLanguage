// A token is like the words of the language, the lexer get the raw text from the .dnx file and turn into tokens

#ifndef DNXLANGUAGE_TOKEN_H
#define DNXLANGUAGE_TOKEN_H
#include <string>

// Responsible for language types and expressions and etc, like IntLint is a int
enum class token_type {
    // -> Literals
    IntLiteral,
    StringLiteral,
    Identifier,

    // -> Types
    Int,
    String,

    // -> Keywords
    If,
    Else,
    While,
    Print,

    // -> Operators
    Plus,
    Minus,
    Star,
    Slash,
    Equal,
    Greater,
    Less,

    // -> Delimiters
    LParen,
    RParen,
    LBrace,
    RBrace,
    Semicolon,

    // -> Only Special
    EndOfFile,
    Unknown
};

// This struct holdes the the type, the raw value and the value from the token
struct token {
    token_type type;
    std::string text; // Raw text
    std::string value; // for IntLint/Identifier

    token(token_type token_type, const std::string& txt, const std::string& val = "")
        : type(token_type), text(txt), value(val) {}
};

#endif //DNXLANGUAGE_TOKEN_H
