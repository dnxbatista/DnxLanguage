#include "Lexer.h"
#include <cctype>
#include <unordered_map>

static const std::unordered_map<std::string, TokenType> keywords = {
    {"var", TokenType::Var},
    {"if", TokenType::If},
    {"else", TokenType::Else},
    {"while", TokenType::While},
    {"print", TokenType::Print},
};

Lexer::Lexer(const std::string& source) : source(source), pos(0) {}

char Lexer::current() const {
    if (pos >= source.size()) {
        return '\0'; // return a null character
    }
    return source[pos];
}

char Lexer::peek() const {
    if (pos +1 >= source.size()) {
        return '\0';
    }
    return source[pos+1];
}

void Lexer::advance() {
    if (pos < source.size()) ++pos;
}

void Lexer::skip_whitespace_and_comments() { // THIS IS NOT WORKING AS IS SUPPOSED TO BE
    while (true) {
        char c = current();
        if (std::isspace(static_cast<unsigned char>(c))) {
        advance();
        continue;
        }

        if (c == '/' && peek() == '/') {
            while (current() != '\0' && current() != ' ') {
                advance();
            }
            continue;
        }
        break;
    }
}

Token Lexer::read_number() {
    std::string number;
    while (std::isdigit(static_cast<unsigned char>(current()))) {
        number += current();
        advance();
    }
    return Token(TokenType::IntLint, number, number);
}

Token Lexer::read_identifier_or_keyword() {
    std::string id;
    while (std::isalnum(static_cast<unsigned char>(current())) || current() == '_') {
        id += current();
        advance();
    }
    auto it = keywords.find(id);
    if (it != keywords.end()) {
        return Token(it->second, id, id);
    }
    return Token(TokenType::Identifier, id, id);
}

Token Lexer::read_single_char_token() {
    char c = current();
    advance();
    switch (c) {
        case '+': return Token(TokenType::Plus, "+");
        case '-': return Token(TokenType::Minus, "-");
        case '*': return Token(TokenType::Star, "*");
        case '/': return Token(TokenType::Slash, "/");
        case '=': return Token(TokenType::Equal, "=");
        case '>': return Token(TokenType::Greater, ">");
        case '<': return Token(TokenType::Less, "<");
        case '(': return Token(TokenType::LParen, "(");
        case ')': return Token(TokenType::RParen, ")");
        case '{': return Token(TokenType::LBrace, "{");
        case '}': return Token(TokenType::RBrace, "}");
        case ';': return Token(TokenType::Semicolon, ";");
        default: return Token(TokenType::Unknown, std::string(1, c));
    }
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (true) {
        skip_whitespace_and_comments();
        char c = current();
        if (c == '\0') {
            tokens.emplace_back(TokenType::EndOfFile, "");
            break;
        }
        if (std::isdigit(static_cast<unsigned char>(c))) {
            tokens.push_back(read_number());
        } else if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            tokens.push_back(read_identifier_or_keyword());
        } else {
            tokens.push_back(read_single_char_token());
        }
    }
    return tokens;
}

