// This turns the ASP into cpp code so you can run it (very basic explanation)

#ifndef DNXLANGUAGE_CODEGEN_H
#define DNXLANGUAGE_CODEGEN_H
#include "AST.h"
#include <string>
#include <sstream>

class CodeGen {
public:
    std::string generate(const translated_program& prog);

private:
    std::ostringstream out;
    std::string indentStr;
    int indentLevel = 0;

    void emit(const std::string& s);
    void emit_indent();
    void increase_indent();
    void decrease_indent();

    void gen_stmt(const ast_statement& stmt);
    void gen_expr(const ast_expression& expr);
};
#endif //DNXLANGUAGE_CODEGEN_H
