#ifndef ASSEMBLER_H_DEFINED
#define ASSEMBLER_H_DEFINED

#include <string>

#include <Assembler/SintacticalAnalyser.hpp>
#include <Assembler/SymbolTable.hpp>
#include <Assembler/DerivationTable.hpp>

struct AssemblerException
{
    int line;
    std::string message;

    AssemblerException(int line, std::string message):
        line(line), message(message)
    { }
};

/// The main class, mate.
///
/// This is where the main algorithm lurks.
class Assembler
{
public:
    /// Initializes an assembler with *syntax*.
    /// It will assemble on the output *filename*.
    ///
    Assembler(SintacticalAnalyser* syntax, std::string filename);

    /// Does the main work, outputting it's result to `outputFile`.
    void assemble();

private:
    SintacticalAnalyser* syntax;
    std::string filename;

    SymbolTable table;
    DerivationTable rawCode;
};

#endif //ASSEMBLER_H_DEFINED

