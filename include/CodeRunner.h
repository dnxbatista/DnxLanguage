// this is responsible for running the output file
// is just for organization
#ifndef DNXLANGUAGE_CODERUNNER_H
#define DNXLANGUAGE_CODERUNNER_H
#include <string>
#include <filesystem>

class CodeRunner{
public:
    int run_program(std::filesystem::path temp_folder_path ,std::filesystem::path trans_file_path);
};

#endif //DNXLANGUAGE_CODERUNNER_H