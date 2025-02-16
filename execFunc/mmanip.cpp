#include "../executor.h"

int memmanip(const Instruction& ins) {

   switch(ins.type) {
      case i_mmov: {

         if (ins.args.size() != 2) return INVALID_ARGS;

         const size_t index = evalData<size_t>(ins.args[0], ins.arg_types[0]);

         if (index > memory.size() || index < 0) return INVALID_ADDRESS;

         MemoryUnit toInsert;

         if (ins.arg_types[1] == v_STR) {
            toInsert.value = ins.args[0];
            toInsert.type = v_STR;
            if (index == memory.size()) {
               memory.push_back(toInsert);
            } else {
               memory[index] = toInsert;
            }
            return 0;
         }

         const long double res = evalData(ins.args[0], ins.arg_types[0]);
         toInsert.value = std::to_string(res);

         if (static_cast<long>(res) == res) {
            toInsert.type = v_NUM;
         } else {
            toInsert.type = v_NUM_F;
         }

         if (index == memory.size()) {
            memory.push_back(toInsert);
         } else {
            memory[index] = toInsert;
         }
      }; break;
      case i_mret: {
         
         if (ins.args.size() > 2) return INVALID_ARGS;

         int regid = REG_PTR;

         if (ins.args.size() == 2) {
            if (ins.arg_types[1] != v_REG) return INVALID_ARGS;

            regid = getRegId(ins.args[1]);

            if (regid == REG_ERR) return REG_DNE;
         }

         const size_t mindex = evalData<size_t>(ins.args[0], ins.arg_types[0]);
         
         if (mindex < 0 || mindex >= memory.size()) return INVALID_ADDRESS;

         MemoryUnit ret = memory[mindex];

         const long double dat = evalData(ret.value, ret.type);

         setRegData(regid, dat);

      }; break;
      case i_mdel: {

         if (ins.args.size() != 1) return INVALID_ARGS;

         const size_t index = evalData<size_t>(ins.args[0], ins.arg_types[0]);

         if (index < 0 || index >= memory.size()) return INVALID_ADDRESS;

         if (index < vars.size()) {
            for (const auto &pair : vars) {
               if (vars[pair.first].memaddr == index) {
                  vars.erase(pair.first);
               }
            }
         }

         memory[index].value = "";
         memory[index].type = 0;
         memcontroller.push_back(index);

      }; break;
      case i_minx: {

         if (ins.args.size() > 3) return INVALID_ARGS;

         int regid = REG_PTR;

         if (ins.args.size() == 3) {
            if (ins.arg_types[2] != v_REG) return INVALID_ARGS;
            regid = getRegId(ins.args[2]);
            if (regid == REG_ERR) return REG_DNE;
         }

         const size_t lhs = evalData<size_t>(ins.args[0], ins.arg_types[0]);
         const size_t rhs = evalData<size_t>(ins.args[1], ins.arg_types[1]);

         MemoryUnit ret = memory[lhs];

         if (ret.type != v_STR) return INVALID_ADDRESS;

         if (rhs >= ret.value.size()) return INVALID_ADDRESS;

         setRegData<char>(regid, ret.value[rhs]);

      }; break;
      case i_mcpy: {
         if (ins.args.size() != 2) return INVALID_ARGS;

         const size_t lhs = evalData<size_t>(ins.args[0], ins.arg_types[0]);
         const size_t rhs = evalData<size_t>(ins.args[1], ins.arg_types[1]);

         if (lhs >= memory.size() || rhs >= memory.size() || lhs < 0 || rhs < 0) return INVALID_ADDRESS;

         memory[rhs] = memory[lhs];

      }; break;
   }

   return 0;
}