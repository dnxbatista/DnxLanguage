// The way this works, is that you receive a .dnx file with the correct syntax
// and it return the .dnx translated to a .cpp file
// is quite "simple" (the codegen sucks btw)
// i used to AI help (not to write everysingle line) but to help me how to make it
// im not a expert but this gave me some understand about how progamming languages works
// and THIS IS NOT A VIBE CODED PROJECT, ALL OF THIS WAS WRITTEN AND RESEARCHED BY A HUMAN BEING.

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "Lexer.h"
#include "Parser.h"
#include "CodeGen.h"

std::string read_file(const std::string& raw_file_path) {
    std::fstream file(raw_file_path);
    std::filesystem::path file_path = raw_file_path;

    //Check filepath
    if (file_path.extension() != ".dnx")
    {
        throw std::runtime_error("Cannot convert a non-dnx file");
    }

    if (!file) {
        throw std::runtime_error("Could not open file: " + raw_file_path);
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename.dnx>";
        return 1;
    }

    std::string dnxfile_path = argv[1];
    std::string output_path = "output.cpp";

    try {
        // try find .dnx file
        std::string source = read_file(dnxfile_path);

        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        Program program = parser.parse();

        CodeGen codegen;
        std::string cpp = codegen.generate(program);

        std::ofstream output(output_path);
        if (!output) {
            std::cerr << "Cannot write to " << output_path << "";
            return 1;
        }
        output << cpp;

        std::cout << "Generated " << output_path << "";
    } catch (const std::exception& e) {
        std::cerr << "Error " << e.what();
        return 1;
    }

    return 0;
}
