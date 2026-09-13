// Check the parseVarDecl to a very basic explanation

#include "Parser.h"
#include <iostream>

Parser::Parser(const std::vector<token>& tokens) : tokens(tokens), pos(0) {}

const token& Parser::current() const {
    return tokens[pos];
}

void Parser::advance() {
    if (pos < tokens.size() - 1) ++pos;
}

void Parser::expect(token_type t, const std::string& msg) {
    if (current().type != t) {
        error(msg + " (got " + current().text + ")");
    }
    advance();
}

[[noreturn]] void Parser::error(const std::string& msg) {
    throw std::runtime_error("Parse error: " + msg);
}

translated_program Parser::parse() {
    translated_program prog;
    while (current().type != token_type::EndOfFile) {
        prog.statements.push_back(parse_statement());
    }
    return prog;
}

// This check what we are reading at the moment and return the correct statement
std::unique_ptr<ast_statement> Parser::parse_statement() {
    switch (current().type) {
        case token_type::Int:
            return parse_int_declaration();
        case token_type::String:
            return parse_string_declaration();
        case token_type::If:
            return parse_if_statement();
        case token_type::While:
            return parse_while_statement();
        case token_type::Print:
            return parse_print_statement();
        case token_type::Identifier:
            return parse_assignment();
        default:
            error("Unexpected token in statement");
    }
}

/*
What we doing here, is skiping the int token, then go to the next token
find and erros in the way, and then declare what type of variable is it
*/
std::unique_ptr<int_declaration> Parser::parse_int_declaration() {
    advance(); // consume 'int'
    if (current().type != token_type::Identifier) {
        error("Expected identifier after 'int'");
    }
    std::string name = current().value;
    advance();

    expect(token_type::Equal, "Expected '=' in variable declaration");
    auto expr = parse_expression();
    expect(token_type::Semicolon, "Expected ';' after variable declaration");

    return std::make_unique<int_declaration>(name, std::move(expr));
}

std::unique_ptr<string_declaration> Parser::parse_string_declaration() {
    advance();
    if (current().type != token_type::Identifier) {
        error("Expected identifier after 'string'");
    }
    std::string name = current().value;
    advance();

    expect(token_type::Equal, "Expected '=' in variable declaration");
    auto expr = parse_expression();
    expect(token_type::Semicolon, "Expected ';' after variable declaration");

    return std::make_unique<string_declaration>(name, std::move(expr));
}

std::unique_ptr<assignment> Parser::parse_assignment() {
    std::string name = current().value;
    advance();
    expect(token_type::Equal, "Expected '=' in assignment");
    auto expr = parse_expression();
    expect(token_type::Semicolon, "Expected ';' after assignment");
    return std::make_unique<assignment>(name, std::move(expr));
}

std::unique_ptr<if_statement> Parser::parse_if_statement() {
    advance(); // 'if'
    expect(token_type::LParen, "Expected '(' after 'if'");
    auto cond = parse_expression();
    expect(token_type::RParen, "Expected ')' after condition");
    auto thenBody = parse_block();

    std::vector<std::unique_ptr<ast_statement>> elseBody;
    if (current().type == token_type::Else) {
        advance();
        elseBody = parse_block();
    }

    return std::make_unique<if_statement>(std::move(cond), std::move(thenBody), std::move(elseBody));
}

std::unique_ptr<while_statement> Parser::parse_while_statement() {
    advance(); // 'while'
    expect(token_type::LParen, "Expected '(' after 'while'");
    auto cond = parse_expression();
    expect(token_type::RParen, "Expected ')' after condition");
    auto body = parse_block();
    return std::make_unique<while_statement>(std::move(cond), std::move(body));
}

std::unique_ptr<print_statement> Parser::parse_print_statement() {
    advance(); // 'print'
    auto expr = parse_expression();
    expect(token_type::Semicolon, "Expected ';' after print");
    return std::make_unique<print_statement>(std::move(expr));
}

std::vector<std::unique_ptr<ast_statement>> Parser::parse_block() {
    expect(token_type::LBrace, "Expected '{'");
    std::vector<std::unique_ptr<ast_statement>> stmts;
    while (current().type != token_type::RBrace && current().type != token_type::EndOfFile) {
        stmts.push_back(parse_statement());
    }
    expect(token_type::RBrace, "Expected '}'");
    return stmts;
}

// Expressions

std::unique_ptr<ast_expression> Parser::parse_expression() {
    return parse_comparison();
}

std::unique_ptr<ast_expression> Parser::parse_comparison() {
    auto left = parse_addition();
    while (current().type == token_type::Greater || current().type == token_type::Less) {
        binary_expression::Op op = (current().type == token_type::Greater)
                            ? binary_expression::Op::Gt
                            : binary_expression::Op::Lt;
        advance();
        auto right = parse_addition();
        left = std::make_unique<binary_expression>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ast_expression> Parser::parse_addition() {
    auto left = parse_multiplication();
    while (current().type == token_type::Plus || current().type == token_type::Minus) {
        binary_expression::Op op = (current().type == token_type::Plus)
                            ? binary_expression::Op::Add
                            : binary_expression::Op::Sub;
        advance();
        auto right = parse_multiplication();
        left = std::make_unique<binary_expression>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ast_expression> Parser::parse_multiplication() {
    auto left = parse_primary();
    while (current().type == token_type::Star || current().type == token_type::Slash) {
        binary_expression::Op op = (current().type == token_type::Star)
                            ? binary_expression::Op::Mul
                            : binary_expression::Op::Div;
        advance();
        auto right = parse_primary();
        left = std::make_unique<binary_expression>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ast_expression> Parser::parse_primary() {
    if (current().type == token_type::IntLiteral) {
        int val = std::stoi(current().value);
        advance();
        return std::make_unique<int_literal>(val);
    }
    if (current().type == token_type::StringLiteral)
    {
        std::string value = current().value;
        advance();
        return std::make_unique<string_literal>(value);
    }
    if (current().type == token_type::Identifier) {
        std::string name = current().value;
        advance();
        return std::make_unique<identifier_expression>(name);
    }
    if (current().type == token_type::LParen) {
        advance();
        auto expr = parse_expression();
        expect(token_type::RParen, "Expected ')'");
        return expr;
    }
    error("Unexpected token in expression");
}