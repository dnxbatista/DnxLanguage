#ifndef DNXLANGUANGE_TEMPHANDLER_H
#define DNXLANGUANGE_TEMPHANDLER_H
#include <filesystem>

class TempHandler
{
public:
    std::filesystem::path create_temp_folder(int local_folder_flag);
    int check_if_temp_exists(std::filesystem::path temp_folder_path);
    int delete_temp_folder(std::filesystem::path temp_folder_path);
};


#endif