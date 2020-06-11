#include "../executor.h"

int rcopy(const Instruction& ins) {
   // r1 -> r2
   if (ins.args.size() != 2) return 1;
   if (ins.arg_types[0] != v_REG || ins.arg_types[1] != v_REG) return 1;

   int r1 = getRegId(ins.args[0]);
   int r2 = getRegId(ins.args[1]);

   if (r1 == REG_ERR || r2 == REG_ERR) return 5;

   if (r1 == REG_PTR) r1 = pointer;
   if (r2 == REG_PTR) r2 = pointer;

   if (r1 == r2) return 0;

   //flag lhs
   if (r1 == REG_FLG) {
      if (r2 < 0) {
         r2 = (r2 * -1) - 1;
         registers_f[r2] = flag;
         return 0;
      }
      registers[r2] = flag;
      return 0;
   }
   //flag rhs
   if (r2 == REG_FLG) {
      if (r1 < 0) {
         r1 = (r1 * -1) - 1;
         flag = registers_f[r1];
         return 0;
      }
      flag = registers[r1];
      return 0;
   }

   // 2 floating point registers
   if (r1 < 0 && r2 < 0) {
      r1 = (r1 * -1) - 1;
      r2 = (r2 * -1) - 1;
      registers_f[r2] = registers_f[r1];
      return 0;
   }

   // 2 int registers
   if (r1 >= 0 && r2 >= 0) {
      registers[r2] = registers[r1];
      return 0;
   } 
   
   // r1 = integer reg, r2 = floating 
   if (r1 >= 0 && r2 < 0) {
      r2 = (r2 * -1) - 1;
      registers_f[r2] = registers[r1];
      return 0;
   }

   // r1 = floating reg, r2 = integer 
   if (r1 < 0 && r2 >= 0) {
      r1 = (r1 * -1) - 1;
      registers[r2] = registers_f[r1];
      return 0;
   }

   return 0;
}