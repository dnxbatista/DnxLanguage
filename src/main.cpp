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
#include "CodeRunner.h"
#include "TempHandler.h"

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
        std::cerr << "Usage: " << argv[0] << " <filename.dnx>\n";
        return 1;
    }

    std::string dnxfile_path = argv[1];

    std::string trans_path = "dnxgeneratedcode.cpp";

    try {
        // try find .dnx file
        std::string source = read_file(dnxfile_path);

        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        Program program = parser.parse();

        CodeGen codegen;
        std::string cpp = codegen.generate(program); // This returns a string
        
        // Generate temp folder
        TempHandler temphandler;
        std::filesystem::path temp_folder_path = temphandler.create_temp_folder();
        if (temphandler.check_if_temp_exists(temp_folder_path) != 0)
        {
            std::cerr << "Temp folder does not exists";
            return 1;
        }

        // Join the temp folder path with the .cpp path
        std::filesystem::path temp_trans_file_path = temp_folder_path / trans_path;

        std::ofstream output(temp_trans_file_path);
        if (!output) {
            std::cerr << "Cannot write to " << temp_trans_file_path.c_str() << "";
            return 1;
        }
        output << cpp;
        output.close(); // Close file before try to run it

        std::cout << "Generated " << trans_path.c_str() << "\n";

        // Try run program using g++
        CodeRunner coderun;
        if (coderun.run_program(temp_folder_path ,temp_trans_file_path) == 0) {
            temphandler.delete_temp_folder(temp_folder_path);
        }

    } catch (const std::exception& error) {
        std::cerr << "Error " << error.what();
        return 1;
    }

    return 0;
}
