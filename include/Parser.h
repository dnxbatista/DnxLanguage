
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

    std::unique_ptr<ast_statement> parse_statement();
    std::unique_ptr<int_declaration> parse_int_declaration();
    std::unique_ptr<string_declaration> parse_string_declaration();
    std::unique_ptr<assignment> parse_assignment();
    std::unique_ptr<if_statement> parse_if_statement();
    std::unique_ptr<while_statement> parse_while_statement();
    std::unique_ptr<print_statement> parse_print_statement();
    std::vector<std::unique_ptr<ast_statement>> parse_block();

    std::unique_ptr<ast_expression> parse_expression();
    std::unique_ptr<ast_expression> parse_comparison();
    std::unique_ptr<ast_expression> parse_addition();
    std::unique_ptr<ast_expression> parse_multiplication();
    std::unique_ptr<ast_expression> parse_primary();

    [[noreturn]] void error(const std::string& msg);
};

#endif //DNXLANGUAGE_PARSER_H
