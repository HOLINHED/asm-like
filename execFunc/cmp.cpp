#include "../executor.h"

void setCmpRes(short r) {
   if (pointer == REG_PTR) {
      pointer = r;
   } else if (pointer == REG_FLG) {
      flag = r;
   } else if (pointer < 0) {
      registers_f[(pointer * -1) - 1] = r;
   } else {
      registers[pointer] = r;
   }
}

int compare(const Instruction& ins) {

   if (ins.args.size() != 2) {
      return INVALID_ARGS;
   }

   std::cout << "HELLO, CMP\n";

   if (ins.args[0] == ins.args[1]) {
      setCmpRes(0);
      return 0;
   }

   long double lhs;
   long double rhs;

   return 0;
}