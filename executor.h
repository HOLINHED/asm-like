#ifndef H_DK9EXEC
#define H_DK9EXEC

#include <vector>
#include "instruction_ids.h"
#include "inst_obj.h"

#define REG_PTR -20
#define REG_FLG -10
#define REG_ERR -30

struct VarCont {
   std::string value;
   int type;
   size_t memaddr;
};

struct MemoryUnit {
   std::string value;
   int type;
};

int getRegId(std::string);

template<class T = std::string>
size_t pushToMem(T, int);

template<class T = long double>
T evalData(std::string, int);

int math(const Instruction&);

bool jump(const Instruction&);

int bitwise(const Instruction&);

int memmanip(const Instruction&);

int mov(const Instruction&);

int rcopy(const Instruction&);

int compare(const Instruction&);

int syscll();

int var(const Instruction&);

void saveDebugLog();

int exec(std::vector<Instruction>, bool);

#endif