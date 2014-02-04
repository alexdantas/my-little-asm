#ifndef LEXICALANALYSER_H_DEFINED
#define LEXICALANALYSER_H_DEFINED

#include <string>
#include <sstream>

#include <LanguageDefinitions.hpp>

/// Handles raw file data and retrieval of tokens.
///
/// It contains the whole code in memory plus the rules on
/// how to extract tokens from it.
///
/// ## Usage
///
///     LexicalAnalyser lex("file.s");
///     while (true)
///     {
///         if (lex.reachedEof) break;
///         std::cout << lex.getToken() << " ";
///     }
///     std::cout << std::endl;
///
class LexicalAnalyser
{
public:
    /// Opens and parses *filename*.
    /// @note This will store the whole *filename* in memory,
    ///       ignoring comments. Watch out for a huge file!
    LexicalAnalyser(std::string filename);

    /// Returns the next token from the file.
    /// @note It ignores comments and newlines, mantaining the
    ///       consistent line numbering.
    /// @return A token string or an empty string when reached
    ///         end-of-file.
    /// @bug If you don't remove comments of the file previously,
    ///      this method won't know and will keep spitting tokens.
    ///      That's why our constructor removes the comments
    ///      when initializing.
    std::string getToken();

    /// Returns the current line count of the file.
    /// @note It grows with each call to *getToken()*.
    int lineCount();

    /// Tells if we've reached end-of-file when parsing with
    /// *getToken()*.
    /// @note Remember that *getToken()* returns an empty
    ///       string when reaching the end-of-file.
    bool reachedEOF();

    /// Removes comments from a *line*.
    /// @note *getToken()* deals with comments on it's own,
    ///       no need to call this explicitly (unless you
    ///       know what you're doing).
    std::string removeComments(std::string line);

    void reset();

private:
    /// Current line count of the file (according to *getToken()*)
    int line_count;

    /// All of the file's contents in memory.
    std::stringstream stream;

    std::stringstream backupStream;

    /// Flag to tell if we've reached end-of-file with *getToken()*
    bool end_of_file;
};

#endif //LEXICALANALYSER_H_DEFINED

