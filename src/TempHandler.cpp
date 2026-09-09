#include "TempHandler.h"
#include <iostream>
#include <random>
#include <sstream>

std::filesystem::path TempHandler::create_temp_folder(){
    // Get the temp folder dir
    std::filesystem::path temp_base_path = std::filesystem::temp_directory_path();

    // Random number generator
    std::random_device rd;
    std::mt19937_64 generator(rd()); //Mersenne Twister algorithm 64 bit
    std::uniform_int_distribution<uint64_t> distribution;

    std::filesystem::path unique_path;
    
    // Check if path already exists in temp dir
    do {
        std::stringstream ss;
        ss << "dnxtemp_" << std::hex << distribution(generator);
        unique_path = temp_base_path / ss.str();
    } while (std::filesystem::exists(unique_path));

    // Create directory
    if(std::filesystem::create_directory(unique_path)) {
        std::cout << "Temp folder created sucessfully\n";
    } else {
        std::cout << "Error creating temp folder\n";
        std::filesystem::path empty_path = "";
        return empty_path;
    }

    return unique_path;
}

int TempHandler::check_if_temp_exists(std::filesystem::path temp_folder_path){
    if (!std::filesystem::exists(temp_folder_path)) {
        return 1;
    }
    return 0;
}

int TempHandler::delete_temp_folder(std::filesystem::path temp_folder_path)
{
    if (!std::filesystem::exists(temp_folder_path)) {
        std::cerr << "Temp folder does not exists in currently directory\n";
        return 1;
    }

    try {
        // Delete folder and files inside
        std::uintmax_t deleted_items = std::filesystem::remove_all(temp_folder_path);
        std::cout << "Deleted folder with: " << deleted_items << "\n";
        return 0;  
    } catch (const std::filesystem::filesystem_error& error)
    {
        std::cerr << "File System Error: " << error.what() << "\n";
        return 1;
    }

    return 0;
}