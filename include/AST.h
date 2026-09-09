// AST means Abstract Syntax Tree.
// What does this do? well if i gonna be honest, i dont really know (even tough i writed all of this)
// what i know is that you create expressions and statements here, like variables types, funcions, ...

#ifndef DNXLANGUAGE_AST_H
#define DNXLANGUAGE_AST_H
#include <string>
#include <vector>
#include <memory>

// -> Expressions
struct ast_expression {
    virtual ~ast_expression() = default;
};

struct int_literal : ast_expression {
    int value;
    explicit int_literal(int v) : value(v) {}
};

struct identifier_expression : ast_expression {
    std::string name;
    explicit identifier_expression(const std::string& n) : name(n) {}
};

struct binary_expression : ast_expression {
    enum Op { Add, Sub, Mul, Div, Gt, Lt};
    Op op;
    std::unique_ptr<ast_expression> left;
    std::unique_ptr<ast_expression> right;

    binary_expression(Op o, std::unique_ptr<ast_expression> l, std::unique_ptr<ast_expression> r) :
    op(o), left(std::move(l)), right(std::move(r)) {}
};

// -> Statements
struct ast_statement {
    virtual ~ast_statement() = default;
};

struct int_declaration : ast_statement {
    std::string name;
    std::unique_ptr<ast_expression> init;
    int_declaration(const std::string& n, std::unique_ptr<ast_expression> i) : name(n), init(std::move(i)) {}
};

struct assignment : ast_statement {
    std::string name;
    std::unique_ptr<ast_expression> value;
    assignment(const std::string& n, std::unique_ptr<ast_expression> v) : name(n), value(std::move(v)) {}
};

struct if_statement : ast_statement {
    std::unique_ptr<ast_expression> condition;
    std::vector<std::unique_ptr<ast_statement>> thenBody;
    std::vector<std::unique_ptr<ast_statement>> elseBody;

    if_statement(std::unique_ptr<ast_expression> c,
        std::vector<std::unique_ptr<ast_statement>> t,
        std::vector<std::unique_ptr<ast_statement>> e)
            : condition(std::move(c)), thenBody(std::move(t)), elseBody(std::move(e)) {}
};

struct while_statement : ast_statement {
    std::unique_ptr<ast_expression> condition;
    std::vector<std::unique_ptr<ast_statement>> body;

    while_statement(std::unique_ptr<ast_expression> c,
        std::vector<std::unique_ptr<ast_statement>> b)
            : condition(std::move(c)), body(std::move(b)) {}
};

struct print_statement : ast_statement {
    std::unique_ptr<ast_expression> expression;
    explicit print_statement(std::unique_ptr<ast_expression> e) : expression(std::move(e)) {}
};

struct translated_program {
    std::vector<std::unique_ptr<ast_statement>> statements;
};

#endif //DNXLANGUAGE_AST_H
