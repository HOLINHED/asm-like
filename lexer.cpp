#include "lexer.h"

#include <iostream>

const char delimiter = ' ';
const char arg_delimiter = ',';

std::string trim(std::string str) {
   return std::regex_replace(str, std::regex("^ +| +$|( ) +"), "$1");
}

std::vector<std::string> lexline(std::string line, const size_t l = 1) {
   std::vector<std::string> result;

   //std::replace(line.begin(), line.end(), ',', ' ');
   //std::cout << line << std::endl;

   std::string toProcess = line;
   std::string* strToken = nullptr;

   //TODO: Make it so it can tokenize multiple strings.
   if (line.find('"') != std::string::npos) {

      const size_t start = line.find_first_of('"');
      const size_t last = line.find_last_of('"');

      if (start == last || start == std::string::npos || last == std::string::npos) {
         std::cerr << "[LEX ERROR] Invalid string on line " << l << std::endl
                   << ":: " << l << "| " << line << std::endl;
         exit(1);
      }

      strToken = new std::string(line.substr(start, last - start + 1));      

      //std::cout << *strToken << std::endl;

      toProcess = line.substr(0, start) + " CC_INST_STR " + line.substr(last + 1);
   }

   //std::cout << toProcess << std::endl;
   std::replace(toProcess.begin(), toProcess.end(), arg_delimiter, ' ');

   std::stringstream strm(toProcess);
   std::string token;

   while(std::getline(strm, token, delimiter)) {
      if (!token.empty()) {
         if (token == "CC_INST_STR" && strToken != nullptr) {
            result.push_back(*strToken);
            continue;
         }
         result.push_back(token);
      }
   }

   delete strToken;

   return result;
}

std::vector<std::vector<std::string>> lex(std::stringstream *content) {
   std::vector<std::vector<std::string>> result;

    std::string line;
    size_t ln = 1;
    while (std::getline(*content, line))  {
      line = std::regex_replace(line, std::regex(";[]*?.*"), "$1"); // ignore comments
      line = trim(line); // ignore whitespace
      if (!line.empty()) {
         //std::cout << line << std::endl;
         auto res = lexline(line, ln);
         result.push_back(res);
      }
      ln++;
    }

   return result;
}