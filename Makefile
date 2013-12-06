# my-little-asm minimal Makefile
#
# Basic Software (2/2013) Assignment
# Alexandre Dantas (10/0090788)

CFLAGS  = -Wall -Wextra -g
LDFLAGS =

# All source files
ASSEMBLER_FILES = src/Assembler.cpp           \
                  src/LexicalAnalyser.cpp     \
                  src/SintacticalAnalyser.cpp \
                  src/assembler_main.cpp      \
                  src/SymbolTable.cpp         \
                  src/DerivationTable.cpp     \
                  src/Log.cpp                 \
                  src/utils.cpp

SIMULATOR_FILES = src/Simulator.cpp           \
                  src/InstructionTable.cpp

all: assembler simulator

assembler: $(ASSEMBLER_FILES)
	g++ $(CFLAGS) $(ASSEMBLER_FILES) -o assembler

simulator: $(SIMULATOR_FILES)
	g++ $(CFLAGS) $< -o simulator

clean:
	rm -f assembler simulator

