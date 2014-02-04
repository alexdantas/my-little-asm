#ifndef TRANSLATOR_H_DEFINED
#define TRANSLATOR_H_DEFINED

#include <string>

/// Translates between a binary of our custom Assembly language
/// into an IA-32 NASM compatible source code.
///
class Translator
{
public:
	/// Translates `filename` into `filename.s`.
	Translator(std::string filename);

	/// Does all the work, actually writing into `output_filename`.
	void translate();

private:
	std::string input_filename;
	std::string output_filename;
};

#endif //TRANSLATOR_H_DEFINED

