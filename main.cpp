#include <iostream>
#include <sstream>
#include <fstream>
#include "lexer.h"
#include "parser.h"
#include "executor.h"
#include "serialization.h"

int main(int argc, char** argv) {

   if (argc < 2) {
      std::cerr << "Usage: " << argv[0] << " <filename>\n";
      exit(1);
   }

   if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "-help") {
      std::cout << "Usage: " << argv[0] << " <filename> <?args>\n\n"
                << "-help (-h) Displays help for program.\n"
                << "-dump (-d) Dumps debug logs into file.\n"
                << "-run (-r) Executes the program after compiling.\n"
                << "-nostrict (-s) Turns off strict mode during runtime.\n"
                << "-runfile (-rf) Runs program from a precompiled file.\n"
                << "-outfile (-o) Sets output file for compiled code.\n"
                ;
      exit(0);   
   }

   bool dump = false;
   bool run = false;
   bool strict = true;
   bool runfile = false;
   std::string compFileName = "out.nsn.o";

   if (argc >= 3) {
      for (int i = 2; i < argc; i++) {
         std::string arg(argv[i]);

         if (arg == "-dump" || arg == "-d") {
            dump = true;
         } else if (arg == "-run" || arg == "-r") {
            if (runfile) {
               std::cerr << "Invaild arguments. -r and -rf cannot both be true.\n";
               exit(1);
            }
            run = true;
         } else if (arg == "-nostrict" || arg == "-s") {
            strict = false;
         } else if (arg == "-runfile" || arg == "-rf") {
            if (run) {
               std::cerr << "Invaild arguments. -r and -rf cannot both be true.\n";
               exit(1);
            }
            runfile = true;
         } else if (arg == "-outfile" || arg == "-o") {
            if (run || runfile) {
               std::cerr << "Invalid arguments. -o and -r/-rf cannot both be true.\n";
               exit(1);
            }
            if (i + 1 == argc) {
               std::cerr << "Invalid arguments. -o expects a filename.\n";
               exit(1);
            } 
            compFileName = std::string(argv[i + 1]);
            i += 1;
         }
         else {
            std::cerr << "Unrecognized argument \"" << arg << "\"\n";
            exit(1);
         }
      }
   }

   std::ifstream file(argv[1]);
   std::stringstream* buffer = new std::stringstream;
   *buffer << file.rdbuf();

   if (!file) {
      std::cerr << "Error loading file \"" << argv[1] << "\".\n";
      exit(1);
   }

   if (!runfile) {

      file.close();

      auto lexresult = lex(buffer);
      
      auto parseresult = parse(lexresult);

      if (run) {
         int rcode = exec(parseresult, strict);
         std::cout << "\nExited with code " << rcode << std::endl;
      } else {
         //compile to source
         serialize(parseresult, compFileName);
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

      std::cout << "==> Dump File Created Successfully!\n";
      if (run) saveDebugLog();
   }

   } else {
      // exec serialized file
      auto parsed = deserialize(buffer);

      exec(parsed, strict);

      if (dump) saveDebugLog();
   }

   delete buffer;

   return 0;
}