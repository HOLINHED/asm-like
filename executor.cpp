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

void bShift(int opCode) {

}


int exec(std::vector<Instruction> inslist) {

   for (size_t i = 0; i < inslist.size(); i++) {

      

   }

   return 0;
}

#define DK9CFLAG 

#ifndef DK9CFLAG
int main(int argc, char** argv) {


   return 0;
}

#endif