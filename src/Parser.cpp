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
        prog.statements.push_back(parseStatement());
    }
    return prog;
}

// This check what we are reading at the moment and return the correct statement
std::unique_ptr<ast_statement> Parser::parseStatement() {
    switch (current().type) {
        case token_type::Int:
            return parseVarDecl();
        case token_type::If:
            return parseIfStmt();
        case token_type::While:
            return parseWhileStmt();
        case token_type::Print:
            return parsePrintStmt();
        case token_type::Identifier:
            return parseAssignment();
        default:
            error("Unexpected token in statement");
    }
}

/*
What we doing here, is skiping the var token, then go to the next token
find and erros in the way, and then declare what type of variable is it
*/
std::unique_ptr<int_declaration> Parser::parseVarDecl() {
    advance(); // consume 'var'
    if (current().type != token_type::Identifier) {
        error("Expected identifier after 'var'");
    }
    std::string name = current().value;
    advance();

    expect(token_type::Equal, "Expected '=' in variable declaration");
    auto expr = parseExpression();
    expect(token_type::Semicolon, "Expected ';' after variable declaration");

    return std::make_unique<int_declaration>(name, std::move(expr));
}

std::unique_ptr<assignment> Parser::parseAssignment() {
    std::string name = current().value;
    advance();
    expect(token_type::Equal, "Expected '=' in assignment");
    auto expr = parseExpression();
    expect(token_type::Semicolon, "Expected ';' after assignment");
    return std::make_unique<assignment>(name, std::move(expr));
}

std::unique_ptr<if_statement> Parser::parseIfStmt() {
    advance(); // 'if'
    expect(token_type::LParen, "Expected '(' after 'if'");
    auto cond = parseExpression();
    expect(token_type::RParen, "Expected ')' after condition");
    auto thenBody = parseBlock();

    std::vector<std::unique_ptr<ast_statement>> elseBody;
    if (current().type == token_type::Else) {
        advance();
        elseBody = parseBlock();
    }

    return std::make_unique<if_statement>(std::move(cond), std::move(thenBody), std::move(elseBody));
}

std::unique_ptr<while_statement> Parser::parseWhileStmt() {
    advance(); // 'while'
    expect(token_type::LParen, "Expected '(' after 'while'");
    auto cond = parseExpression();
    expect(token_type::RParen, "Expected ')' after condition");
    auto body = parseBlock();
    return std::make_unique<while_statement>(std::move(cond), std::move(body));
}

std::unique_ptr<print_statement> Parser::parsePrintStmt() {
    advance(); // 'print'
    auto expr = parseExpression();
    expect(token_type::Semicolon, "Expected ';' after print");
    return std::make_unique<print_statement>(std::move(expr));
}

std::vector<std::unique_ptr<ast_statement>> Parser::parseBlock() {
    expect(token_type::LBrace, "Expected '{'");
    std::vector<std::unique_ptr<ast_statement>> stmts;
    while (current().type != token_type::RBrace && current().type != token_type::EndOfFile) {
        stmts.push_back(parseStatement());
    }
    expect(token_type::RBrace, "Expected '}'");
    return stmts;
}

// Expressions

std::unique_ptr<ast_expression> Parser::parseExpression() {
    return parseComparison();
}

std::unique_ptr<ast_expression> Parser::parseComparison() {
    auto left = parseAddition();
    while (current().type == token_type::Greater || current().type == token_type::Less) {
        binary_expression::Op op = (current().type == token_type::Greater)
                            ? binary_expression::Op::Gt
                            : binary_expression::Op::Lt;
        advance();
        auto right = parseAddition();
        left = std::make_unique<binary_expression>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ast_expression> Parser::parseAddition() {
    auto left = parseMultiplication();
    while (current().type == token_type::Plus || current().type == token_type::Minus) {
        binary_expression::Op op = (current().type == token_type::Plus)
                            ? binary_expression::Op::Add
                            : binary_expression::Op::Sub;
        advance();
        auto right = parseMultiplication();
        left = std::make_unique<binary_expression>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ast_expression> Parser::parseMultiplication() {
    auto left = parsePrimary();
    while (current().type == token_type::Star || current().type == token_type::Slash) {
        binary_expression::Op op = (current().type == token_type::Star)
                            ? binary_expression::Op::Mul
                            : binary_expression::Op::Div;
        advance();
        auto right = parsePrimary();
        left = std::make_unique<binary_expression>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ast_expression> Parser::parsePrimary() {
    if (current().type == token_type::IntLint) {
        int val = std::stoi(current().value);
        advance();
        return std::make_unique<int_literal>(val);
    }
    if (current().type == token_type::Identifier) {
        std::string name = current().value;
        advance();
        return std::make_unique<identifier_expression>(name);
    }
    if (current().type == token_type::LParen) {
        advance();
        auto expr = parseExpression();
        expect(token_type::RParen, "Expected ')'");
        return expr;
    }
    error("Unexpected token in expression");
}