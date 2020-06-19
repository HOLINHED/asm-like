#include <iostream>
#include <sstream>
#include <map>
#include <time.h>
#include <fstream>
#include "instruction_ids.h"
#include "executor.h"
#include "serialization.h"

// Error Codes 
#define INVALID_INS_DAT 1    // invalid instructions in .data
#define VAR_IN_MAIN 2        // variable decleared during .main
#define ILLEGAL_INS 3        // illegal instructions (outside .main or .data) 
#define INVALID_ARGS 4       // invalid arguments for instruction
#define REG_DNE 5            // register does not exist.
#define REDEC_ERR 6          // variable already declared
#define VAR_NOT_FOUND 7      // variable not found
#define INVALID_FLAG 8       // instruction does not exist.
#define INVALID_ADDRESS 9    // memory address is invalid.
#define FLOAT_MOD_ERR 10     // 

std::map<std::string, VarCont> vars;
std::vector<MemoryUnit> memory;
std::vector<size_t> memcontroller;
long registers[10]{0};
double registers_f[5]{0};
int pointer = 0;
unsigned int flag = 0;
int runFlag = -1;

int getRegId(std::string reg) {
   if (reg == "$A") return REG_PTR;
   if (reg == "$B") return REG_FLG;

   bool freg = false;
   if (reg[1] == 'f') freg = true; 

   const int rnum = std::stoi(reg.substr(!freg ? 1 : 2));

   if (rnum < 0 || rnum > (freg ? 4 : 9)) {
      return REG_ERR; 
   }

   // negetive register means it's a floating point. (id * -1) - 1 to convert to index
   return (rnum * (freg ? -1 : 1)) - freg; 
}

template<class T = long>
void setRegData(int regid, T dat) {
   if (regid == REG_PTR) {
      setRegData<T>(pointer, dat);
   } else if (regid == REG_FLG) {
      flag = dat;
   } else if (regid < 0) {
      registers_f[(regid * -1) - 1] = dat;
   } else {
      registers[regid] = dat;
   }
}

template<class T = long>
T getRegData(int regid) {
   if (regid == REG_PTR) {
      return getRegData<T>(pointer);
   } else if (regid == REG_FLG) {
      return flag;
   } else if (regid < 0) {
      return registers_f[(regid * -1) - 1];
   } else {
      return registers[regid];
   }

   std::cerr << "[Runtime Error] Could not retrive register data from " << regid << std::endl;
   exit(1);
   return 0;
}

template<class T = std::string>
size_t pushToMem(T value, int type) {
   MemoryUnit toInsert;
   if (memcontroller.size() != 0) {
      const size_t iindex = memcontroller[memcontroller.size() - 1];
      toInsert.value = std::string(value);
      toInsert.type = type;
      memory[iindex] = toInsert;
      memcontroller.pop_back();
      return iindex;
   }
   toInsert.value = std::string(value);
   toInsert.type = type;
   memory.push_back(toInsert);
   return memory.size() - 1;
}

template<class T = long double>
T evalData(std::string value, int type) {
   if (type == v_MEM) {
      const std::string mv = value.substr(1);
      size_t addr;
      if (mv[0] == '$'){
         addr = evalData(mv, v_REG);
      } else if (mv[0] == '#') {
         addr = evalData(mv, v_VAR);
      } else if (mv[0] == '*') {
         addr = evalData(mv, v_MEM);
      } else if (mv == "msize" || mv == "mtop") {
         addr = evalData(mv, v_DAT);
      } else {
         addr = std::stoi(mv);
      } 

      if (addr >= memory.size()) {
         std::cout << "[Runtime Error] evalData: Failed to bind v_MEM '" << value << "' to a value.\n";
         exit(1);
      }

      return evalData(memory[addr].value, memory[addr].type);
   }
   if (type == v_DAT) {
      if (value == "msize") return memory.size();
      if (value == "mtop") return memory.size() - 1;
      else {
         std::cout << "[Runtime Error] evalData: Failed to bind v_DAT '" << value << "' to a value.\n";
         exit(1);
      }
   } else if (type == v_NUM) {
      return std::stol(value); 
   } else if (type == v_NUM_F) {
      return std::stod(value);
   } else if (type == v_STR) {
      return value.size();
   } else if (type == v_REG) {
      const int regid = getRegId(value);
      if (regid == REG_ERR) {
         std::cout << "[Runtime Error] evalData: Failed to bind v_REG '" << value << "' to a value.\n";
         exit(1);
      }
      if (regid == REG_PTR) return pointer;
      if (regid == REG_FLG) return flag;
      if (regid < 0) {
         return registers_f[(regid * -1) - 1];
      } else {
         return registers[regid];
      }
   } else if (type == v_VAR) {
      const std::string pstr = value.substr(1);
      if (vars.find(pstr) != vars.end()) {
         return vars[pstr].memaddr;
      } else {
         std::cout << "[Runtime Error] evalData: Failed to bind v_VAR '" << value << "' to a value.\n";
         exit(1);
      }
   } else if (type == v_CHR) {
      return value[0];
   } else {
      std::cout << "[Runtime Error] evalData: Invalid argument for data evaluation.\n";
      exit(1);
   }

   return 0;
}

