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
std::string CodeGen::generate(const translated_program& prog) {
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

void CodeGen::gen_stmt(const ast_statement& stmt) {
    if (auto v = dynamic_cast<const int_declaration*>(&stmt)) { 
        emit_indent();
        emit("int ");
        emit(v->name);
        emit(" = ");
        gen_expr(*v->init);
        emit(";\n");
    } else if (auto a = dynamic_cast<const assignment*>(&stmt)) {
        emit_indent();
        emit(a->name);
        emit(" = ");
        gen_expr(*a->value);
        emit(";\n");
    } else if (auto i = dynamic_cast<const if_statement*>(&stmt)) {
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
    } else if (auto w = dynamic_cast<const while_statement*>(&stmt)) {
        emit_indent();
        emit("while (");
        gen_expr(*w->condition);
        emit(") {\n");
        increase_indent();
        for (auto& s : w->body) gen_stmt(*s);
        decrease_indent();
        emit_indent();
        emit("}\n");
    } else if (auto p = dynamic_cast<const print_statement*>(&stmt)) {
        emit_indent();
        emit("std::cout << ");
        gen_expr(*p->expression);
        emit(" << std::endl;\n");
    } else {
        // Unknown statement, just ignore
    }
}

void CodeGen::gen_expr(const ast_expression& expr) {
    if (auto i = dynamic_cast<const int_literal*>(&expr)) {
        emit(std::to_string(i->value));
    } else if (auto id = dynamic_cast<const identifier_expression*>(&expr)) {
        emit(id->name);
    } else if (auto b = dynamic_cast<const binary_expression*>(&expr)) {
        emit("(");
        gen_expr(*b->left);
        switch (b->op) {
            case binary_expression::Add: emit(" + "); break;
            case binary_expression::Sub: emit(" - "); break;
            case binary_expression::Mul: emit(" * "); break;
            case binary_expression::Div: emit(" / "); break;
            case binary_expression::Gt:  emit(" > "); break;
            case binary_expression::Lt:  emit(" < "); break;
        }
        gen_expr(*b->right);
        emit(")");
    } else {
        emit("0"); // fallback
    }
}