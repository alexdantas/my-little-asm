#ifndef DERIVATIONTABLE_H_DEFINED
#define DERIVATIONTABLE_H_DEFINED

#include <inttypes.h>           // int16_t
#include <vector>
#include <string>

// Default initial size of the memory.
#define MEMORY_SIZE 65535

struct RawCodeLine
{
    int16_t operation;
    int16_t arg1;
    int16_t arg2;

    RawCodeLine(int16_t operation=-1, int16_t arg1=-1, int16_t arg2=-1):
        operation(operation), arg1(arg1), arg2(arg2)
    { }
};

/// This class holds the raw integer/binary code generated
/// by the `Assembler`.
///
/// It acts as an expandable memory region which can be
/// written to a file.
///
class DerivationTable
{
public:
    /// Creates a new memory holder with `size` blocks.
    DerivationTable(int size=MEMORY_SIZE);

    /// Adds an `operation` with its arguments on the memory.
    ///
    /// You can safely call this method several times, because
    /// it increments the written address automatically.
    /// It starts writing at address zero and goes on until
    /// you stop.
    ///
    /// If you want to be sure of the address you're writing,
    /// use `insert_at`.
    void insert(int16_t operation, int16_t arg1=-1, int16_t arg2=-1);

    /// Puts the `operation` and it's args on a specific memory
    /// address.
    ///
    /// Don't worry, memory expands as you write on it.
    ///
    /// @note You might overwrite addresses already written with
    ///       `insert()`.
    void insert_at(int n, int16_t operation, int16_t arg1=-1, int16_t arg2=-1);

    /// Outputs all the raw memory to `filename`.
    void outputToFile(std::string filename);

    /// Returns the specific memory block at `index`.
    int16_t operator[](int index)
    {
        return (this->table.at(index).operation);
    }

private:
    std::vector<RawCodeLine> table; ///< Raw table.

    /// Current address on which we're writing.
    ///
    /// Whenever you call `insert`, we write on
    /// `current_index` and increment this by one.
    ///
    /// @note If you call `insert_at`, it doesn't
    ///       affect this (so you might overwrite
    ///       addresses already written).
    int current_index;

    /// Last memory region to write on the file.
    ///
    /// When we `outputToFile()`, we write at most
    /// `last_index` memory blocks.
    /// This is the largest memory address you've
    /// written with `insert` or `insert_at`.
    int last_index;
};

#endif //DERIVATIONTABLE_H_DEFINED

