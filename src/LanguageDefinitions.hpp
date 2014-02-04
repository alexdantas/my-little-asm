// Contains common definitions for our custom Assembly language
//

#ifndef LANG_DEFINITIONS_H_DEFINED
#define LANG_DEFINITIONS_H_DEFINED

// instructions OPCODE
#define MINIMUM_OPCODE 0
#define ADD    1
#define SUB    2
#define MULT   3
#define DIV    4
#define JMP    5
#define JMPN   6
#define JMPP   7
#define JMPZ   8
#define COPY   9
#define LOAD   10
#define STORE  11
#define INPUT  12
#define OUTPUT 13
#define STOP   14
#define MAXIMUM_OPCODE 15

// minor tokens
#define CHAR_COMMENT ';'
#define CHAR_LABEL   ':'

#define MAXIMUM_LABEL_SIZE 1001

// I need this duplicate information because of Translator.cpp
// and InstructionTable.cpp.
//
// TODO: Make a way to remove this enum in favor of previous
//       #defines!

// instructions OPCODE
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
	OPCODE_STOP
};

#endif // LANG_DEFINITIONS_H_DEFINED

