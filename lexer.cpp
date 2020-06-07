#include "lexer.h"

#include <iostream>

std::string trim(std::string str) {
   return std::regex_replace(str, std::regex("^ +| +$|( ) +"), "$1");
}

std::vector<std::string> lexline(std::string line) {
   std::vector<std::string> result;

   return result;
}

std::vector<std::vector<std::string>> lex(std::stringstream *content) {
   std::vector<std::vector<std::string>> result;

    std::string line;
    while (std::getline(*content, line))  {
      line = std::regex_replace(line, std::regex(";[]*?.*"), "$1"); // ignore comments
      line = trim(line); // ignore whitespace
      if (!line.empty()) {
         //std::cout << line << std::endl;
         result.push_back(lexline(line));
      }
    }

   return result;
}