//            ,,                                ,,
//  .M"""bgd  db                              `7MM           mm
// ,MI    "Y                                    MM           MM
// `MMb.    `7MM  `7MMpMMMb.pMMMb.`7MM  `7MM    MM   ,6"Yb.mmMMmm ,pW"Wq.`7Mb,od8
//   `YMMNq.  MM    MM    MM    MM  MM    MM    MM  8)   MM  MM  6W'   `Wb MM' "'
// .     `MM  MM    MM    MM    MM  MM    MM    MM   ,pm9MM  MM  8M     M8 MM
// Mb     dM  MM    MM    MM    MM  MM    MM    MM  8M   MM  MM  YA.   ,A9 MM
// P"Ybmmd" .JMML..JMML  JMML  JMML.`Mbod"YML..JMML.`Moo9^Yo.`Mbmo`Ybmd9'.JMML.
//
// Basic Software - 1st Assignment, 2/2013
// Alexandre Dantas (10/0090788)
//
// Simulates programs assembled by my `assembler`.
//
// Little-endian forevis

#include <iostream>
#include <vector>
#include <cstdlib>              // exit()
#include <fstream>
#include <inttypes.h>           // int16_t
#include "InstructionTable.cpp"


#define MEMORY_SIZE 65536 // 2^16 16-bit words

// Simulator registers
int16_t ACC;              // Accumulator
int16_t PC;               // Program Counter
std::vector<int16_t> MEM; // Memory (must be [MEMORY_SIZE]);

// `exit` return value errors
#define ERROR_INVALID_ARGUMENTS   666
#define ERROR_COULD_NOT_OPEN_FILE 1337
#define ERROR_INVALID_INSTRUCTION 24
#define ERROR_DIVISION_BY_ZERO    2222 // pra central do Brasil
#define ERROR_INPUT_OVERFLOW_OR_UNDERFLOW 31415

void mistery(); // guess what?

