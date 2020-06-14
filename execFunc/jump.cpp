#include "../executor.h"

bool jump(const Instruction& ins) {
   
   bool doJump = false;

   switch(ins.type) {
      case i_jeq: {
         doJump = getRegData(pointer) == 0;
      }; break;
      case i_jne: {
         doJump = getRegData(pointer) != 0;
      }; break;
      case i_jae: {
         doJump = getRegData(pointer) >= 0;
      }; break;
      case i_jue: {
         doJump = getRegData(pointer) <= 0;
      }; break;
      case i_ja: {
         doJump = getRegData(pointer) > 0;
      }; break;
      case i_ju: {
         doJump = getRegData(pointer) < 0;
      }; break;
      case i_jmp: {
         doJump = true;
      }; break;
   }

   return doJump;
}