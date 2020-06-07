#ifndef H_DK9LEXER
#define H_DK9LEXER

#include <vector>
#include <string>
#include <sstream>
#include <regex>

static char delimiter = ' ';
static char arg_delimiter = ',';

std::string trim(std::string);

std::vector<std::string> lexline(std::string, const size_t);

// vector of vectors of strings
std::vector<std::vector<std::string>> lex(std::stringstream*);

#endif