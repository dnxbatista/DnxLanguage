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

    emit("//The following code was translated from a .dnx file.\n");
    emit("#include <iostream>\n");
    emit("#include <string>\n");
    emit("int main() {\n");
    increase_indent();

    for (auto& stmt : prog.statements) {
        generate_statement(*stmt);
    }

    emit("    return 0;\n");
    decrease_indent();
    emit("}\n");

    return out.str();
}

void CodeGen::generate_statement(const ast_statement& statement) {
    if (auto v = dynamic_cast<const int_declaration*>(&statement)) { 
        emit_indent();
        emit("int ");
        emit(v->name);
        emit(" = ");
        generate_expression(*v->init);
        emit(";\n");
    } else if (auto v = dynamic_cast<const string_declaration*>(&statement)) {
        emit_indent();
        emit("std::string ");
        emit(v->name);
        emit(" = ");
        generate_expression(*v->init);
        emit(";\n");
    } else if (auto a = dynamic_cast<const assignment*>(&statement)) {
        emit_indent();
        emit(a->name);
        emit(" = ");
        generate_expression(*a->value);
        emit(";\n");
    } else if (auto i = dynamic_cast<const if_statement*>(&statement)) {
        emit_indent();
        emit("if (");
        generate_expression(*i->condition);
        emit(") {\n");
        increase_indent();
        for (auto& s : i->thenBody) generate_statement(*s);
        decrease_indent();
        emit_indent();
        emit("}");
        if (!i->elseBody.empty()) {
            emit(" else {\n");
            increase_indent();
            for (auto& s : i->elseBody) generate_statement(*s);
            decrease_indent();
            emit_indent();
            emit("}");
        }
        emit("\n");
    } else if (auto w = dynamic_cast<const while_statement*>(&statement)) {
        emit_indent();
        emit("while (");
        generate_expression(*w->condition);
        emit(") {\n");
        increase_indent();
        for (auto& s : w->body) generate_statement(*s);
        decrease_indent();
        emit_indent();
        emit("}\n");
    } else if (auto p = dynamic_cast<const print_statement*>(&statement)) {
        emit_indent();
        emit("std::cout << ");
        generate_expression(*p->expression);
        emit(" << std::endl;\n");
    } else {
        // Unknown statement, just ignore
    }
}

void CodeGen::generate_expression(const ast_expression& expression) {
    if (auto i = dynamic_cast<const int_literal*>(&expression)) {
        emit(std::to_string(i->value));
    } else if (auto s = dynamic_cast<const string_literal*>(&expression)) {
        emit("\"" + s->value + "\"");
    } else if (auto id = dynamic_cast<const identifier_expression*>(&expression)) {
        emit(id->name);
    } else if (auto b = dynamic_cast<const binary_expression*>(&expression)) {
        emit("(");
        generate_expression(*b->left);
        switch (b->op) {
            case binary_expression::Add: emit(" + "); break;
            case binary_expression::Sub: emit(" - "); break;
            case binary_expression::Mul: emit(" * "); break;
            case binary_expression::Div: emit(" / "); break;
            case binary_expression::Gt:  emit(" > "); break;
            case binary_expression::Lt:  emit(" < "); break;
        }
        generate_expression(*b->right);
        emit(")");
    } else {
        emit("0"); // fallback
    }
}