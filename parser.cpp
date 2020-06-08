#include "parser.h"
#include <iostream>
#include <map>

#define INS_ARR INSTRUCTION_IDS
#define INS_SIZE INSTRUCTION_SIZE

bool isNum(std::string str) {
   for (char c : str) {
      if (!std::isdigit(c)) return false;
   }
   return true;
}

std::vector<int> getArgTypeList(std::vector<std::string> args) {
   std::vector<int> argtypes;
      for (std::string arg : args) {
         if (isNum(arg)) {
            argtypes.push_back(v_NUM);
            continue;
         }
         int argid = v_DAT;
         switch(arg[0]) {
            case '"': argid = v_STR; break;
            case '$': argid = v_REG; break;
            case '#': argid = v_VAR; break;
         }
         argtypes.push_back(argid);
   }
   return argtypes;
}

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

std::map<std::string, int> labelList;

Instruction parseVar(std::vector<std::string> line, Instruction& result, size_t ln = 1) {
   if (line.size() != 2) {
      std::cerr << "[Parse Error] variable name expected 1 argument. On line " << ln << std::endl;
      exit(1);
   }

   result.size = line[1].size();

   if (line[0] == ":") {
      std::cerr << "[Parse Error] Variable does not have a name. On line " << ln << std::endl;
      exit(1); 
   }

   result.uid = line[0].substr(0, line[0].size() - 1);

   line.erase(line.begin());

   result.args = line;

   result.arg_types = getArgTypeList(result.args);

   return result;
}

std::vector<Instruction> parse(std::vector<std::vector<std::string>> adt) {

   std::vector<Instruction> result;

   size_t ln = 1;
   for (std::vector<std::string> line : adt) {
      const int ins = getInsType(line[0]);

      if (ins == -1) {
         std::cerr << "[Parse Error] Instruction \"" << line[0] << "\" undefined on line " << ln << ".\n";
         exit(1);
      }

      Instruction toPush;

      toPush.type = ins;

      if (ins == i_VAR) parseVar(line, toPush, ln);
      if (ins == i_LBL) parseLabel(line, toPush, ln);
      //std::cout << "FOUND INSTRUCTION " << ins << std::endl;

      result.push_back(toPush);

      //PRINT INSTRUCTION 
      std::cout << "------------------------\n"
                << "Type: " << toPush.type << "\n"
                << "size: " << toPush.size << "\n"
                << "uid: " << toPush.uid << "\n"
                << "jmp_index: " << toPush.jmp_index << "\n"
                << "args: [";

      for (std::string piss : toPush.args) {
         std::cout << piss << ", ";
      }
      std::cout << "]\narg_types: [";

      for (int shit : toPush.arg_types) {
         std::cout << shit << ", ";
      }
      std::cout << "]\n";

      ln++;
   }

   return result;
}