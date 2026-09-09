
// The parser get yours tokens and build the AST with the correct "grammar"
// check the .cpp file to better understand

#ifndef DNXLANGUAGE_PARSER_H
#define DNXLANGUAGE_PARSER_H
#include "Token.h"
#include "AST.h"
#include <vector>
#include <memory>
#include <stdexcept>

class Parser {
public:
    explicit Parser(const std::vector<token>& tokens);
    translated_program parse();

private:
    const std::vector<token>& tokens;
    size_t pos;

    const token& current() const;
    void advance();
    void expect(token_type type, const std::string& message);

    std::unique_ptr<ast_statement> parseStatement();
    std::unique_ptr<int_declaration> parseVarDecl();
    std::unique_ptr<assignment> parseAssignment();
    std::unique_ptr<if_statement> parseIfStmt();
    std::unique_ptr<while_statement> parseWhileStmt();
    std::unique_ptr<print_statement> parsePrintStmt();
    std::vector<std::unique_ptr<ast_statement>> parseBlock();

    std::unique_ptr<ast_expression> parseExpression();
    std::unique_ptr<ast_expression> parseComparison();
    std::unique_ptr<ast_expression> parseAddition();
    std::unique_ptr<ast_expression> parseMultiplication();
    std::unique_ptr<ast_expression> parsePrimary();

    [[noreturn]] void error(const std::string& msg);
};

#endif //DNXLANGUAGE_PARSER_H
