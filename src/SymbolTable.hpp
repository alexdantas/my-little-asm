#ifndef SYMBOLTABLE_H_DEFINED
#define SYMBOLTABLE_H_DEFINED

#include <vector>
#include <string>
#include <inttypes.h>           // int16_t

/// A single symbol.
struct Symbol
{
    enum MemoryType { MEMORY_TEXT, MEMORY_DATA };
    enum DataType { DATA_CONST, DATA_SPACE, DATA_NOT_DATA };

    std::string label;
    int16_t     memory_position;
    MemoryType  memory_type;
    DataType    data_type;

    Symbol(std::string label, int16_t memory_position, MemoryType memory_type, DataType data_type);
};

/// The whole symbol table.
class SymbolTable
{
public:
    SymbolTable();
    virtual ~SymbolTable();

    bool has(std::string label);
    void insert(std::string label, int memory_position, Symbol::MemoryType memory_type, Symbol::DataType data_type);
    int size();
    void offset(int n);
    void append(SymbolTable* other);
    int memoryPositionOf(std::string label);

    /// Tells if `label` is part of the SEGMENT DATA.
    bool isDataSymbol(std::string label);

    /// Tells if `label` is a CONST DATA symbol.
    bool isConstSymbol(std::string label);

private:
    std::vector<Symbol> table; ///< Raw table.
};

#endif //SYMBOLTABLE_H_DEFINED

