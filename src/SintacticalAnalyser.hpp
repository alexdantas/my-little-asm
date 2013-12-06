#ifndef SINTACTICALANALYSER_H_DEFINED
#define SINTACTICALANALYSER_H_DEFINED

#include "LexicalAnalyser.hpp"
#include "SymbolTable.hpp"
#include "DerivationTable.hpp"

///
class SintacticalAnalyser
{
public:
    SintacticalAnalyser(LexicalAnalyser* lex);
    virtual ~SintacticalAnalyser();

    /// Fills the symbol table with all labels on the code.
    ///
    /// This goes through the whole code inside the lexical
    /// analyser, building the symbol table.
    ///
    /// After this we're ready to go to the `secondPass`.
    void firstPass(SymbolTable* table);

    /// Actually generates the code based on the symbol table.
    ///
    /// This pass uses the symbol table created by the `firstPass`
    /// and outputs the raw code on the derivation table `rawCode`.
    ///
    /// That table can then be used to print to a file or simulate
    /// immediately.
    void secondPass(SymbolTable* table, DerivationTable* rawCode);

private:
    LexicalAnalyser* lex;
};

#endif //SINTACTICALANALYSER_H_DEFINED

