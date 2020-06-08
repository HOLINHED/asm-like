#include "parser.h"
#include <iostream>

#define INS_ARR INSTRUCTION_IDS
#define INS_SIZE INSTRUCTION_SIZE

int getInsType(std::string str) {

   std::string search = str;

   if (str[0] == ':') search = "LBL";
   if (str == ".data") search = "FLG_1";
   if (str == ".main") search = "FLG_2";
   if (str[str.size() - 1] == ':') search = "VAR";

   for (int i = 0; i < INS_SIZE; i++) {
      if (INS_ARR[i] == search) return i;
   }

   return -1;
}

std::vector<Instruction> parse(std::vector<std::vector<std::string>> adt) {

   std::vector<Instruction> result;

   size_t ln = 1;
   for (std::vector<std::string> line : adt) {
      Instruction cins;
      const int ins = getInsType(line[0]);

      if (ins == -1) {
         std::cerr << "[Parse Error] Instruction \"" << line[0] << "\" undefined on line " << ln << ".\n";
         exit(1);
      }

      cins.type = ins;

      if (ins == h_VAR) {
         if (line.size() != 2) {
            std::cerr << "[Parse Error] variable name expected 1 argument. On line " << ln << std::endl;
            exit(1);
         }

         cins.size = line[1].size();

         if (line[0] == ":") {
            std::cerr << "[Parse Error] Variable does not have a name. On line " << ln << std::endl;
            exit(1); 
         }

         cins.uid = line[0].substr(0, line[0].size() - 1);

         line.erase(line.begin());

         cins.args = line;

         std::cout << "VARIABLE DATA\n\n"
                   << "Type: " << cins.type << "\n"
                   << "size: " << cins.size << "\n"
                   << "uid: "  << cins.uid << "\n"
                   << "args: " << cins.args[0] << "\n";
      }
      //std::cout << "FOUND INSTRUCTION " << ins << std::endl;

      ln++;
   }

   return result;
}