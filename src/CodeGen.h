#ifndef DNXLANGUAGE_CODEGEN_H
#define DNXLANGUAGE_CODEGEN_H
#include "AST.h"
#include <string>
#include <sstream>

class CodeGen {
public:
    std::string generate(const Program& prog);

private:
    std::ostringstream out;
    std::string indentStr;
    int indentLevel = 0;

    void emit(const std::string& s);
    void emit_indent();
    void increase_indent();
    void decrease_indent();

    void gen_stmt(const ASTStmt& stmt);
    void gen_expr(const ASTExpr& expr);
};
#endif //DNXLANGUAGE_CODEGEN_H
