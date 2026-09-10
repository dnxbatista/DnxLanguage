#include "Lexer.h"
#include <cctype>
#include <unordered_map>
#include <stdexcept>

static const std::unordered_map<std::string, token_type> keywords = {
    {"int", token_type::Int},
    {"string", token_type::String},
    {"if", token_type::If},
    {"else", token_type::Else},
    {"while", token_type::While},
    {"print", token_type::Print},
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

void Lexer::skip_whitespace_and_comments() {
    while (true) {
        char c = current();
        if (std::isspace(static_cast<unsigned char>(c))) {
        advance();
        continue;
        }

        if (c == '#') {
            while (current() != '\0' && current() != '\n') {
                advance();
            }
            continue;
        }
        break;
    }
}

// -> Read variable types 
token Lexer::read_number() {
    std::string number;
    while (std::isdigit(static_cast<unsigned char>(current()))) {
        number += current();
        advance();
    }
    return token(token_type::IntLiteral, number, number);
}

token Lexer::read_string()
{
    advance(); // first quote

    std::string value;
    while (current() != '"' && current() != '\0'){
        value += current();
        advance();
    }

    if (current() != '"')
    {
        throw std::runtime_error("String not finished");
    }

    advance();
    return token(token_type::StringLiteral, value, value);
}

token Lexer::read_identifier_or_keyword() {
    std::string id;
    while (std::isalnum(static_cast<unsigned char>(current())) || current() == '_') {
        id += current();
        advance();
    }
    auto it = keywords.find(id);
    if (it != keywords.end()) {
        return token(it->second, id, id);
    }
    return token(token_type::Identifier, id, id);
}

token Lexer::read_single_char_token() {
    char c = current();
    advance();
    switch (c) {
        case '+': return token(token_type::Plus, "+");
        case '-': return token(token_type::Minus, "-");
        case '*': return token(token_type::Star, "*");
        case '/': return token(token_type::Slash, "/");
        case '=': return token(token_type::Equal, "=");
        case '>': return token(token_type::Greater, ">");
        case '<': return token(token_type::Less, "<");
        case '(': return token(token_type::LParen, "(");
        case ')': return token(token_type::RParen, ")");
        case '{': return token(token_type::LBrace, "{");
        case '}': return token(token_type::RBrace, "}");
        case ';': return token(token_type::Semicolon, ";");
        default: return token(token_type::Unknown, std::string(1, c));
    }
}

std::vector<token> Lexer::tokenize() {
    std::vector<token> tokens;
    while (true) {
        skip_whitespace_and_comments();
        char c = current();
        if (c == '\0') {
            tokens.emplace_back(token_type::EndOfFile, "");
            break;
        }
        if (std::isdigit(static_cast<unsigned char>(c))) {
            tokens.push_back(read_number());
        } else if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            tokens.push_back(read_identifier_or_keyword());
        } else if(c == '"'){
            tokens.push_back(read_string());
        } else {
            tokens.push_back(read_single_char_token());
        }
    }
    return tokens;
}

