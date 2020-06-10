#include <iostream>
#include <sstream>
#include <map>
#include "instruction_ids.h"
#include "executor.h"

/*
   return number meanings

   0 - ok
   1 - invalid instructions in .data
   2 - variable decleared during .main
   3 - illegal instructions (outisde .main or .data)
   4 - invalid arguments for an instruction
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

std::map<std::string, std::string> vars;

std::vector<double> memory;
double registers[10]{0};
unsigned int pointer = 0;
unsigned int flag = 0;
int runFlag = -1;

void math(int opCode) {

}

void jump(int opCode) {

}

void bitwise(int opCode) {

}

void memmanip(int opCode) {

}

int exec(std::vector<Instruction> inslist, bool strict = true) {

   for (size_t i = 0; i < inslist.size(); i++) {
      const int ins = inslist[i].type;

      if (ins == i_FLG_1 || ins == i_FLG_2) {
         runFlag = (i_FLG_2 - ins);
         //std::cout << "Type: " << inslist[i].type << "   FlagCode: " << runFlag << std::endl;
         continue;
      }

      // data
      if (runFlag == 1) {
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
         if (ins == i_VAR && strict) {
            std::cerr << "[Runtime Error] Variable declaration during .main flag illegal in strict mode.\n";
            return 2;
         }
         if (ins == i_li) {
            if (inslist[i].args.size() != 1 || inslist[i].arg_types[0] != v_NUM) {
               std::cerr << "[Runtime Error] Instruction li expects 1 argument of type 'NUM'.\n";  
               return 4;
            }

            flag = std::stoi(inslist[i].args[0]);
         }
         continue;
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