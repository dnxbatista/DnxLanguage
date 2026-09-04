#include "Parser.h"
#include <iostream>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0) {}

const Token& Parser::current() const {
    return tokens[pos];
}

void Parser::advance() {
    if (pos < tokens.size() - 1) ++pos;
}

void Parser::expect(TokenType t, const std::string& msg) {
    if (current().type != t) {
        error(msg + " (got " + current().text + ")");
    }
    advance();
}

[[noreturn]] void Parser::error(const std::string& msg) {
    throw std::runtime_error("Parse error: " + msg);
}

Program Parser::parse() {
    Program prog;
    while (current().type != TokenType::EndOfFile) {
        prog.statements.push_back(parseStatement());
    }
    return prog;
}

std::unique_ptr<ASTStmt> Parser::parseStatement() {
    switch (current().type) {
        case TokenType::Var:
            return parseVarDecl();
        case TokenType::If:
            return parseIfStmt();
        case TokenType::While:
            return parseWhileStmt();
        case TokenType::Print:
            return parsePrintStmt();
        case TokenType::Identifier:
            return parseAssignment();
        default:
            error("Unexpected token in statement");
    }
}

std::unique_ptr<VarDecl> Parser::parseVarDecl() {
    advance(); // consume 'var'
    if (current().type != TokenType::Identifier) {
        error("Expected identifier after 'var'");
    }
    std::string name = current().value;
    advance();

    expect(TokenType::Equal, "Expected '=' in variable declaration");
    auto expr = parseExpression();
    expect(TokenType::Semicolon, "Expected ';' after variable declaration");

    return std::make_unique<VarDecl>(name, std::move(expr));
}

std::unique_ptr<Assignment> Parser::parseAssignment() {
    std::string name = current().value;
    advance();
    expect(TokenType::Equal, "Expected '=' in assignment");
    auto expr = parseExpression();
    expect(TokenType::Semicolon, "Expected ';' after assignment");
    return std::make_unique<Assignment>(name, std::move(expr));
}

std::unique_ptr<IfStmt> Parser::parseIfStmt() {
    advance(); // 'if'
    expect(TokenType::LParen, "Expected '(' after 'if'");
    auto cond = parseExpression();
    expect(TokenType::RParen, "Expected ')' after condition");
    auto thenBody = parseBlock();

    std::vector<std::unique_ptr<ASTStmt>> elseBody;
    if (current().type == TokenType::Else) {
        advance();
        elseBody = parseBlock();
    }

    return std::make_unique<IfStmt>(std::move(cond), std::move(thenBody), std::move(elseBody));
}

std::unique_ptr<WhileStmt> Parser::parseWhileStmt() {
    advance(); // 'while'
    expect(TokenType::LParen, "Expected '(' after 'while'");
    auto cond = parseExpression();
    expect(TokenType::RParen, "Expected ')' after condition");
    auto body = parseBlock();
    return std::make_unique<WhileStmt>(std::move(cond), std::move(body));
}

std::unique_ptr<PrintStmt> Parser::parsePrintStmt() {
    advance(); // 'print'
    auto expr = parseExpression();
    expect(TokenType::Semicolon, "Expected ';' after print");
    return std::make_unique<PrintStmt>(std::move(expr));
}

std::vector<std::unique_ptr<ASTStmt>> Parser::parseBlock() {
    expect(TokenType::LBrace, "Expected '{'");
    std::vector<std::unique_ptr<ASTStmt>> stmts;
    while (current().type != TokenType::RBrace && current().type != TokenType::EndOfFile) {
        stmts.push_back(parseStatement());
    }
    expect(TokenType::RBrace, "Expected '}'");
    return stmts;
}

// Expressions

std::unique_ptr<ASTExpr> Parser::parseExpression() {
    return parseComparison();
}

std::unique_ptr<ASTExpr> Parser::parseComparison() {
    auto left = parseAddition();
    while (current().type == TokenType::Greater || current().type == TokenType::Less) {
        BinaryExpr::Op op = (current().type == TokenType::Greater)
                            ? BinaryExpr::Op::Gt
                            : BinaryExpr::Op::Lt;
        advance();
        auto right = parseAddition();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTExpr> Parser::parseAddition() {
    auto left = parseMultiplication();
    while (current().type == TokenType::Plus || current().type == TokenType::Minus) {
        BinaryExpr::Op op = (current().type == TokenType::Plus)
                            ? BinaryExpr::Op::Add
                            : BinaryExpr::Op::Sub;
        advance();
        auto right = parseMultiplication();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTExpr> Parser::parseMultiplication() {
    auto left = parsePrimary();
    while (current().type == TokenType::Star || current().type == TokenType::Slash) {
        BinaryExpr::Op op = (current().type == TokenType::Star)
                            ? BinaryExpr::Op::Mul
                            : BinaryExpr::Op::Div;
        advance();
        auto right = parsePrimary();
        left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<ASTExpr> Parser::parsePrimary() {
    if (current().type == TokenType::IntLint) {
        int val = std::stoi(current().value);
        advance();
        return std::make_unique<IntLiteral>(val);
    }
    if (current().type == TokenType::Identifier) {
        std::string name = current().value;
        advance();
        return std::make_unique<IdentifierExpr>(name);
    }
    if (current().type == TokenType::LParen) {
        advance();
        auto expr = parseExpression();
        expect(TokenType::RParen, "Expected ')'");
        return expr;
    }
    error("Unexpected token in expression");
}