int main(int argc, char* argv[])
{
    // checking arguments (input file)
    if (argc < 2)
    {
        std::cout << "simulador: fatal error: no input files"
                  << std::endl
                  << std::endl
                  << "Usage:\n$ simulador (input object file)"
                  << std::endl;

        exit(ERROR_INVALID_ARGUMENTS);
    }

    // verbose mode?
    bool config_verbose_mode = false;

    if (argc == 3)
    {
        if (strcmp(argv[2], "-v") == 0)
        {
            std::cout << "* Verbose output (lines starting with '*')"
                      << std::endl;

            config_verbose_mode = true;
        }
    }

    // opening file
    std::fstream input(argv[1], std::ios::binary | std::ios::in);
    if (!input.is_open())
    {
        std::cout << "Error: couldn't open file '"
                  << argv[1]
                  << "'"
                  << std::endl;

        exit(ERROR_COULD_NOT_OPEN_FILE);
    }

    // initializing simulator environment
    {
        ACC = 0;
        PC  = 0;
        MEM.resize(MEMORY_SIZE);
        for (unsigned int i = 0; i < MEM.size(); i++)
            MEM[i] = 0;
    }

    // reading all of the file's contents into memory and
    // then closing it
    int index = 0;
    while (true)
    {
        int16_t chunk;

        // read() forces me to read a (char*),
        // damn ugly line...
        input.read((char*)(&chunk), sizeof(int16_t));
        if (input.eof())
            break;

        MEM[index] = chunk;
        index++;
    }
    // `index` now holds the size of the file
    input.close();

    // parsing memory contents
    while (true)
    {
        int16_t current_instruction;

        current_instruction = MEM[PC];
        PC++;

        // PC now points to the first argument of the instruction.
        // If the instruction doesn't have any arguments, it points
        // to the next instruction.

        // Instruction sanity check
        if (!instructionTable.containsOpcode(current_instruction))
        {
            std::cout << "Invalid instruction (opcode: "
                      << current_instruction
                      << ")"
                      << std::endl;

            exit(ERROR_INVALID_INSTRUCTION);
        }

        // The instruction name (important)
        std::string name(instructionTable.getNameByOpcode(current_instruction));

        if (config_verbose_mode)
        {
            std::cout << "*" << std::endl
                      << "* Instruction: " << name
                      << " (opcode " << current_instruction << ")" << std::endl
                      << "* ACC:         " << ACC << std::endl
                      << "* PC:          " << PC  << std::endl;
        }

/// Compares case-insensitively the instruction name and a (const char*).
///
/// NOTE BUG if you change the variable name, make sure to update this too!
#define INSTRUCTION_IS(b) (strcasecmp(name.c_str(), b) == 0)

        // Now we implement the specific behavior of each instruction
        // based on our internal registers.

        if (INSTRUCTION_IS("ADD"))
        {
            int16_t arg = MEM[PC];
            ACC += MEM[arg];
            PC++;

            if (config_verbose_mode)
                std::cout << "* Adding '"
                          << MEM[arg]
                          << "' to ACC"
                          << std::endl;
        }
        else if (INSTRUCTION_IS("SUB"))
        {
            int16_t arg = MEM[PC];
            ACC -= MEM[arg];
            PC++;

            if (config_verbose_mode)
                std::cout << "* Subtracting '"
                          << MEM[arg]
                          << "' from ACC"
                          << std::endl;
        }
        else if (INSTRUCTION_IS("MULT"))
        {
            int16_t arg = MEM[PC];
            ACC *= MEM[arg];
            PC++;

            if (config_verbose_mode)
                std::cout << "* Multiplicating ACC to "
                          << MEM[arg]
                          << std::endl;
        }
        else if (INSTRUCTION_IS("DIV"))
        {
            int16_t arg = MEM[PC];

            // testing division by zero!
            if (MEM[arg] == 0)
            {
                std::cout << "Error: Attempt to divide by zero!"
                          << std::endl;

                exit(ERROR_DIVISION_BY_ZERO);
            }

            ACC /= MEM[arg];
            PC++;

            if (config_verbose_mode)
                std::cout << "* Dividing ACC to "
                          << MEM[arg]
                          << std::endl;
        }
        else if (INSTRUCTION_IS("JMP"))
        {
            PC = MEM[PC];
            if (config_verbose_mode)
                std::cout << "* Jumping to address "
                          << PC
                          << std::endl;
        }
        else if (INSTRUCTION_IS("JMPN"))
        {
            if (ACC < 0)
                PC = MEM[PC];
            else
                PC++;

            if (config_verbose_mode)
                std::cout << "* Jumping to address "
                          << PC
                          << std::endl;
        }
        else if (INSTRUCTION_IS("JMPP"))
        {
            if (ACC > 0)
                PC = MEM[PC];
            else
                PC++;

            if (config_verbose_mode)
                std::cout << "* Jumping to address "
                          << PC
                          << std::endl;
        }
        else if (INSTRUCTION_IS("JMPZ"))
        {
            if (ACC == 0)
                PC = MEM[PC];
            else
                PC++;

            if (config_verbose_mode)
                std::cout << "* Jumping to address "
                          << PC
                          << std::endl;
        }
        else if (INSTRUCTION_IS("COPY"))
        {
            // no need to remember current position
            // because PC will send me to the next instruction
            // anyways
            int16_t arg1 = MEM[PC];
            PC++;
            int16_t arg2 = MEM[PC];
            PC++;

            if (config_verbose_mode)
                std::cout << "* Copying address "
                          << arg1      << " ("
                          << MEM[arg1] << ")"
                          << " to address "
                          << arg2      << " ("
                          << MEM[arg2] << ")"
                          << std::endl;

            MEM[arg2] = MEM[arg1];
        }
        else if (INSTRUCTION_IS("LOAD"))
        {
            int16_t arg = MEM[PC];
            ACC = MEM[arg];
            PC++;

            if (config_verbose_mode)
                std::cout << "* Loading '"
                          << MEM[arg]
                          << "' to ACC"
                          << std::endl;
        }
        else if (INSTRUCTION_IS("STORE"))
        {
            int16_t arg = MEM[PC];
            MEM[arg] = ACC;
            PC++;

            if (config_verbose_mode)
                std::cout << "* Storing '"
                          << ACC
                          << "' into address "
                          << arg
                          << std::endl;
        }
        else if (INSTRUCTION_IS("INPUT"))
        {
            int input = 0;
            std::cin >> input;

            if (input < -32768)
            {
                std::cerr << "Error: INPUT value is too low to "
                          << "fit into memory (minimum "
                          << -32768
                          << ")"
                          << std::endl;
                exit(ERROR_INPUT_OVERFLOW_OR_UNDERFLOW);
            }

            if (input > 32767)
            {
                std::cerr << "Error: INPUT value is too big to "
                          << "fit into memory (maximum "
                          << 32767
                          << ")"
                          << std::endl;
                exit(ERROR_INPUT_OVERFLOW_OR_UNDERFLOW);
            }

            int16_t arg = MEM[PC];
            MEM[arg] = (int16_t)(input);
            PC++;
        }
        else if (INSTRUCTION_IS("OUTPUT"))
        {
            int16_t arg = MEM[PC];

            if (config_verbose_mode)
                std::cout << "* Outputting address "
                          << arg
                          << std::endl;

            std::cout << MEM[arg] << std::endl;
            PC++;
        }
        else if (INSTRUCTION_IS("STOP"))
        {
            break;
        }
        else if (INSTRUCTION_IS("ISA")) // my mistery instruction
        {
            mistery();
        }
        else
        {
            std::cout << "Invalid instruction (opcode: "
                      << current_instruction
                      << ")"
                      << std::endl;

            exit(ERROR_INVALID_INSTRUCTION);
        }
    }

    if (config_verbose_mode)
        std::cout << "* Program terminated normally"
                  << std::endl;

    return 0;
}























































