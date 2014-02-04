#ifndef REVERSESYMBOLTABLE_H_DEFINED
#define REVERSESYMBOLTABLE_H_DEFINED

#include <string>
#include <vector>

/// An arbitrary `label` for an `index` on
/// the binary file.
///
struct Symbol
{
	Symbol(int index, std::string label):
		index(index), label(label)
	{ };

	int index;
	std::string label;
};

/// Contains all the indexes to the binary file,
/// along with arbitrary labels to them.
///
class ReverseSymbolTable
{
public:
	void add(int index);
	bool has(int index);
	std::string labelOf(int index);

private:
	/// TODO maybe use std::set to avoid duplicates and
	///      enjoy it's native `find` member.
	std::vector<Symbol> symbols;
};

#endif //REVERSESYMBOLTABLE_H_DEFINED

