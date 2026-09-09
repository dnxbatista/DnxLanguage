#include "CodeGen.h"
#include <iostream>

void CodeGen::emit(const std::string& s) {
    out << s;
}

void CodeGen::emit_indent() {
    for (int i = 0; i < indentLevel; ++i) emit("    ");
}

void CodeGen::increase_indent() { ++indentLevel; }
void CodeGen::decrease_indent() { --indentLevel; }

// This is reponsible for generating the .cpp file from the .dnx (as a string)
std::string CodeGen::generate(const Program& prog) {
    out.str("");
    indentLevel = 0;

    emit("// this code is from a .dnx file\n");
    emit("#include <iostream>\n");
    emit("int main() {\n");
    increase_indent();

    for (auto& stmt : prog.statements) {
        gen_stmt(*stmt);
    }

    emit("    return 0;\n");
    decrease_indent();
    emit("}\n");

    return out.str();
}

void CodeGen::gen_stmt(const ASTStmt& stmt) {
    if (auto v = dynamic_cast<const VarDecl*>(&stmt)) { // THIS ONLY WORK WITH INT
        emit_indent();
        emit("int ");
        emit(v->name);
        emit(" = ");
        gen_expr(*v->init);
        emit(";\n");
    } else if (auto a = dynamic_cast<const Assignment*>(&stmt)) {
        emit_indent();
        emit(a->name);
        emit(" = ");
        gen_expr(*a->value);
        emit(";\n");
    } else if (auto i = dynamic_cast<const IfStmt*>(&stmt)) {
        emit_indent();
        emit("if (");
        gen_expr(*i->condition);
        emit(") {\n");
        increase_indent();
        for (auto& s : i->thenBody) gen_stmt(*s);
        decrease_indent();
        emit_indent();
        emit("}");
        if (!i->elseBody.empty()) {
            emit(" else {\n");
            increase_indent();
            for (auto& s : i->elseBody) gen_stmt(*s);
            decrease_indent();
            emit_indent();
            emit("}");
        }
        emit("\n");
    } else if (auto w = dynamic_cast<const WhileStmt*>(&stmt)) {
        emit_indent();
        emit("while (");
        gen_expr(*w->condition);
        emit(") {\n");
        increase_indent();
        for (auto& s : w->body) gen_stmt(*s);
        decrease_indent();
        emit_indent();
        emit("}\n");
    } else if (auto p = dynamic_cast<const PrintStmt*>(&stmt)) {
        emit_indent();
        emit("std::cout << ");
        gen_expr(*p->expression);
        emit(" << std::endl;\n");
    } else {
        // Unknown statement, just ignore
    }
}

void CodeGen::gen_expr(const ASTExpr& expr) {
    if (auto i = dynamic_cast<const IntLiteral*>(&expr)) {
        emit(std::to_string(i->value));
    } else if (auto id = dynamic_cast<const IdentifierExpr*>(&expr)) {
        emit(id->name);
    } else if (auto b = dynamic_cast<const BinaryExpr*>(&expr)) {
        emit("(");
        gen_expr(*b->left);
        switch (b->op) {
            case BinaryExpr::Add: emit(" + "); break;
            case BinaryExpr::Sub: emit(" - "); break;
            case BinaryExpr::Mul: emit(" * "); break;
            case BinaryExpr::Div: emit(" / "); break;
            case BinaryExpr::Gt:  emit(" > "); break;
            case BinaryExpr::Lt:  emit(" < "); break;
        }
        gen_expr(*b->right);
        emit(")");
    } else {
        emit("0"); // fallback
    }
}