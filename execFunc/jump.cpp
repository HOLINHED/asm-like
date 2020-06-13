#include "../executor.h"

int getRegValue() {
   if (pointer == REG_PTR) return pointer;
   if (pointer == REG_FLG) return flag;
   if (pointer < 0) {
      return static_cast<int>(registers_f[(pointer * -1) - 1]);
   } else {
      return registers[pointer];
   }
}

bool jump(const Instruction& ins) {
   
   bool doJump = false;

   switch(ins.type) {
      case i_jeq: {
         doJump = getRegValue() == 0;
      }; break;
      case i_jne: {
         doJump = getRegValue() != 0;
      }; break;
      case i_jae: {
         doJump = getRegValue() >= 0;
      }; break;
      case i_jue: {
         doJump = getRegValue() <= 0;
      }; break;
      case i_ja: {
         doJump = getRegValue() > 0;
      }; break;
      case i_ju: {
         doJump = getRegValue() < 0;
      }; break;
      case i_jmp: {
         doJump = true;
      }; break;
   }

   return doJump;
}