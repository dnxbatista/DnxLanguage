#include "CodeRunner.h"
#include <iostream>
#include <cstdlib>
#include "ConsoleHelper.h"

int CodeRunner::run_program(std::filesystem::path temp_folder_path ,std::filesystem::path trans_file_path){
    std::string exe_path = "output_program";

    console::to_console("Trying to run program\n");
    std::filesystem::path temp_exe_path = temp_folder_path / exe_path;
    std::string compile_cmd = "g++ " + trans_file_path.string() + " -o " + temp_exe_path.string();

    int compile_status = std::system(compile_cmd.c_str());

    if (compile_status != 0) {
        console::to_console_error("Error: C++ compilation failed.");
        return 1;
    }

    console::to_console_sucess("Running program...\n");
    console::to_console("-----------------------------------------\n");

    std::string run_command = temp_folder_path.string() + ".\\output_program.exe"; // This is not a crossplataform thing but, yeah, #windows forever

    int run_status = std::system(run_command.c_str());

    std::cout << "\n"; // jumps one line
    console::to_console("-----------------------------------------\n");
    console::to_console_force("Program exited with status: ", run_status, "\n");
    return 0;
}