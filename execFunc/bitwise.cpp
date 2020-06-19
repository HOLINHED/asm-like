#include "../executor.h"

int bitwise(const Instruction& ins) {

   if (ins.args.size() > 3) return INVALID_ARGS;

   if (ins.type == i_bnot) {
      if (ins.args.size() != 2) return INVALID_ARGS;

      int regid = REG_PTR;

      if (ins.arg_types[1] != v_REG) return INVALID_ARGS;
      regid = getRegId(ins.args[1]);
      if (regid == REG_ERR) return REG_DNE;

      const long dat = evalData<long>(ins.args[0], ins.arg_types[0]);

      setRegData(regid, ~dat);

      return 0;
   }

   int regid = REG_PTR;

   if (ins.args.size() == 3) {
      if (ins.arg_types[2] != v_REG) return INVALID_ARGS;
      regid = getRegId(ins.args[2]);
      if (regid == REG_ERR) return REG_DNE;
   }

   const long lhs = evalData<long>(ins.args[0], ins.arg_types[0]);
   const long rhs = evalData<long>(ins.args[1], ins.arg_types[1]);

   long result;

   switch(ins.type) {
      case i_band: {
         result = lhs & rhs;
      }; break;
      case i_bor: {
         result = lhs | rhs;
      }; break;
      case i_bxor: {
         result = lhs ^ rhs;
      }; break;
      case i_bsl: {
         result = lhs << rhs;
      }; break;
      case i_bsr: {
         result = lhs >> rhs;
      }; break;
   }

   setRegData(regid, result);

   return 0;
}