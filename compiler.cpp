#include <iostream>
#include <sstream> 
#include <fstream>
#include "lexer.h"
#include "parser.h"
#define DK9CFLAG 1
#include "executor.h"

int main(int argc, char** argv) {

   if (argc < 2) {
      std::cerr << "Usage: " << argv[0] << " <filename>\n";
      exit(1);
   }

   if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "-help") {
      std::cout << "Usage: " << argv[0] << " <filename> <?args>\n\n"
                << "-help (-h) Displays help for program.\n"
                << "-dump (-d) Dumps values of each register into a file.\n"
                << "-run (-r) Executes the program after compiling.\n"
                ;
      exit(0);   
   }

   bool dump = false;
   bool run = false;

   if (argc >= 3) {
      for (int i = 2; i < argc; i++) {
         std::string arg(argv[i]);

         if (arg == "-dump" || arg == "-d") {
            dump = true;
         } 
         else if (arg == "-run" || arg == "-r") {
            run = true;
         } else {
            std::cerr << "Unrecognized argument \"" << arg << "\"\n";
            exit(1);
         }
      }
   }

   std::ifstream file(argv[1]);

   if (!file) {
      std::cerr << "Error loading file \"" << argv[1] << "\".\n";
      exit(1);
   }

   std::stringstream* buffer = new std::stringstream;
   *buffer << file.rdbuf();

   file.close();

   auto lexresult = lex(buffer);
   
   auto parseresult = parse(lexresult);

   if (run) {
      std::cout << "This feature has not been implemented yet.\n";
   }

   if (dump) {
      std::stringstream finaldout;

      finaldout << "======= LEXER RESULT =======\n";
      for (auto y : lexresult) {
         finaldout << "[ ";
         for (std::string x : y) {
            finaldout << x << ", ";
         }
         finaldout << "]\n";
      }

      finaldout << "\n ======= PARSER RESULT =======\n";

      for (auto toPush : parseresult) {
         finaldout << "------------------------\n"
            << "Type: " << toPush.type << "\n"
            << "size: " << toPush.size << "\n"
            << "uid: " << toPush.uid << "\n"
            << "jmp_index: " << toPush.jmp_index << "\n"
            << "args: [";

         for (std::string piss : toPush.args) {
            finaldout << piss << ", ";
         }
         finaldout << "]\narg_types: [";

         for (int a : toPush.arg_types) {
            finaldout << a << ", ";
         }
         finaldout << "]\n";
      }

      finaldout << "------------------------\n";

      std::ofstream dumpFile;

      dumpFile.open("FINAL_DUMP.txt");

      if (!dumpFile) {
         std::cerr << "ERROR GENERATING DUMP FILE\n";
         exit(1);
      }

      dumpFile << finaldout.str();
      dumpFile.close();
   }

   delete buffer;

   return 0;
}