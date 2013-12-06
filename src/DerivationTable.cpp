#include <fstream>
#include "DerivationTable.hpp"
#include "Log.hpp"

DerivationTable::DerivationTable(int size):
    current_index(0)
{
    this->table.resize(size, RawCodeLine());
}
void DerivationTable::insert_at(int n, int16_t operation, int16_t arg1, int16_t arg2)
{
    // does this make any difference at all?
    // if (n > (int)(this->table.size()))
    //     this->table.resize(n+10, 0);

    RawCodeLine r(operation, arg1, arg2);
    this->table.insert(this->table.begin() + n, r);
}

void DerivationTable::insert(int16_t operation, int16_t arg1, int16_t arg2)
{
    this->insert_at(this->current_index, operation, arg1, arg2);
    this->current_index++;
}
void DerivationTable::outputToFile(std::string filename)
{
    std::fstream file;

    file.open(filename.c_str(), std::ios::binary | std::ios::out);
    if (!file.is_open())
        throw "DerivationTable: Output file could not be opened";

    Log::debug("Opened '" + filename + "'");

    // Writing all the raw integers on the file.
    for (unsigned int i = 0; i < (this->table.size()); i++)
    {

// Hanly alias for writing thigs - read() expects char*
#define WRITE(a) (file.write((char*)(&(a)), sizeof(int16_t)))

        // This part is very important: WILL IGNORE ANY -1

        if (this->table[i].operation != -1)
        {
            WRITE(this->table[i].operation);
        }

        if (this->table[i].arg1 != -1)
        {
            WRITE(this->table[i].arg1);
        }

        if (this->table[i].arg2 != -1)
        {
            WRITE(this->table[i].arg2);
        }

    }

    file.close();
}

