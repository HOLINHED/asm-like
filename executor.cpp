#include <iostream>
#include <sstream>
#include <map>
#include <any>
#include "instruction_ids.h"
#include "executor.h"

// Error Codes 
#define INVALID_INS_DAT 1    // invalid instructions in .data
#define VAR_IN_MAIN 2        // variable decleared during .main
#define ILLEGAL_INS 3        // illegal instructions (outside .main or .data) 
#define INVALID_ARGS 4       // invalid arguments for instruction
#define REG_DNE 5            // register does not exist.
#define REDEC_ERR 6          // variable already declared

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

struct MemoryUnit {
   void* value;
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

std::vector<MemoryUnit> memory;
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

int mov(const Instruction& ins) {
   return 0;
}

int rcopy(const Instruction& ins) {
   return 0;
}

int compare(const Instruction& ins) {
   return 0;
}

int syscll() {
   return 0;
}

int var(const Instruction& ins) {
   if (ins.args.size() != 1) {
      return INVALID_ARGS;
   }

   if (vars.find(ins.uid) != vars.end()) {
      vars[ins.uid].value = ins.args[0];
      vars[ins.uid].type = ins.arg_types[0];

      //std::cout << "REDEC VAR\n";
      std::cout << vars[ins.uid].value << std::endl;
      std::cout << vars[ins.uid].type << std::endl;
      return 0;
   }

   VarCont toinsert;
   toinsert.value = ins.args[0];
   toinsert.type = ins.arg_types[0];

   vars[ins.uid] = toinsert;

   //std::cout << "-------\nname: " << ins.uid << std::endl;
   //std::cout << "value: " << toinsert.value << std::endl;
   //std::cout << "arg_type: " << toinsert.type << std::endl << "-------\n";

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
               return INVALID_ARGS;
            }
         }
         if (ins != i_VAR) {
            std::cerr << "[Runtime Error] Invalid instructions present during .data flag.\n";
            return INVALID_INS_DAT;
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
               return VAR_IN_MAIN;
            }
            const int r = var(inslist[i]);
            if (var(inslist[i]) != 0) {
               if (r == INVALID_ARGS) {
                  std::cerr << "[Runtime Error] Variable assignment requires 1 argument.\n";
               }
               return r;
            }
         }
         if (ins == i_li) {
            if (inslist[i].args.size() != 1 || inslist[i].arg_types[0] != v_NUM) {
               std::cerr << "[Runtime Error] Instruction li expects 1 argument of type 'NUM'.\n";  
               return INVALID_ARGS;
            }

            flag = std::stoi(inslist[i].args[0]);
         }
         if (ins == i_la) {
            if (inslist[i].args.size() != 1 || inslist[i].arg_types[0] != v_REG) {
               std::cerr << "[Runtime Error] Instruction la expects 1 argument of type 'REG'.\n";  
               return INVALID_ARGS;
            }

            const int rnum = getRegId(inslist[i].args[0]);

            if (rnum == -3) {
               std::cerr << "[Runtime Error] Register \"" << inslist[i].args[0] << "\" does not exist.\n";
               return REG_DNE;
            }

            pointer = rnum;
         }

         continue;//end .main flag loop
      }

      // invalid, instructions outside of either flag.
      if (runFlag < 0) {
         std::cerr << "[Runtime Error] Instructions present outside of .data and/or .main.\n";
         return ILLEGAL_INS;
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