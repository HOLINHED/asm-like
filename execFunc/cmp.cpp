#include "../executor.h"

int compare(const Instruction& ins) {

   if (ins.args.size() != 2) {
      return INVALID_ARGS;
   }

   if (ins.args[0] == ins.args[1]) {
      setRegData<short>(REG_PTR, 0);
      return 0;
   }

   long double lhs = evalData(ins.args[0], ins.arg_types[0]);
   long double rhs = evalData(ins.args[1], ins.arg_types[1]);

   if (lhs < rhs) setRegData<short>(REG_PTR, -1);
   if (lhs == rhs) setRegData<short>(REG_PTR, 0);
   if (lhs > rhs) setRegData<short>(REG_PTR, 1);

   return 0;
}