// implement math instructions
#include "execFunc/math.cpp"

// implement jmp instructions
#include "execFunc/jump.cpp"

#include "execFunc/bitwise.cpp"

// implement memory manip functions
#include "execFunc/mmanip.cpp"

//IMPLEMENT mov() function
#include "execFunc/mov.cpp"

//IMPLEMENT rcopy() function
#include "execFunc/rcopy.cpp"

//IMPLEMENT cmp function
#include "execFunc/cmp.cpp"

// IMPLEMENT syscall function
#include "execFunc/syscll.cpp"

int var(const Instruction& ins) {
   if (ins.args.size() != 1) {
      return INVALID_ARGS;
   }

   if (vars.find(ins.uid) != vars.end()) {
      vars[ins.uid].value = ins.args[0];
      vars[ins.uid].type = ins.arg_types[0];
      memory[vars[ins.uid].memaddr].value = ins.args[0];
      memory[vars[ins.uid].memaddr].type = ins.arg_types[0];
      return 0;
   }

   VarCont toinsert;
   toinsert.value = ins.args[0];
   toinsert.type = ins.arg_types[0];
   //toinsert.memaddr = memory.size();

   toinsert.memaddr = pushToMem(toinsert.value, toinsert.type);

   vars[ins.uid] = toinsert;
   return 0;
}

void saveDebugLog() {
   std::stringstream outstr;

   outstr << "- [General] ---------------------\n"
          << "RUNFLAG - " << runFlag << std::endl
          << "$A - " << pointer << std::endl;
   if (pointer >= 0) {
      outstr << "r[$A] = " << registers[pointer] << std::endl;
   } else if (pointer == REG_PTR) {
      outstr << "r[$A] = " << pointer << std::endl;
   } else if (pointer == REG_FLG) {
      outstr << "r[$A] = " << flag << std::endl;
   } else if (pointer < 0) {
      outstr << "r[$A] = " << registers_f[(pointer * -1) - 1] << std::endl;
   }
   outstr << "$B - " << flag << std::endl;

   outstr << "\n- [Variables] -------------------\n";
   for (const auto &pair : vars) {
     outstr << pair.first << ": " << vars[pair.first].value << "\t\t[" << vars[pair.first].type << "]\t{" << vars[pair.first].memaddr << "}\n"; 
   }

   outstr << "\n- [Memory Controller] -----------\n";
   for (size_t i = 0; i < memcontroller.size(); i++) {
      outstr << '[' << i << "] = " << memcontroller[i] << std::endl;
   }

   outstr << "\n- [Memory] ----------------------\n";
   for (size_t i = 0; i < memory.size(); i++) {
      outstr << "m[" << i << "] = " << memory[i].value << "    {Type: " << memory[i].type << "}\n";
   }

   outstr << "\n- [INT Registers] ---------------\n";
   for (size_t i = 0; i < 10; i++) {
      outstr << "r[$" << i << "] = " << registers[i] << std::endl;
   }
   outstr << "\n- [FLOAT Registers] -------------\n";
   for (size_t i = 0; i < 5; i++) {
      outstr << "f[$" << i << "] = " << registers_f[i] << std::endl;
   }

   std::ofstream coutfile;

   coutfile.open("EXEC_FINAL_DUMP.txt");

   if (!coutfile) {
      std::cerr << "problem creating exec final dump file.\n";
      exit(1);
   }

   coutfile << outstr.str();
   coutfile.close();
}

