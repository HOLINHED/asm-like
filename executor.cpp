#include <iostream>
#include <sstream>
#include <map>
#include <fstream>
#include "instruction_ids.h"
#include "executor.h"

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

std::map<std::string, VarCont> vars;

std::vector<MemoryUnit> memory;
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

int math(const Instruction& ins) {
   return 0;
}

int jump(int opCode) {
   return 0;
}

int bitwise(const Instruction& ins) {
   return 0;
}

int memmanip(const Instruction& ins) {
   return 0;
}

//IMPLEMENT mov() function
#include "execFunc/mov.cpp"

//IMPLEMENT rcopy() function
#include "execFunc/rcopy.cpp"

int compare(const Instruction& ins) {
   return 0;
}

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
   toinsert.memaddr = memory.size();

   vars[ins.uid] = toinsert;

   MemoryUnit instomem;
   instomem.value = toinsert.value;
   instomem.type = toinsert.type;
   memory.push_back(instomem);

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
     outstr << pair.first << ": " << vars[pair.first].value << " [" << vars[pair.first].type << "]\n"; 
   }

   outstr << "\n- [Memory] ----------------------\n";
   for (size_t i = 0; i < memory.size(); i++) {
      outstr << "m[" << i << "] = " << std::any_cast<std::string>(memory[i].value) << "    {Type: " << memory[i].type << "}\n";
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
            if (var(inslist[i]) != 0) {
               if (r == INVALID_ARGS) {
                  std::cerr << "[Runtime Error] Variable assignment requires 1 argument. On instruction " << i << std::endl;
               }
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

#define DK9CFLAG 

#ifndef DK9CFLAG
int main(int argc, char** argv) {


   return 0;
}

#endif