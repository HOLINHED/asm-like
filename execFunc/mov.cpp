#include "../executor.h"

int mov(const Instruction& ins) {
   
   if (ins.args.size() != 2 || ins.arg_types[1] != v_REG) return INVALID_ARGS;

   if (ins.args[0] == ins.args[1]) return 0;

   int r2 = getRegId(ins.args[1]);

   if (r2 == REG_ERR) return REG_DNE; //reg_dne
   
   if (r2 == REG_PTR) r2 = pointer;

   // REG, REG
   if (ins.arg_types[0] == v_REG) {
      //2 registers. can reuse copy function.
      return rcopy(ins);
   }

   if (ins.arg_types[0] == v_DAT) {
      if (ins.args[0] == "msize") {
         if (r2 == REG_FLG) {
            flag = memory.size();
         } else if (r2 < 0) {
            registers_f[(r2 * -1) - 1] = memory.size();
         } else {
            registers[r2] = memory.size();
         }
         return 0;
      } else return INVALID_ARGS;
   }

   //NUM/numf,char reg
   if (ins.arg_types[0] == v_NUM || ins.arg_types[0] == v_NUM_F || ins.arg_types[0] == v_CHR) {
      double num;

      if (ins.arg_types[0] == v_CHR) {
         num = ins.args[0][0];
      } else {
         num = std::stod(ins.args[0]);
      }

      if (r2 == REG_FLG) { //flag
         flag = num;
         return 0;
      }

      if (r2 < 0) { //float register
         registers_f[(r2 * -1) - 1] = num;
      } else { // int register
         registers[r2] = num;
      }

      return 0;
   }

   //str, reg
   if (ins.arg_types[0] == v_STR) {
      MemoryUnit toins;
      toins.value = ins.args[0];
      toins.type = v_STR;

      if (r2 == REG_FLG) {
         flag = memory.size();
      } else if (r2 < 0) {
         registers_f[(r2 * -1) - 1] = memory.size();
      } else {
         registers[r2] = memory.size();
      }

      memory.push_back(toins);
   }

   //VAR, reg
   if (ins.arg_types[0] == v_VAR) {
      std::string varname = ins.args[0].substr(1);
      if (vars.find(varname) != vars.end()) {
         const size_t cres = vars[varname].memaddr;
         if (r2 == REG_FLG) {
            flag = cres;
            return 0;
         }
         if (r2 < 0) {
            registers_f[(r2 * -1) - 1] = cres;
         } else {
            registers[r2] = cres;
         }
      } else {
         return VAR_NOT_FOUND;
      }
   }

   return 0;
}