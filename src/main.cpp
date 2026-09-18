// DnxLanguage reads a .dnx source file, translates it into C++ code,
// compiles the generated code with g++, and runs the resulting program.
// Temporary files are removed automatically after execution unless the
// local-folder option is enabled.
//
// This project was written and researched by a human developer with the
// help of AI-assisted guidance during development.
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "Lexer.h"
#include "Parser.h"
#include "CodeGen.h"
#include "CodeRunner.h"
#include "TempHandler.h"
#include "ConsoleHelper.h"
#include "AppFlags.h"

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

    // Receive arguments
    if (argc < 2) {
        console::to_console_error("Usage: ", argv[0], " <filename.dnx>", " <flags>\n");
        //std::cerr << "Usage: " << argv[0] << " <filename.dnx>\n";
        return 1;
    }

    if (argc == 3){
        std::string optional_arg = argv[2];
        if (optional_arg == "-l") {
            appflags.use_local_folder = 1;
        } else if (optional_arg == "-s"){
            appflags.log_sequence = 1;
        } else if (optional_arg == "-ls"){
            appflags.use_local_folder = 1;
            appflags.log_sequence = 1;
        } 
        else {
            console::to_console_error("This is not a valid optional argument");
            return 1;
        }
    }

    std::string dnxfile_path = argv[1];

    TempHandler temphandler;
    std::string trans_path = "dnxgeneratedcode.cpp";
    std::filesystem::path temp_folder_path = temphandler.create_temp_folder(appflags.use_local_folder);

    try {
        // try find .dnx file
        std::string source = read_file(dnxfile_path);

        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        Parser parser(tokens);
        translated_program program = parser.parse();

        CodeGen codegen;
        std::string cpp = codegen.generate(program); // This returns a string

        // Join the temp folder path with the .cpp path
        std::filesystem::path temp_trans_file_path = temp_folder_path / trans_path;

        std::ofstream output(temp_trans_file_path);
        if (!output) {
            console::to_console_error("Cannot write to ", temp_trans_file_path.c_str(), "");
            return 1;
        }
        output << cpp;
        output.close(); // Close file before try to run it

        console::to_console("Generated ", trans_path.c_str(), "\n");

        // Try run program using g++ and check for local flag
        CodeRunner coderun;
        if (coderun.run_program(temp_folder_path ,temp_trans_file_path) == 0 &&
    appflags.use_local_folder == 0) {
            temphandler.delete_temp_folder(temp_folder_path);
        }

    } catch (const std::exception& error) {
        console::to_console_error("Main program:\n");
        std::cerr << error.what() << "\n";

        // try to delete the temp folder if the program just fails for some reason
        if (std::filesystem::exists(temp_folder_path) == true) {
            temphandler.delete_temp_folder(temp_folder_path);
        }
        return 1;
    }

    return 0;
}
