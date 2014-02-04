#include <sstream>              // stringstream
#include <cstdlib>              // strtol()
#include <iostream>

#include "Utils.hpp"

std::string intToString(int num)
{
    std::stringstream ss;
    ss.str("");
    ss << num;

    std::string numStr = ss.str();
    return numStr;
}

int stringToInt(std::string str)
{
    // Hexadecimal constants
    if (str.find("0x") != std::string::npos ||
        str.find("0X") != std::string::npos)
    {
        return strtol(str.c_str(), NULL, 16);
    }

    // Binary constants
    if (str.find("0b") != std::string::npos ||
        str.find("0B") != std::string::npos)
    {
        // strtol() doesn't work with "0b", erasing it
        // (assuming it's at the start of the string)
        str.erase(str.begin());
        str.erase(str.begin());

        return strtol(str.c_str(), NULL, 2);
    }

    // Decimal constants
    return atoi(str.c_str());
}

std::string basename(std::string filename)
{
	size_t pos = filename.rfind(".");

	if (pos == std::string::npos) // didn't find
		return "";

	if (pos == 0) // . is at the front - dotfile
		return "";

	return filename.substr(0, pos);
}

