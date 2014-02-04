# my-little-asm minimal Makefile
#
# System Software Assignment (Universidade de Brasilia 2/2013)

CFLAGS  = -Wall -Wextra -g -I"src/"
LDFLAGS =

# All source files
ASSEMBLER_FILES  = $(shell find src/Assembler/ -type f -iname '*.cpp')
SIMULATOR_FILES  = $(shell find src/Simulator/ -type f -iname '*.cpp')
TRANSLATOR_FILES = $(shell find src/Translator/ -type f -iname '*.cpp')
MISC_FILES       = $(shell find src/Misc -maxdepth 1 -type f -iname '*.cpp')

all: assembler simulator translator

assembler:
	g++ $(CFLAGS) $(ASSEMBLER_FILES) $(MISC_FILES) -o assembler

simulator:
	g++ $(CFLAGS) $(SIMULATOR_FILES) $(MISC_FILES) -o simulator

translator:
	g++ $(CFLAGS) $(TRANSLATOR_FILES) $(MISC_FILES) -o translator

clean:
	rm -f assembler simulator translator

