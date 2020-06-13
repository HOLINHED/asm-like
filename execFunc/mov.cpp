#include "../executor.h"

int mov(const Instruction& ins) {

   if (ins.args.size() != 2 || ins.arg_types[1] != v_REG) return INVALID_ARGS;

   if (ins.args[0] == ins.args[1]) return 0;

   int r2 = getRegId(ins.args[1]);

   if (r2 == REG_ERR) return REG_DNE;

   if (ins.arg_types[0] == v_STR) {
      const size_t memaddr = pushToMem(ins.args[0], v_STR);
      setRegData<size_t>(r2, memaddr);
      return 0;
   }

   const long double dataToSet = evalData(ins.args[0], ins.arg_types[0]);

   setRegData<long double>(r2, dataToSet);

   return 0;
}