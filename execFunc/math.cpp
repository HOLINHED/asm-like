#include "../executor.h"

int math(const Instruction& ins) {

   if (ins.args.size() == 3 && ins.arg_types[2] != v_REG) return INVALID_ARGS;
   if (ins.args.size() > 3) return INVALID_ARGS;

   long double lhs = evalData<long double>(ins.args[0], ins.arg_types[0]);
   long double rhs = evalData<long double>(ins.args[1], ins.arg_types[1]);

   int regid = ins.args.size() == 3 ? getRegId(ins.args[2]) : REG_PTR;

   if (regid == REG_ERR) return REG_DNE;

   bool fpv = static_cast<long>(lhs) != lhs || static_cast<long>(rhs) != rhs;
   long double res;

   switch(ins.type) {
      case i_add: {
         res = lhs + rhs;
      }; break;
      case i_sub: {
         res = lhs - rhs;
      }; break;
      case i_mul: {
         res = lhs * rhs;
      }; break;
      case i_div: {
         res = lhs / rhs;
      }; break;
      case i_mod: {
         if (fpv) {
            return FLOAT_MOD_ERR;
         }
         res = static_cast<long>(lhs) % static_cast<long>(rhs);
      }; break;
            
   }

   fpv = static_cast<long>(res) != res;

   if (fpv) {
      setRegData<double>(regid, res);
   } else {
      setRegData<long>(regid, res);
   }

   return 0;
}