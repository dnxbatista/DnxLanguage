#ifndef DNXLANGUAGE_AST_H
#define DNXLANGUAGE_AST_H
#include <string>
#include <vector>
#include <memory>

// -> Expressions
struct ASTExpr {
    virtual ~ASTExpr() = default;
};

struct IntLiteral : ASTExpr {
    int value;
    explicit IntLiteral(int v) : value(v) {}
};

struct IdentifierExpr : ASTExpr {
    std::string name;
    explicit IdentifierExpr(const std::string& n) : name(n) {}
};

struct BinaryExpr : ASTExpr {
    enum Op { Add, Sub, Mul, Div, Gt, Lt};
    Op op;
    std::unique_ptr<ASTExpr> left;
    std::unique_ptr<ASTExpr> right;

    BinaryExpr(Op o, std::unique_ptr<ASTExpr> l, std::unique_ptr<ASTExpr> r) :
    op(o), left(std::move(l)), right(std::move(r)) {}
};

// -> Statements
struct ASTStmt {
    virtual ~ASTStmt() = default;
};

struct VarDecl : ASTStmt {
    std::string name;
    std::unique_ptr<ASTExpr> init;
    VarDecl(const std::string& n, std::unique_ptr<ASTExpr> i) : name(n), init(std::move(i)) {}
};

struct Assignment : ASTStmt {
    std::string name;
    std::unique_ptr<ASTExpr> value;
    Assignment(const std::string& n, std::unique_ptr<ASTExpr> v) : name(n), value(std::move(v)) {}
};

struct IfStmt : ASTStmt {
    std::unique_ptr<ASTExpr> condition;
    std::vector<std::unique_ptr<ASTStmt>> thenBody;
    std::vector<std::unique_ptr<ASTStmt>> elseBody;

    IfStmt(std::unique_ptr<ASTExpr> c,
        std::vector<std::unique_ptr<ASTStmt>> t,
        std::vector<std::unique_ptr<ASTStmt>> e)
            : condition(std::move(c)), thenBody(std::move(t)), elseBody(std::move(e)) {}
};

struct WhileStmt : ASTStmt {
    std::unique_ptr<ASTExpr> condition;
    std::vector<std::unique_ptr<ASTStmt>> body;

    WhileStmt(std::unique_ptr<ASTExpr> c,
        std::vector<std::unique_ptr<ASTStmt>> b)
            : condition(std::move(c)), body(std::move(b)) {}
};

struct PrintStmt : ASTStmt {
    std::unique_ptr<ASTExpr> expression;
    explicit PrintStmt(std::unique_ptr<ASTExpr> e) : expression(std::move(e)) {}
};

struct Program {
    std::vector<std::unique_ptr<ASTStmt>> statements;
};

#endif //DNXLANGUAGE_AST_H
