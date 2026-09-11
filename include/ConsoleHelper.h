// This is just for help me print things on console easier
#ifndef DNXLANGUAGE_CONSOLEHELPER_H
#define DNXLANGUAGE_CONSOLEHELPER_H
#include "AppFlags.h"
#include <sstream>
#include <string_view> // used for better compatibility -> thx google

namespace console{
    template <typename... Args> // yeah using templates here guys :C
    std::string concat(Args&&... args)
    {
        std::ostringstream oss;
        (oss << ... << args);
        return oss.str();
    }

    // Prints message to console
    template <typename... Args>
    void to_console(Args&&... args){ 
        if (appflags.log_sequence == 0) return; // Ignore if log sequence is false
        std::cout << concat(std::forward<Args>(args)...) << std::endl;
    }

    // Prints message to console (ignore log_sequence flag)
    template <typename... Args>
    void to_console_force(Args&&... args){
        std::cout << concat(std::forward<Args>(args)...) << std::endl;
    }

    // Prints message to console with red color (also uses cerr and ignore log_sequence)
    template <typename... Args>
    void to_console_error(Args&&... args){
        std::cerr << "\033[31m" << concat(std::forward<Args>(args)...) << "\033[0m" << std::endl;
    }

    // Prints message to console with yellow color (also uses cerr)
    template <typename... Args>
    void to_console_warning(Args&&... args){
        std::cout << "\033[33m" << concat(std::forward<Args>(args)...) << "\033[0m" << std::endl;
    }

    // Prints message to console with green color
    template <typename... Args>
    void to_console_sucess(Args&&... args){
        if (appflags.log_sequence == 0) return; 
        std::cout << "\033[32m" << concat(std::forward<Args>(args)...) << "\033[0m" << std::endl;
    }
}

#endif //DNXLANGUAGE_CONSOLEHELPER_H