int exec(std::vector<Instruction> inslist, bool strict = true) {

   //init
   srand(time(0));

   for (size_t i = 0; i < inslist.size(); i++) {
      const int ins = inslist[i].type;

      if (ins == i_FLG_1 || ins == i_FLG_2) {
         runFlag = (i_FLG_2 - ins);
         continue;
      }

      // data
      if (runFlag == 1) {
         if (ins == i_VAR) {
            if (var(inslist[i]) == 1) {
               std::cerr << "[Runtime Error] Variable assignment requires 1 argument. On instruction " << i << std::endl;
               return INVALID_ARGS;
            }
         }
         if (ins != i_VAR) {
            std::cerr << "[Runtime Error] Invalid instructions present during .data flag. On instruction " << i << std::endl;
            return INVALID_INS_DAT;
         }
         continue;
      }  

      //main
      if (runFlag == 0) {
         if (ins == i_LBL) continue;
         if (ins == i_return) return 0;
         if (ins == i_VAR) {
            if (strict) {
               std::cerr << "[Runtime Error] Variable declaration during .main flag illegal in strict mode. On instruction " << i << std::endl;
               return VAR_IN_MAIN;
            }
            const int r = var(inslist[i]);
            if (r == INVALID_ARGS) {
               std::cerr << "[Runtime Error] Variable assignment requires 1 argument. On instruction " << i << std::endl;
               return r;
            }
         }
         if (ins == i_li) {
            if (inslist[i].args.size() != 1 || inslist[i].arg_types[0] != v_NUM) {
               std::cerr << "[Runtime Error] Instruction li expects 1 argument of type 'NUM'. On instruction " << i << std::endl;
               return INVALID_ARGS;
            }

            flag = std::stoi(inslist[i].args[0]);
         }
         if (ins == i_la) {
            if (inslist[i].args.size() != 1 || inslist[i].arg_types[0] != v_REG) {
               std::cerr << "[Runtime Error] Instruction la expects 1 argument of type 'REG'. On instruction " << i << std::endl;
               return INVALID_ARGS;
            }

            const int rnum = getRegId(inslist[i].args[0]);

            if (rnum == REG_ERR) {
               std::cerr << "[Runtime Error] Register \"" << inslist[i].args[0] << "\" does not exist. On instruction " << i << std::endl;
               return REG_DNE;
            }

            pointer = rnum;
         }
         if (ins == i_copy) {
            const int r = rcopy(inslist[i]);
            if (r == 1) {
               std::cout << "[Runtime Error] Copy requires 2 arguments of type 'REG'. On instruction " << i << std::endl;
               return INVALID_ARGS;
            }
            if (r == REG_DNE) {
               std::cout << "[Runtime Error] Invalid registers in copy instruction. On instruction " << i << std::endl;
               return REG_DNE;
            }
         }

         if (ins == i_mov) {
            const int r = mov(inslist[i]);
            if (r == INVALID_ARGS) {
               std::cout << "[Runtime Error] 'mov' requires 2 arguments of type 'REG'/'DAT', 'REG' On instruction " << i << std::endl;
               return INVALID_ARGS;
            }
            if (r == REG_DNE) {
               std::cout << "[Runtime Error] Invalid register in 'mov' instruction. On instruction " << i << std::endl;
               return REG_DNE;
            }
            if (r == VAR_NOT_FOUND) {
               std::cout << "[Runtime Error] Cannot find variable in 'mov' instruction. On instruction " << i << std::endl;
               return VAR_NOT_FOUND;
            }
         }

         if (ins == i_syscall) {
            const int r = syscll();
            if (r == INVALID_FLAG) {
               std::cout << "[Runtime Error] Invalid syscall flag. On instruction " << i << std::endl;
               return INVALID_FLAG;
            }
            if (r == INVALID_ADDRESS) {
               std::cout << "[Runtime Error] Invalid memory address for syscall. On instruction " << i << std::endl;
               return INVALID_ADDRESS;
            }
         }

         if (ins == i_cmp) {
            const int r = compare(inslist[i]);
            if (r == INVALID_ARGS) {
               std::cout << "[Runtime Error] 'cmp' contains invalid arguments. On instruction " << i << std::endl;
               return INVALID_ARGS;
            }
            if (r == REG_DNE) {
               std::cout << "[Runtime Error] Invalid register on instruction " << i << std::endl;
               return REG_DNE;
            }
         }

         if (ins >= i_jeq && ins <= i_jmp) {
            const bool r = jump(inslist[i]);
            if (r) {
               i = inslist[i].jmp_index;
            }
         }

         if (ins >= i_add && ins <= i_mod) {
            const int r = math(inslist[i]);
            if (r == INVALID_ARGS) {
               std::cout << "[Runtime Error] 'add-mod' contains invalid arguments. On instruction " << i << std::endl;
               return INVALID_ARGS;
            }
            if (r == REG_DNE) {
               std::cout << "[Runtime Error] Invalid register. On instruction " << i << std::endl;
               return REG_DNE;
            }
            if (r == FLOAT_MOD_ERR) {
               std::cout << "[Runtime Error] Floating point modulo. On instruction " << i << std::endl;
               return FLOAT_MOD_ERR;
            }
         }

         if (ins >= i_mmov && ins <= i_mcpy) {
            const int r = memmanip(inslist[i]);
            if (r == INVALID_ARGS) {
               std::cout << "[Runtime Error] Invalid args on instruction " << i << std::endl;
               return INVALID_ARGS;
            }
            if (r == REG_DNE) {
               std::cout << "[Runtime Error] Invalid register on instruction " << i << std::endl;
               return REG_DNE;
            }
            if (r == INVALID_ADDRESS) {
               std::cout << "[Runtime Error] Invaild memory address on instruction " << i << std::endl;
               return INVALID_ADDRESS;
            }
         }

         if (ins >= i_band && ins <= i_bsr) {
            const int r = bitwise(inslist[i]);
            if (r == INVALID_ARGS) {
               std::cout << "[Runtime Error] Invalid args on instruction " << i << std::endl;
               return INVALID_ARGS;
            }
            if (r == REG_DNE) {
               std::cout << "[Runtime Error] Invalid register on instruction " << i << std::endl;
               return REG_DNE;
            }
         }

         continue;//end .main flag loop
      }

      // invalid, instructions outside of either flag.
      if (runFlag < 0) {
         std::cerr << "[Runtime Error] Instructions present outside of .data and/or .main. On instruction " << i << std::endl;
         return ILLEGAL_INS;
      }
      // end for loop
   }

   return 0;
}