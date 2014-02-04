#include "Log.hpp"

bool Log::isDebugMode     = false;
bool Log::isVerboseMode   = false;

// Terminal scape codes.
//
// They are commands to the terminal
// (print the following text at color X, clear the screen, etc).
//
// Don't use this on files!

std::string bold_red     = "\e[1;31m";
std::string red          = "\e[0;31m";
std::string bold_cyan    = "\e[1;36m";
std::string cyan         = "\e[0;36m";
std::string bold_green   = "\e[1;32m";
std::string green        = "\e[0;32m";
std::string bold_yellow  = "\e[1;33m";
std::string yellow       = "\e[0;33m";
std::string reset_color  = "\e[0m";
std::string clear_screen = "[H[2J";

// Prefixes
std::string errorPrefix   = "Error:   ";
std::string warningPrefix = "Warning: ";
std::string debugPrefix   = "Debug:   ";
std::string verbosePrefix = "Verbose: ";

void Log::error(std::string msg)
{
    // if (isUsingFiles)
    //     stderr    << "Error:   " << msg << std::endl;
    // else
    std::cerr << bold_red << errorPrefix << msg << reset_color << std::endl;
}
void Log::warning(std::string msg)
{
    // if (isUsingFiles)
    //     stderr    << "Warning: " << msg << std::endl;
    // else
    std::cerr << bold_yellow << warningPrefix << msg << reset_color << std::endl;
}
void Log::log(std::string msg)
{
    // if (isUsingFiles)
    //     stdout    << msg << std::endl;
    // else
    std::cout << bold_green << msg << reset_color << std::endl;
}
void Log::debug(std::string msg)
{
    if (isDebugMode)
        std::cerr << bold_cyan << debugPrefix << msg << reset_color << std::endl;

//        Log::log("Debug:   " + msg);
}
void Log::verbose(std::string msg)
{
    if (isVerboseMode)
        std::cerr << cyan << verbosePrefix << msg << reset_color << std::endl;

//        Log::log("Debug:   " + msg);
}
void Log::debugMode(bool option)
{
    if (option == false)
    {
        isDebugMode = false;
        return;
    }

    // Prints a nice logo
    isDebugMode = true;
    Log::debug(">Debug mode activated");
}
void Log::verboseMode(bool option)
{
    if (option == false)
    {
        isVerboseMode = false;
        return;
    }

    // Prints a nice logo
    isVerboseMode = true;
    Log::verbose(">Verbose Mode activated");
}

