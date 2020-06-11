#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include "ins_map.h"

int main() {

   const std::string ARG_TYPES[] = 
   {
      "STR",
      "NUM",
      "DAT",
      "REG",
      "VAR",
      "LBL",
      "MEM",
      "MEMPTR",
      "NUM_F",
   };

   //const size_t ARG_LSIZE = 9;
   const size_t ARG_LSIZE = sizeof(ARG_TYPES) / sizeof(ARG_TYPES[0]);

   const size_t INSTRUCTION_SIZE = sizeof(INSTRUCTION_IDS) / sizeof(INSTRUCTION_IDS[0]);

   std::stringstream strm;
   strm << "#ifndef DK9H_INS\n";
   strm << "#define DK9H_INS\n";

   size_t i = 0;
   for (; i < INSTRUCTION_SIZE; i++) {
      strm << "#define i_" << INSTRUCTION_IDS[i] << " " << i << "\n";
   }

   for (size_t j = 0; j < ARG_LSIZE; j++) {
      strm << "#define v_" << ARG_TYPES[j] << " " << (10 * i + j) << "\n";
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