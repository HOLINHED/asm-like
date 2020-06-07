#include <iostream>
#include <sstream> 
#include <fstream>
#include "lexer.h"
#include "parser.h"

int main(int argc, char** argv) {

   if (argc < 2) {
      std::cerr << "Usage: " << argv[0] << " <filename>\n";
      exit(1);
   }

   if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "-help") {
      std::cout << "Usage: " << argv[0] << " <filename> <?args>\n\n"
                << "-help (-h) Displays help for program.\n"
                << "-dump (-d) Dumps values of each register into a file.\n"
                ;
      exit(0);   
   }

   bool dump = false;

   if (argc >= 3) {
      for (size_t i = 2; i < argc; i++) {
         std::string arg(argv[i]);

         if (arg == "-dump" || arg == "-d") {
            dump = true;
         } else {
            std::cerr << "Unrecognized argument \"" << arg << "\"\n";
            exit(1);
         }
      }
   }

   std::ifstream file(argv[1]);

   if (!file) {
      std::cerr << "Error loading file \"" << argv[1] << "\"\n";
      exit(1);
   }

   std::stringstream* buffer = new std::stringstream;
   *buffer << file.rdbuf();

   file.close();

   auto lexresult = lex(buffer);

   for (auto y : lexresult) {
      std::cout << "[ ";
      for (std::string x : y) {
         std::cout << x << ", ";
      }
      std::cout << "]\n";
   }

   delete buffer;

   return 0;
}