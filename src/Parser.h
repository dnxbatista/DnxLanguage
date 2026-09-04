#ifndef DNXLANGUAGE_PARSER_H
#define DNXLANGUAGE_PARSER_H
#include "Token.h"
#include "AST.h"
#include <vector>
#include <memory>
#include <stdexcept>

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    Program parse();

private:
    const std::vector<Token>& tokens;
    size_t pos;

    const Token& current() const;
    void advance();
    void expect(TokenType type, const std::string& message);

    std::unique_ptr<ASTStmt> parseStatement();
    std::unique_ptr<VarDecl> parseVarDecl();
    std::unique_ptr<Assignment> parseAssignment();
    std::unique_ptr<IfStmt> parseIfStmt();
    std::unique_ptr<WhileStmt> parseWhileStmt();
    std::unique_ptr<PrintStmt> parsePrintStmt();
    std::vector<std::unique_ptr<ASTStmt>> parseBlock();

    std::unique_ptr<ASTExpr> parseExpression();
    std::unique_ptr<ASTExpr> parseComparison();
    std::unique_ptr<ASTExpr> parseAddition();
    std::unique_ptr<ASTExpr> parseMultiplication();
    std::unique_ptr<ASTExpr> parsePrimary();

    [[noreturn]] void error(const std::string& msg);
};

#endif //DNXLANGUAGE_PARSER_H
