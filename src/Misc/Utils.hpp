// Lots of useful functions.

#ifndef UTILS_H_DEFINED
#define UTILS_H_DEFINED

#include <string>

/// Helper function that converts an `int` to a `std::string`.
std::string intToString(int num);

/// Converts a general string to an integer constant.
///
/// It allows several string formats:
///
/// * Decimal: "2", "-5", "122"
/// * Hex:     "0xFFB", -0x2f, "0X4"
/// * Binary:  "0b00011", "0B10010"
int stringToInt(std::string str);

/// Returns `filename` without extension.
///
/// If `filename` is a dotfile (starts with '.') or doesn't
/// have extension, returns empty string.
std::string basename(std::string filename);

#endif /* UTILS_H_DEFINED */

