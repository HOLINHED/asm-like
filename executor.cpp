#include <iostream>
#include <sstream>
#include <map>
#include <any>
#include "instruction_ids.h"
#include "executor.h"

/*
   return number meanings

   0 - ok
   1 - invalid instructions in .data
   2 - variable decleared during .main
   3 - illegal instructions (outisde .main or .data)
   4 - invalid arguments for an instruction
   5 - register does not exist.
*/

// TEMP
std::string parseStr(const std::string& s) {
    std::stringstream ss{""};

    for(size_t i = 0; i < s.length(); i++) {
        if (s.at(i) == '\\') {
            switch(s.at(i + 1)) {
                case 'n':  ss << "\n"; i++; break;
                case '"':  ss << "\""; i++; break;
                default:   ss << "\\";      break;
            }       
        } else {
            ss << s.at(i);
        }
    }

    return ss.str();
}
// TEMP

struct VarCont {
   std::string value;
   int type;
};

int getRegId(std::string reg) {
   if (reg == "$A") return -2;
   if (reg == "$B") return -1;

   const int rnum = std::stoi(reg.substr(1));

   if (rnum < 0 || rnum > 9) {
      return -3; //-3 means register number is invalid
   }

   return rnum;
}

std::map<std::string, VarCont> vars;

std::vector<double> memory;
double registers[10]{0};
int pointer = 0;
unsigned int flag = 0;
int runFlag = -1;

int math(const Instruction& ins) {
   return 0;
}

int jump(int opCode) {
   return 0;
}

int bitwise(const Instruction& ins) {
   return 0;
}

int memmanip(const Instruction& ins) {
   return 0;
}

int var(const Instruction& ins) {
   if (ins.args.size() != 1) {
      return 1;
   }

   VarCont toinsert;
   toinsert.value = ins.args[0];
   toinsert.type = ins.arg_types[0];

   vars[ins.uid] = toinsert;

   std::cout << "name: " << ins.uid << std::endl;
   std::cout << "value: " << toinsert.value << std::endl;
   std::cout << "arg_type: " << toinsert.type << std::endl;

   return 0;
}

int exec(std::vector<Instruction> inslist, bool strict = true) {

   for (size_t i = 0; i < inslist.size(); i++) {
      //std::cout << "FLAG: " << flag << std::endl;
      //std::cout << "POINTER: " << pointer << std::endl;
      const int ins = inslist[i].type;

      if (ins == i_FLG_1 || ins == i_FLG_2) {
         runFlag = (i_FLG_2 - ins);
         //std::cout << "Type: " << inslist[i].type << "   FlagCode: " << runFlag << std::endl;
         continue;
      }

      // data
      if (runFlag == 1) {
         if (ins == i_VAR) {
            if (var(inslist[i]) == 1) {
               std::cerr << "[Runtime Error] Variable assignment requires 1 argument.\n";
               return 4;
            }
         }
         if (ins != i_VAR) {
            std::cerr << "[Runtime Error] Invalid instructions present during .data flag.\n";
            return 1;
         }
         continue;
      }  

      //main
      if (runFlag == 0) {
         if (ins == i_LBL) continue;
         if (ins == i_return) return 0;
         if (ins == i_VAR) {
            if (strict) {
               std::cerr << "[Runtime Error] Variable declaration during .main flag illegal in strict mode.\n";
               return 2;
            }
            if (var(inslist[i]) == 1) {
               std::cerr << "[Runtime Error] Variable assignment requires 1 argument.\n";
               return 4;
            }
         }
         if (ins == i_li) {
            if (inslist[i].args.size() != 1 || inslist[i].arg_types[0] != v_NUM) {
               std::cerr << "[Runtime Error] Instruction li expects 1 argument of type 'NUM'.\n";  
               return 4;
            }

            flag = std::stoi(inslist[i].args[0]);
         }
         if (ins == i_la) {
            if (inslist[i].args.size() != 1 || inslist[i].arg_types[0] != v_REG) {
               std::cerr << "[Runtime Error] Instruction la expects 1 argument of type 'REG'.\n";  
               return 4;
            }

            const int rnum = getRegId(inslist[i].args[0]);

            if (rnum == -3) {
               std::cerr << "[Runtime Error] Register \"" << inslist[i].args[0] << "\" does not exist.\n";
               return 5;
            }

            pointer = rnum;
         }

         continue;//end .main flag loop
      }

      // invalid, instructions outside of either flag.
      if (runFlag < 0) {
         std::cerr << "[Runtime Error] Instructions present outside of .data and/or .main.\n";
         return 3;
      }
      // end for loop
   }

   return 0;
}

#define DK9CFLAG 

#ifndef DK9CFLAG
int main(int argc, char** argv) {


   return 0;
}

#endif