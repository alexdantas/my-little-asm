// System Software Assignment (Universidade de Brasilia 2/2013)

#include <iostream>
#include <cstdlib>              // exit()
#include <cstring>

#include <Translator/Translator.hpp>
#include <Misc/Log.hpp>

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		Log::error("translator: no input files\n"
		           "\n"
		           "Usage:\n"
		           "    $ translator (input asm file) [-d]\n"
		           "\n"
		           "-d     debug mode");

		exit(666);
	}

	// -d always at the end debugs
	if (argc == 3)
		if (std::string(argv[2]) == std::string("-d"))
			Log::debugMode(true);

	try
	{
		Translator translator(argv[1]);

		translator.translate();
	}
	catch (const char* error)
	{
		throw std::string(error);
	}
	catch (std::string error)
	{
		Log::error("Exception caught: " + error);
		return 666;
	}
	return 0;
}

