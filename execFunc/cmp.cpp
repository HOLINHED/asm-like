#include "../executor.h"

int compare(const Instruction& ins) {

   if (ins.args.size() > 3) {
      return INVALID_ARGS;
   }

   if (ins.args.size() == 3 && ins.arg_types[2] != v_REG) {
      return INVALID_ARGS;
   }

   int regid = ins.args.size() == 3 ? getRegId(ins.args[2]) : REG_PTR;

   if (regid == REG_ERR) {
      return REG_DNE;
   }

   if (ins.args[0] == ins.args[1]) {
      setRegData<short>(regid, 0);
      return 0;
   }

   long double lhs = evalData(ins.args[0], ins.arg_types[0]);
   long double rhs = evalData(ins.args[1], ins.arg_types[1]);

   if (lhs < rhs) setRegData<short>(regid, -1);
   if (lhs == rhs) setRegData<short>(regid, 0);
   if (lhs > rhs) setRegData<short>(regid, 1);

   return 0;
}