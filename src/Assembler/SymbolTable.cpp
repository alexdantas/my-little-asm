#include <Assembler/SymbolTable.hpp>

Symbol::Symbol(std::string label, int16_t memory_position, MemoryType memory_type, DataType data_type):
    label(label),
    memory_position(memory_position),
    memory_type(memory_type),
    data_type(data_type)
{ }

SymbolTable::SymbolTable()
{ }
SymbolTable::~SymbolTable()
{
    this->table.clear();
}
bool SymbolTable::has(std::string label)
{
    for (unsigned int i = 0; i < (this->table.size()); i++)
        if (this->table[i].label == label)
            return true;

    return false;
}
void SymbolTable::insert(std::string label, int memory_position, Symbol::MemoryType memory_type, Symbol::DataType data_type)
{
    Symbol s(label, memory_position, memory_type, data_type);
    this->table.push_back(s);
}
int SymbolTable::size()
{
    return (this->table.size());
}
void SymbolTable::offset(int n)
{
    for (unsigned int i = 0; i < (this->table.size()); i++)
        this->table[i].memory_position += n;
}
void SymbolTable::append(SymbolTable* other)
{
    for (unsigned int i = 0; i < (other->table.size()); i++)
        this->table.push_back(other->table[i]);
}
int SymbolTable::memoryPositionOf(std::string label)
{
    for (unsigned int i = 0; i < (this->table.size()); i++)
        if (this->table[i].label == label)
            return this->table[i].memory_position;

    return -1;
}
bool SymbolTable::isDataSymbol(std::string label)
{
    for (unsigned int i = 0; i < (this->table.size()); i++)
        if (this->table[i].label == label)
            return (this->table[i].memory_type == Symbol::MEMORY_DATA);

    return false;
}
bool SymbolTable::isConstSymbol(std::string label)
{
    if (!(this->isDataSymbol(label)))
        return false;

    for (unsigned int i = 0; i < (this->table.size()); i++)
        if (this->table[i].label == label)
            return (this->table[i].data_type == Symbol::DATA_CONST);

    return false;
}

