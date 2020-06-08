#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

int main() {
   const static std::string INSTRUCTION_IDS[] = 
   {
      "li",
      "la",
      "mov",
      "syscall",
      "add",
      "sub",
      "div",
      "mod",
      "copy",
      "cmp", 
      "jeq",
      "jne",
      "jae",
      "jue",
      "ja",
      "ju",
      "jmp",
      "mmov",
      "mret",
      "mcpy",
      "band",
      "bor",
      "bsl",
      "bsr",
      "LBL",
      "VAR",
      "FLG_1",
      "FLG_2",
   };

   const static size_t INSTRUCTION_SIZE = sizeof(INSTRUCTION_IDS) / sizeof(INSTRUCTION_IDS[0]);

   std::stringstream strm;
   strm << "#ifndef DK9H_INS\n";
   strm << "#define DK9H_INS\n";

   for (size_t i = 0; i < INSTRUCTION_SIZE; i++) {
      strm << "#define h_" << INSTRUCTION_IDS[i] << " " << i << "\n";
   }

   strm << "#endif";

   std::ofstream file("instruction_ids.h");

   if (!file) {
      std::cerr << "FILE FAILED TO OPEN\n";
      exit(1);
   }

   file << strm.str();
   file.close();

   return 0;
}