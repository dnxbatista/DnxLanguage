#include "CodeRunner.h"
#include <iostream>
#include <cstdlib>

int coderunner::run_program(std::string outpath){
    std::cout << "Trying run program\n";
    std::string compile_cmd = "g++ " + outpath + " -o output_program";

    int compile_status = std::system(compile_cmd.c_str());

    if (compile_status != 0) {
        std::cerr << "Error: C++ compilation failed.";
        return 1;
    }

    std::cout << "Running program...\n";
    std::cout << "-----------------------------------------\n";

    std::string run_command = ".\\output_program.exe";

    int run_status = std::system(run_command.c_str());

    std::cout << "-----------------------------------------\n";
    std::cout << "Program exited with status: " + run_status << "\n";
    return 0;
}