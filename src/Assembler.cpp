#include "Assembler.hpp"
#include <iostream>
#include "Log.hpp"

Assembler::Assembler(SintacticalAnalyser* syntax, std::string filename):
    syntax(syntax),
    filename(filename)
{ }
void Assembler::assemble()
{

    Log::debug("Assembler: Starting First Pass");
    syntax->firstPass(&(this->table));

    Log::debug("Assembler: Starting Second Pass");
    syntax->secondPass(&(this->table), &(this->rawCode));

    Log::debug("Assembler: Starting Output to File");
    this->rawCode.outputToFile(this->filename);
}