void mistery()
{
#define AsAaaA "\n"
#define AaaaaAAA <<
            std::cout
   AaaaaAAA "                ,        ,         " AaaaaAAA AsAaaA       AaaaaAAA "               /(        )`        " AaaaaAAA AsAaaA
                AaaaaAAA "               \\ \\___   / |        "
AaaaaAAA AsAaaA
             AaaaaAAA "               /- _  `-/  '        " AaaaaAAA AsAaaA
                      AaaaaAAA "              (/\\/ \\ \\   /\\        " AaaaaAAA
                AsAaaA AaaaaAAA "              / /   | `    \\       " AaaaaAAA AsAaaA AaaaaAAA "              O O   ) /    |       " AaaaaAAA AsAaaA
               AaaaaAAA "              `-^--'`<     '       " AaaaaAAA AsAaaA
                AaaaaAAA "             (_.)  _  )   /        " AaaaaAAA AsAaaA AaaaaAAA "              `.___/`    /         " AaaaaAAA AsAaaA
                    AaaaaAAA
                "                `-----' /          " AaaaaAAA
                AsAaaA
       AaaaaAAA "   <----.     __ / __   \\          " AaaaaAAA AsAaaA AaaaaAAA"   <----|====O)))==) \\) /====      " AaaaaAAA AsAaaA
  AaaaaAAA "   <----'    `--' `.__,' \\         " AaaaaAAA AsAaaA
              AaaaaAAA "                |        |         " AaaaaAAA AsAaaA
                 AaaaaAAA "                 \\       /       /\\ " AaaaaAAA AsAaaA
        AaaaaAAA "            ______( (_  / \\______/ " AaaaaAAA AsAaaA
                  AaaaaAAA "          ,'  ,-----'   |          " AaaaaAAA AsAaaA
  AaaaaAAA "          `--{__________) " AaaaaAAA AsAaaA;
}

