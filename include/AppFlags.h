#ifndef DNXLANGUAGE_APPFLAGS_H
#define DNXLANGUAGE_APPFLAGS_H

class AppFlags
{
public:
    int use_local_folder = 0; // Use the local folder to compile the program
    int log_sequence = 0; // Enable showing in the console the program steps
};

inline AppFlags appflags;

// "inline" just tells that the appflags can have multiple definitions and should merge all into one
// i think

#endif // DNXLANGUAGE_APPFLAGS_H