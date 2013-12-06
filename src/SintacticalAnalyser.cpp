#include <iostream>
#include <cstdlib> // atoi()
#include <ctype.h> // isdigit()
#include "SintacticalAnalyser.hpp"
#include "Assembler.hpp"
#include "InstructionTable.cpp"
#include "Log.hpp"
#include "utils.hpp"

// I KNOW global variables suck, but I couldn't figure out
// a clean way of doing what I did.
// (insert explanation here)
int global_address_of_last_instruction = 0;

// LOCAL helper functions

bool caseInsensitiveEqual(std::string str, const char* cstr)
{
    return (strcasecmp(str.c_str(), cstr) == 0);
}

SintacticalAnalyser::SintacticalAnalyser(LexicalAnalyser* lex):
    lex(lex)
{ }
SintacticalAnalyser::~SintacticalAnalyser()
{ }

// Pretty alias to ease throwing of exception on firstPass() and
// secondPass()
#define exception(msg) (throw AssemblerException(this->lex->lineCount(), msg))

void SintacticalAnalyser::firstPass(SymbolTable* table)
{
    int  memory_position = 0;
    int  text_memory_position = 0;

    int  current_line = 0;
    bool section_text = false;
    bool section_data = false;

    SymbolTable symbolData;
    SymbolTable symbolText;

    while (true)
    {
        // Tokens of the current line.
        // For a line like
        //     "LABEL: ADD ONE" it will be
        // a vector like
        //     ["LABEL:"] ["ADD"] ["ONE"]
        std::vector<std::string> tokens;
        tokens.clear();

        // Getting all tokens from the current line.
        while (tokens.empty())
        {
            if (lex->reachedEOF()) // End of the file!
                break;

            std::string token("");

            while ((token = lex->getToken()) != "")
                tokens.push_back(token);
        }

        if (lex->reachedEOF()) // Also needed
            break;

        // This is the current line!
        // We don't use it inside this loop, just for
        // debugging purposes.
        // We use `(void)` to shut up the compiler.
        current_line = lex->lineCount();
        (void)(current_line);

        // // NOTE BUG
        // // if the user specified commas (,) separated by
        // // spaces, there might be another token here!
        // if (tokens.size() == 0 || tokens.size() > 3)
        //     exception(std::string("Invalid formatting, wrong "
        //                           "number of terms. Note: Keep "
        //                           "commas together with their "
        //                           "arguments."));

        // The very first (non-comment) line of the code must be
        // a SECTION TEXT or SECTION DATA.
        if (!section_text && !section_data)
        {
            if (tokens.size() != 2)
                exception("Malformed SECTION");

            // Must be SECTION
            if (!caseInsensitiveEqual(tokens[0], "SECTION"))
                exception("Must specify SECTION TEXT or "
                          "SECTION DATA before any "
                          "code/declaration");

            // Must be TEXT or DATA
            if (!caseInsensitiveEqual(tokens[1], "DATA") &&
                !caseInsensitiveEqual(tokens[1], "TEXT"))
                exception("Invalid SECTION '" +
                          tokens[1] +
                          "', must be TEXT or DATA");

            // Which section are we dealing with?
            if (caseInsensitiveEqual(tokens[1], "DATA"))
            {
                section_data = true;
                Log::debug("Entered SECTION DATA");
            }
            else
            {
                section_text = true;
                Log::debug("Entered SECTION TEXT");
            }

            continue; // time to get me some new tokens
        }

        if (section_text)
        {
            // If we got to the beginning of another SECTION...
            if (caseInsensitiveEqual(tokens[0], "SECTION"))
            {
                if (caseInsensitiveEqual(tokens[1], "DATA"))
                {
                    section_text    = false;
                    section_data    = true;
                    memory_position = 0;
                    continue; // back to the beginning of the loop
                }
                else
                    exception("Invalid SECTION, must be DATA");
            }

// Syntatic sugar
#define LAST_CHAR(str) (*(str.end() - 1))

            // Declaration of a label
            // (last char of the first word is ':')
            if (LAST_CHAR(tokens[0]) == ':')
            {
                if (tokens[0].size() > MAXIMUM_LABEL_SIZE + 1) // +1 because of the ':'
                    exception("Label exceeded maximum label size (" +
                              intToString(MAXIMUM_LABEL_SIZE) +
                              ")");

                if (isdigit(tokens[0][0]))
                    exception("Labels cannot start with numbers.");

                if (table->has(tokens[0]))
                    exception("Redeclaration of label " +
                              tokens[0]);

                Log::debug("Added TEXT symbol '" + tokens[0] + "'");

                symbolText.insert(tokens[0], text_memory_position, Symbol::MEMORY_TEXT, Symbol::DATA_NOT_DATA);
                tokens.erase(tokens.begin()); // We delete the label so we
                                              // can treat the following
                                              // tokens seamlessly.
            }
            if (tokens.empty())
                continue; // back to the beginning of the loop

            // Now we deal with the (assumed) instruction
            if (!instructionTable.containsName(tokens[0]))
                exception("Invalid instruction '" +
                          tokens[0] +
                          "'");

            // So we got a valid instruction, great!
            text_memory_position++;

            int args_count = instructionTable.getArgumentsCountByName(tokens[0]);

            // Unusual argument size.
            // (The only exception is the instruction STOP, which has 0
            //  arguments)
            //
            //  (also, my mistery instruction)
            if ((args_count <= 0) ||
                (args_count >  2))
            {
                if (instructionTable.getOpcodeByName(tokens[0]) == OPCODE_STOP ||
                    instructionTable.getOpcodeByName(tokens[0]) == OPCODE_ISA)
                    continue; // back to the beginning of the loop

                exception("Invalid instruction '" +
                          tokens[0] +
                          "'");
            }

            // Missing arguments
            if (args_count > (int)(tokens.size() - 1))
                exception("Missing arguments for instruction '" +
                          tokens[0] +
                          "'");

            // There's a little trick, where arguments for the same instruction
            // MUST be separated by a colon.
            // I wonder if it would be best to simply cut this restriction off...
            if (args_count == 2)
            {
                if (LAST_CHAR(tokens[1]) != ',')
                    exception("Missing colon (',') to separate"
                              " arguments for instruction '" +
                              tokens[0] +
                              "'");

                tokens[1].erase(tokens[1].end() - 1);
            }

            // Finally, updating our current position based on the argument
            // count!
            text_memory_position += args_count;

            // So now I have a consistent token array of an existing instruction
            // with it's arguments!
            // See you later at the secondPass()!
        }

        if (section_data)
        {
            if (tokens.size() > 3)
                exception("Argument excess on SECTION DATA");

            // Just in case we're switching to another section.
            if (caseInsensitiveEqual(tokens[0], "SECTION"))
            {
                if (caseInsensitiveEqual(tokens[1], "TEXT"))
                {
                    section_data    = false;
                    section_text    = true;
                    text_memory_position = 0;
                    continue; // back to the beginning of the loop
                }
                else
                    exception("Invalid SECTION, must be TEXT");
            }

            // New labels are the only thing we're interested right now.
            if (LAST_CHAR(tokens[0]) == ':')
            {
                if (table->has(tokens[0]))
                    exception("Redeclaration of label '" +
                              tokens[0] +
                              "'");

                Log::debug("Added DATA symbol '" + tokens[0] + "'");

                if (tokens.size() == 1) // only the label, nothing else
                    symbolData.insert(tokens[0], memory_position, Symbol::MEMORY_DATA, Symbol::DATA_SPACE);
                else
                {
                    if (caseInsensitiveEqual(tokens[1], "CONST"))
                        symbolData.insert(tokens[0], memory_position, Symbol::MEMORY_DATA, Symbol::DATA_CONST);

                    else if (caseInsensitiveEqual(tokens[1], "SPACE"))
                        symbolData.insert(tokens[0], memory_position, Symbol::MEMORY_DATA, Symbol::DATA_SPACE);

                    else
                        exception("Unknown DATA format.");
                }

                tokens.erase(tokens.begin()); // We delete the label so we
                                              // can treat the following
                                              // tokens seamlessly.

                if (tokens.empty())
                    continue;

                // To make sure we got the addresses right, let's see
                // if SPACE wanted a vector.
                if (caseInsensitiveEqual(tokens[0], "SPACE"))
                {
                    if (tokens.size() == 2) // vector SPACE
                    {
                        int n = atoi(tokens[1].c_str());
                        memory_position += n;
                    }
                    else
                        memory_position++; // single SPACE
                }
                else if (caseInsensitiveEqual(tokens[0], "CONST"))
                {
                    memory_position++;
                }
                else
                {
                    exception("Invalid DATA format '" +
                              tokens[0] +
                              "'");
                }
            }

            // That was the only thing I needed to worry about.
            // See you later at the secondPass()!
        }
    }

    // Out of the loop!

    // If the last section was section_text, this means that
    // we must add an offset to all of the DATA, so that
    // we effectively switch them over on the final code.
    global_address_of_last_instruction = text_memory_position;

    // To make sure the address 0 is the first instruction
    // (and not raw data), we add a memory address offset
    // of the all the instructions.
    //
    // If `global_address_of_last_instruction` is zero, won't
    // make any difference on the final code.
    symbolData.offset(global_address_of_last_instruction);

    // Finally, copying the two intermediate tables into the
    // final symbol table.
    table->append(&symbolText);
    table->append(&symbolData);
}
void SintacticalAnalyser::secondPass(SymbolTable* table, DerivationTable* rawCode)
{
    this->lex->reset();

    // Just a reminder that any syntatic errors were already checked
    // on the firsPass().
    // So we'll have arrays of clean syntax ("LABEL: INSTRUCTION ARG1, ARG2").

    int  current_line = 1;
    bool section_text = false;
    bool section_data = false;

    // Temporary container for the data section.
    DerivationTable tmpData;

    while (true)
    {
        // Tokens of the current line, just like on firstPass.
        std::vector<std::string> tokens;
        tokens.clear();

        // Getting all tokens from the current line.
        while (tokens.empty())
        {
            if (lex->reachedEOF()) // End of the file!
                break;

            std::string token("");

            while ((token = lex->getToken()) != "")
                tokens.push_back(token);
        }

        if (lex->reachedEOF()) // Also needed
            break;

        // This is the current line!
        // We don't use it inside this loop, just for
        // debugging purposes.
        // We use `(void)` to shut up the compiler.
        current_line = lex->lineCount();
        (void)(current_line);

        // Declaration of a label, it's useless to us now.
        if (LAST_CHAR(tokens[0]) == ':')
        {
            // If this is true, then we made a big mistake!
            if (!(table->has(tokens[0])))
                exception("Internal error, inexistent label " +
                          tokens[0]);

            tokens.erase(tokens.begin());
            if (tokens.empty())
                continue; // get me some tokens
        }

        // SECTIONS, BAYB
        if (!section_text && !section_data)
        {
            // Must be SECTION
            if (!caseInsensitiveEqual(tokens[0], "SECTION"))
                exception("Must specify SECTION TEXT or SECTION "
                          "DATA before any code/declaration");

            // Must be TEXT or DATA
            if (!caseInsensitiveEqual(tokens[1], "DATA") &&
                !caseInsensitiveEqual(tokens[1], "TEXT"))
                exception("Invalid SECTION '" +
                          tokens[1] +
                          "', must be TEXT or DATA");

            // Which section are we dealing with?
            if (caseInsensitiveEqual(tokens[1], "DATA"))
                section_data = true;
            else
                section_text = true;

            continue; // back to the beginning of the loop
        }

        if (section_text)
        {
            // If we got to the beginning of another SECTION...
            if (caseInsensitiveEqual(tokens[0], "SECTION"))
            {
                if (caseInsensitiveEqual(tokens[1], "DATA"))
                {
                    section_text    = false;
                    section_data    = true;
                    continue; // back to the beginning of the loop
                }
                else
                    exception("Invalid SECTION, must be DATA");
            }

            // Ignoring label declarations, we've already dealt with
            // them on firstPass()
            if (LAST_CHAR(tokens[0]) == ':')
                tokens.erase(tokens.begin());

            // Now to the instructions!

            Log::debug("Found instruction '" + tokens[0] + "'");

            int args_count = instructionTable.getArgumentsCountByName(tokens[0]);

            if (args_count == 0)
            {
                rawCode->insert(instructionTable.getOpcodeByName(tokens[0]));
                continue; // back to the beginning of the loop, either
                          // STOP or ISA
            }

            int16_t instruction = instructionTable.getOpcodeByName(tokens[0]);
            int16_t arg1 = -1;
            int16_t arg2 = -1;

            // If we got here, the instruction has 1 or two arguments

            // Removing comma of first argument
            // (if the instruction has two arguments)
            if (args_count == 2)
                tokens[1].erase(tokens[1].end() - 1);

            // Now, we can have simple labels (LABEL) or
            // vectorial ones (LABEL[2]).

            // These will have the vectorial offset if any of the labels
            // are vectorial.
            // If not, they'll remain at zero.
            int memory_offset_arg1 = 0;
            int memory_offset_arg2 = 0;

            { // MUST MAKE THIS A FUNCTION OR SOMETHIGN
                size_t position1 = tokens[1].find("[");

                // If found "[", it's a vectorial label
                if (position1 != std::string::npos)
                {
                    // If didn't found "]", error!
                    size_t position2 = tokens[1].find("]");
                    if (position2 == std::string::npos)
                        exception("Misconstructed vectorial"
                                  "space (Did you forget the ']'?)");

                    // The value between []s.
                    // (note that position1 points to "[" and position2 points to "]"
                    //  That's why we need the "+1" and "-1")
                    std::string valueString(tokens[1].substr((position1 + 1), (position2 - 1)));

                    int value = atoi(valueString.c_str());

                    memory_offset_arg1 += value;

                    // Now that we have the memory position, we can safely delete
                    // everything after "["
                    std::string tmp = tokens[1].substr(0, position1);
                    tokens[1] = tmp;
                }
            }

            if (args_count == 2)
            { // MUST MAKE THIS A FUNCTION OR SOMETHING
                size_t position1 = tokens[2].find("[");

                // If found "[", it's a vectorial label
                if (position1 != std::string::npos)
                {
                    // If didn't found "]", error!
                    size_t position2 = tokens[2].find("]");
                    if (position2 == std::string::npos)
                        exception("Misconstructed vectorial"
                                  "space (Did you forget the ']'?)");

                    // The value between []s.
                    // (note that position1 points to "[" and position2 points to "]"
                    //  That's why we need the "+1" and "-1")
                    std::string valueString(tokens[2].substr((position1 + 1), (position2 - 1)));

                    int value = atoi(valueString.c_str());

                    memory_offset_arg2 += value;

                    // Now that we have the memory position, we can safely delete
                    // everything after "["
                    std::string tmp = tokens[2].substr(0, position1);
                    tokens[2] = tmp;
                }
            }

            // Also, append ':' so the symbol
            // table will recognize this as a symbol
            tokens[1] += ":";
            if (args_count == 2)
                tokens[2] += ":";

            if (!table->has(tokens[1]))
                exception("Undefined label '" +
                          tokens[1] +
                          "'");
            if (args_count == 2)
                if (!table->has(tokens[2]))
                    exception("Undefined label '" +
                              tokens[2] +
                              "'");

            arg1 = table->memoryPositionOf(tokens[1]);
            if (memory_offset_arg1 != 0)
                arg1 += memory_offset_arg1;

            if (args_count == 2) // for now only COPY
            {
                arg2 = table->memoryPositionOf(tokens[2]);
                if (memory_offset_arg2 != 0)
                    arg2 += memory_offset_arg2;
            }

            // ENFORCING INSTRUCTION RESTRICTIONS

            // RIGHT before inserting into the final memory, let's check
            // if the user is trying to jump into the DATA segment
            if (instruction == OPCODE_JMP  ||
                instruction == OPCODE_JMPN ||
                instruction == OPCODE_JMPP ||
                instruction == OPCODE_JMPZ)
                if (table->isDataSymbol(tokens[1]))
                    exception("Attempt to jump into SEGMENT DATA memory.");

            // Also, it'd be nice if we could prevent the user from
            // writing into TEXT memory
            if (instruction == OPCODE_STORE ||
                instruction == OPCODE_INPUT)
            {
                if (!(table->isDataSymbol(tokens[1])))
                    exception("Attempt to write into SEGMENT TEXT memory.");

                if (table->isConstSymbol(tokens[1]))
                    exception("Attempt to write into CONST memory.");
            }

            if  (instruction == OPCODE_COPY)
            {
                if (!(table->isDataSymbol(tokens[2])))
                    exception("Attempt to write into SEGMENT TEXT memory.");

                if (table->isConstSymbol(tokens[2]))
                    exception("Attempt to write into CONST memory.");
            }

            // Everything's fine now!
            rawCode->insert(instruction, arg1, arg2); // Even if it's
                                                      // -1 it won't
                                                      // appear on the
                                                      // final code.
        }

        if (section_data)
        {
            // The trick here is inserting all data right after the
            // last instruction.
            // To do so, we keep this `data_index` counter and append
            // every SPACE or CONST to the last instruction address
            // plus this `data_index`.

            static int data_index = 0;

            // If we got to the beginning of another SECTION...
            if (caseInsensitiveEqual(tokens[0], "SECTION"))
            {
                if (caseInsensitiveEqual(tokens[1], "TEXT"))
                {
                    section_data = false;
                    section_text = true;
                    continue; // back to the beginning of the loop
                }
                else
                    exception("Invalid SECTION, must be TEXT");
            }

            // Ignoring label declarations, we've already dealt with
            // them on firstPass()
            if (LAST_CHAR(tokens[0]) == ':')
                tokens.erase(tokens.begin());

// This alias appends `n` to data memory
// (it starts right after the last instruction).
#define DATA_INSERT(n)                                                                  \
            {                                                                           \
                rawCode->insert_at(global_address_of_last_instruction + data_index, n); \
                data_index++;                                                           \
            }

            // SPACE sets a memory address to zero.
            // We can have a single or multiple SPACES.
            if (caseInsensitiveEqual(tokens[0], "SPACE"))
            {
                // Format "SPACE"
                if (tokens.size() == 1)
                {
                    DATA_INSERT(0);
                    Log::debug("Added SPACE");
                }
                // Format "SPACE n"
                else if (tokens.size() == 2)
                {
                    // Adding n blank SPACEs.
                    int n = atoi(tokens[1].c_str());
                    for (int k = 0; k < n; k++)
                        DATA_INSERT(0);

                    Log::debug("Added SPACE of size '" + intToString(n) + "'");
                }
                else
                    exception("Invalid SPACE format.");
            }

            // User-defined constant.
            //
            // This part is tricky. The user can provide:
            // * Decimal constants: 2, -5, 122...
            // * Hex constants: 0xFFB, -0x2f, 0x4
            // * Binary constants: 0b00011, 0b10010
            else if (caseInsensitiveEqual(tokens[0], "CONST"))
            {
                int num = stringToInt(tokens[1]);

                // Checking for overflow/underflow
                if (num < -32768)
                    Log::warning("Line " +
                                 intToString(this->lex->lineCount()) +
                                 ": Underflow on constant '" +
                                 tokens[1] +
                                 "' (value: " +
                                 intToString(num) +
                                 ")");

                if (num > 32767)
                    Log::warning("Line " +
                                 intToString(this->lex->lineCount()) +
                                 ": Overflow on constant '" +
                                 tokens[1] +
                                 "' (value: " +
                                 intToString(num) +
                                 ")");

                int16_t c = (int16_t)(num);

                DATA_INSERT(c);
                Log::debug("Added CONST '" + intToString(c) + "'");
            }
            else
            {
                exception("Invalid Instruction '" + tokens[0] + "'");
            }
        }
    }
}

