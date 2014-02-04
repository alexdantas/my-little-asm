#include <cstdlib> // atoi()
#include <sstream>

#include <Translator/ReverseSymbolTable.hpp>
#include <Misc/Utils.hpp>

void ReverseSymbolTable::add(int index)
{
	// labels will be like "label_n" where n is
	// now many labels do we have until now.
	std::string label("LABEL_" + intToString(symbols.size()));


	Symbol s(index, label);
	symbols.push_back(s);
}
bool ReverseSymbolTable::has(int index)
{
	for (unsigned int i = 0; i < symbols.size(); i++)
		if (symbols[i].index == index)
			return true;

	return false;
}
std::string ReverseSymbolTable::labelOf(int index)
{
	for (unsigned int i = 0; i < symbols.size(); i++)
		if (symbols[i].index == index)
			return symbols[i].label;

	throw ("ReverseSymbolTable::labelOf Can't find label for " +
	       intToString(index));

	return "";
}

