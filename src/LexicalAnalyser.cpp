#include "LexicalAnalyser.hpp"
#include <iostream>
#include <cstdlib>
#include <fstream>

LexicalAnalyser::LexicalAnalyser(std::string filename):
    line_count(0),
    end_of_file(false)
{
    // Opening file for reading
    std::fstream file(filename.c_str(), std::ios_base::in);

    if (!file.is_open())
        throw "Lexycal Analyser: Input file could not be opened";

    // Storing the whole file into our memory stream, without
    // comments but preserving blank lines.
    while (!file.eof())
    {
        std::string current_line;
        std::getline(file, current_line);

        current_line = this->removeComments(current_line);
        current_line += "\n"; // need this for correct line
                              // numbering

        this->stream       << current_line;
        this->backupStream << current_line;
    }
    file.close();

    // I need to call this for the first time so it
    // refreshes internal data structures and such.
    this->getToken();
}
std::string LexicalAnalyser::getToken()
{
    if (this->end_of_file)
        return "";

    // This is a temporary string stream that will hold
    // the current line.
    static std::stringstream tmp("");

    // This is the token that we'll try to get from the
    // temporary stream.
    std::string token("");

    // We'll keep tossing lines from the file into our
    // temporary stream, while it doesn't give us tokens
    while (!(tmp >> token))
    {
        static bool returned_already = false;

        if (!returned_already)
        {
            returned_already = true;
            return "";
        }

        std::string line("");

        while (line.empty())
        {
            // If we've reached the end of the file stream,
            if (this->stream.rdbuf()->in_avail() == 0)
            {
                // cut it off!
                this->end_of_file = true;
                return "";
            }

            std::getline(this->stream, line);
            this->line_count++;
        }

        // Setting the contents of our temporary stream
        tmp.clear();
        tmp.str(line);

        returned_already = false;
    }
    return token;
}
int LexicalAnalyser::lineCount()
{
    if (this->line_count == 0)
        return 1;

    return (this->line_count);
}
std::string LexicalAnalyser::removeComments(std::string line)
{
    std::string tmp("");

    for (unsigned int i = 0; i < (line.size()); i++)
    {
        if (line[i] == CHAR_COMMENT)
            break;

        tmp.push_back(line[i]);
    }

    return tmp;
}
bool LexicalAnalyser::reachedEOF()
{
    return (this->end_of_file);
}
void LexicalAnalyser::reset()
{
    this->line_count  = 0;
    this->end_of_file = false;

    this->stream << this->backupStream.rdbuf();
}

