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

   if (ins.args[0] == ins.args[1]) {
      setCmpRes(0);
      return 0;
   }

   long double lhs = evalData(ins.args[0], ins.arg_types[0]);
   long double rhs = evalData(ins.args[1], ins.arg_types[1]);

   std::cout << "lhs: " << lhs << std::endl;
   std::cout << "rhs: " << rhs << std::endl;

   if (lhs < rhs) setCmpRes(-1);
   if (lhs == rhs) setCmpRes(0);
   if (lhs > rhs) setCmpRes(1);

   return 0;
}