// 1o trabalho de Software Basico, 2/2013
// Alexandre Dantas (10/0090788)
//
//

#include <iostream>
#include <cstdlib>              // exit()
#include <inttypes.h>           // int16_t
#include <cstring>
#include "Assembler.hpp"
#include "Log.hpp"
#include "utils.hpp"

// exit return value errors
#define ERROR_INVALID_ARGUMENTS   666
#define ERROR_COULD_NOT_OPEN_FILE 1337
#define ERROR_INVALID_INSTRUCTION 24
#define ERROR_EXCEPTION_CAUGHT    11

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "assembler: fatal error: no input files"
                  << std::endl
                  << std::endl
                  << "Usage:"
                  << std::endl
                  << "$ assembler (input asm file) (output obj file)"
                  << std::endl;

        exit(ERROR_INVALID_ARGUMENTS);
    }

    if (argc == 4)
        if (strcmp(argv[3], "-v") == 0)
            Log::debugMode(true);

    try
    {
        LexicalAnalyser*     lex    = new LexicalAnalyser(argv[1]);
        SintacticalAnalyser* syntax = new SintacticalAnalyser(lex);

        Assembler assembler(syntax, argv[2]);
        assembler.assemble();
    }
    catch (const char* error)
    {
        Log::error("Exception caught: " +
                   std::string(error));

        return ERROR_COULD_NOT_OPEN_FILE;
    }
    catch (AssemblerException e)
    {
        Log::error("Line " +
                   intToString(e.line) +
                   ": " +
                   e.message);

        return ERROR_EXCEPTION_CAUGHT;
    }
    return 0;
}

