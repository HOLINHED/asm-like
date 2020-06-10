#include <iostream>
#include "instruction_ids.h"
#include "executor.h"

std::vector<double> memory;
double registers[10]{0};
unsigned int pointer = 0;
unsigned int flag = 0;
int runFlag = 0;

void math(int opCode) {

}

void jump(int opCode) {

}

void bitwise(int opCode) {

}


int exec(std::vector<Instruction> inslist) {

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
      }  

      //main
      if (runFlag == 0) {
         std::cout << "RUNTIME WOOOOO\n";
      }



   }

   return 0;
}

#define DK9CFLAG 

#ifndef DK9CFLAG
int main(int argc, char** argv) {


   return 0;
}

#endif