#include <inttypes.h>  // int16_t
#include <fstream>
#include <iostream>

#include <LanguageDefinitions.hpp>
#include <Translator/Translator.hpp>
#include <Translator/ReverseSymbolTable.hpp>
#include <Misc/Utils.hpp>
#include <Misc/Log.hpp>

Translator::Translator(std::string filename)
{
    std::string name = basename(filename);
    if (name == "")
        throw "Translator: Invalid filename '" + filename +"'";

    this->input_filename  = filename;
    this->output_filename = name + ".s";
}
void Translator::translate()
{
    Log::debug("Translating into " + output_filename);

    // Input file
    std::ifstream file(input_filename.c_str(), std::ios::binary);
    if (!file.is_open())
        throw "Translator: Input file could not be opened";

    Log::debug("Opened '" + input_filename + "'");

    // Output file
    std::ofstream out_file(output_filename.c_str());
    if (!file.is_open())
        throw "Translator: Output file could not be opened";

    Log::debug("Opened '" + output_filename + "'");

    ReverseSymbolTable symbol_table;

// Macro to ease reading - reads a chunk from the file.
#define READ(n)  	                               \
	    {										   \
	        file.read((char*)&n, sizeof(int16_t)); \
	        if (file.eof())                        \
		        break;                             \
        }

    // Where in the binary file we're currently at.
    int position_count = 0;

    // Need to build the symbol table.
    while (true)
    {
        // Getting chunk out of file
        int16_t chunk = 0;

        READ(chunk);

        symbol_table.add(position_count);
        position_count++;
    }

    // After reaching `eof()`, all I/O operations fail.
    // Thus, we need to call `clear()` on the stream.
    file.clear();
    file.seekg(0, std::ios::beg);

    // things you gotta keep in mind
    position_count = 0;
    bool first_line   = true;
    bool section_text = true;

    // Going through the file
    while (true)
    {
        // Getting chunk out of file
        int16_t chunk = 0;
        READ(chunk);

        // Setting things based on chunk

        // Always start as section text
        if (first_line)
        {
	        // Copying IO procedures
	        std::string file("src/Translator/io.s");
	        std::ifstream input_output(file.c_str());
	        if (!input_output.is_open())
		        throw "Translator: Couldn't open file '" + file + "'";
	        while (!input_output.eof())
	        {
		        std::string tmp;
		        std::getline(input_output, tmp);
		        out_file << tmp << "\n";
	        }

            first_line = false;
        }

// PROGRAMA NAO FUNCIONA QUANDO TEM MULTIPLOS STOPS!
//
// HOJE EU VOU LENDO ATE ENCONTRAR STOP,
// DEPOIS SUPONHO SECTION_DATA
//
// A UNICA FORMA DE EU SABER QUANDO COMECA SECTION_DATA
// EH VER O OPCODE DE STOP, CERTO?

        // Making decisions based on current state
        if (section_text)
        {
	        // Hooray, end of code!
	        if (chunk == OPCODE_STOP)
	        {
		        section_text = false;
		        // finishing text section
		        out_file << "\n\n"
		                 << "exit:\tmov eax, 1\n"
		                 << "\tmov ebx, 0\n"
		                 << "\tint 80h\n";

		        // alright, starting data section!
		        out_file << "\n"
		                 << "		section .data\n"
		                 << "\n"
		                 << "endl:				db  10      ; internal newline\n"
		                 << "buffer_size:		dw	256		; internal buffer size\n"
		                 << "buffer_max_size:	EQU	256		; internal buffer max size\n\n";

		        position_count++;
		        continue;
	        }

	        out_file << symbol_table.labelOf(position_count) << ":";
	        switch(chunk)
	        {
	        case OPCODE_ADD:
		        READ(chunk);
		        position_count++;
		        out_file << "add ax, WORD[" << symbol_table.labelOf(chunk) << "]";
		        break;

	        case OPCODE_SUB:
		        READ(chunk);
		        position_count++;
		        out_file << "sub ax, WORD[" << symbol_table.labelOf(chunk) << "]";
		        break;

	        case OPCODE_MULT:
		        READ(chunk);
		        position_count++;
		        out_file << "mul WORD[" << symbol_table.labelOf(chunk) << "]";
		        break;

	        case OPCODE_DIV:
		        READ(chunk);
		        position_count++;
		        out_file << "cwd\n"
		                 << "\tidiv WORD[" << symbol_table.labelOf(chunk) << "]";
		        break;

	        case OPCODE_JMP:
		        READ(chunk);
		        position_count++;
		        out_file << "jmp " << symbol_table.labelOf(chunk);
		        break;

	        case OPCODE_JMPN:
		        READ(chunk);
		        position_count++;
		        out_file << "mov bx, 0\n"
		                 << "\tcmp ax, bx\n"
		                 << "\tjl " << symbol_table.labelOf(chunk);
		        break;

	        case OPCODE_JMPP:
		        READ(chunk);
		        position_count++;
		        out_file << "mov bx, 0\n"
		                 << "\tcmp ax, bx\n"
		                 << "\tjg " << symbol_table.labelOf(chunk);
		        break;

	        case OPCODE_JMPZ:
		        READ(chunk);
		        position_count++;
		        out_file << "mov bx, 0\n"
		                 << "\tcmp ax, bx\n"
		                 << "\tje " << symbol_table.labelOf(chunk);
		        break;

	        case OPCODE_COPY:
		        {
		        // x86: mov A, B     =>   A = B
		        // OUR: COPY A, B    =>   B = A
		        READ(chunk);
		        position_count++;
		        int16_t tmp = chunk;

		        READ(chunk);
		        position_count++;
		        out_file << "mov bx, WORD[" << symbol_table.labelOf(tmp) << "]\n";
		        out_file << "\tmov [" << symbol_table.labelOf(chunk) << "], bx";
	        	}
		        break;

	        case OPCODE_LOAD:
		        READ(chunk);
		        position_count++;
		        out_file << "mov ax, WORD[" << symbol_table.labelOf(chunk) << "]";
		        break;

	        case OPCODE_STORE:
		        READ(chunk);
		        position_count++;
		        out_file << "mov WORD[" << symbol_table.labelOf(chunk) << "], ax";
		        break;

	        case OPCODE_INPUT:
		        READ(chunk);
		        position_count++;
		        out_file << "call read_int\n"
		                 << "\tmov WORD[" << symbol_table.labelOf(chunk) << "], si";
		        break;

	        case OPCODE_OUTPUT:
		        READ(chunk);
		        position_count++;
		        out_file << "mov si, [" << symbol_table.labelOf(chunk) << "]\n"
		                 << "call show_int";
		        break;
	        }

	        out_file << "\n";
        }
        else // section_data
        {
	        // CONVERT NUMBER IN CASE OF CONST
	        out_file << symbol_table.labelOf(position_count) << ":";
	        out_file << "dw " << chunk;
	        out_file << "\n";
        }
        position_count++;
    }
    file.close();
}

