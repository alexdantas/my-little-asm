/// Definition and implementation of the big table with
/// all the instructions and what they do.
///

#include <string>
#include <vector>
#include <cstring>

// All possible instructions
enum Opcodes
{
    OPCODE_ADD = 1,
    OPCODE_SUB,
    OPCODE_MULT,
    OPCODE_DIV,
    OPCODE_JMP,
    OPCODE_JMPN,
    OPCODE_JMPP,
    OPCODE_JMPZ,
    OPCODE_COPY,
    OPCODE_LOAD,
    OPCODE_STORE,
    OPCODE_INPUT,
    OPCODE_OUTPUT,
    OPCODE_STOP,
    OPCODE_ISA
};


/// A single instruction, baby
struct Instruction
{
    std::string name;       ///< Human-readable identificator.
    int opcode;             ///< Machine-readable identificator.
    int arguments_ammount;  ///< How many arguments does it expect
    int size;               ///< It's size in words. On our case, it's int16_t.

    /// Creates a new instruction with `name`, `opcode` and `arguments_ammount`.
    Instruction(std::string name, int opcode, int arguments_ammount):
        name(name),
        opcode(opcode),
        arguments_ammount(arguments_ammount),
        size(1 + arguments_ammount) // + 1 size because of it's opcode
    { }
};

/// The whole table of instructions.
///
/// @note Do not instantiate this! There is a global one
///       that was already made for this! See below.
class InstructionTable
{
public:
    /// Initializing the whole table.
    ///
    /// Here's where you specify your own language's instructions.
    InstructionTable()
    {

// Quickly adding instructions to the instruction set
#define ADD_INSTR(name, opcode, arguments_ammount) \
        (this->instructions.push_back(Instruction(name, opcode, arguments_ammount)))

        // "regular" language instructions
        ADD_INSTR("ADD",    OPCODE_ADD,    1);
        ADD_INSTR("SUB",    OPCODE_SUB,    1);
        ADD_INSTR("MULT",   OPCODE_MULT,   1);
        ADD_INSTR("DIV",    OPCODE_DIV,    1);
        ADD_INSTR("JMP",    OPCODE_JMP,    1);
        ADD_INSTR("JMPN",   OPCODE_JMPN,   1);
        ADD_INSTR("JMPP",   OPCODE_JMPP,   1);
        ADD_INSTR("JMPZ",   OPCODE_JMPZ,   1);
        ADD_INSTR("COPY",   OPCODE_COPY,   2);
        ADD_INSTR("LOAD",   OPCODE_LOAD,   1);
        ADD_INSTR("STORE",  OPCODE_STORE,  1);
        ADD_INSTR("INPUT",  OPCODE_INPUT,  1);
        ADD_INSTR("OUTPUT", OPCODE_OUTPUT, 1);
        ADD_INSTR("STOP",   OPCODE_STOP,   0);
        ADD_INSTR("ISA",    OPCODE_ISA,    0); // my mistery instruction

        // assembler directives
        ADD_INSTR("SECTION", -1, 0);
        ADD_INSTR("SPACE",   -1, 0);
        ADD_INSTR("CONST",   -1, 0);
    }

    int getOpcodeByName(std::string name)
    {
        for (unsigned int i = 0; i < (this->instructions.size()); i++)
            if (strcasecmp(this->instructions[i].name.c_str(), name.c_str()) == 0)
                return (this->instructions[i].opcode);

        return -1;
    }

    std::string getNameByOpcode(int opcode)
    {
        for (unsigned int i = 0; i < (this->instructions.size()); i++)
            if (this->instructions[i].opcode == opcode)
                return (this->instructions[i].name);

        return "";
    }

    int getArgumentsCountByName(std::string name)
    {
        for (unsigned int i = 0; i < (this->instructions.size()); i++)
            if (strcasecmp(this->instructions[i].name.c_str(), name.c_str()) == 0)
                return (this->instructions[i].arguments_ammount);

        return 0;
    }

    bool containsName(std::string name)
    {
        for (unsigned int i = 0; i < (this->instructions.size()); i++)
            if (strcasecmp(this->instructions[i].name.c_str(), name.c_str()) == 0)
                return true;

        return false;
    }

    bool containsOpcode(int opcode)
    {
        for (unsigned int i = 0; i < (this->instructions.size()); i++)
            if (this->instructions[i].opcode == opcode)
                return true;

        return false;
    }

private:
    std::vector<Instruction> instructions;
};


/// The global instruction table.
InstructionTable instructionTable;

