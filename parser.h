#ifndef H_DK9PARSER
#define H_DK9PARSER

#include <vector>
#include <string>
#include <map>

#include "instruction_ids.h"

struct Instruction {
   int type;
   size_t size;
   std::string uid;
   size_t jmp_index;
   std::vector<std::string> args;
   std::vector<int> arg_types;
};

const static std::string INSTRUCTION_IDS[] = 
{
   "li",
   "la",
   "mov",
   "syscall",
   "add",
   "sub",
   "mul",
   "div",
   "mod",
   "copy",
   "cmp", 
   "jeq",
   "jne",
   "jae",
   "jue",
   "ja",
   "ju",
   "jmp",
   "mmov",
   "mret",
   "mcpy",
   "band",
   "bor",
   "bsl",
   "bsr",
   "LBL",
   "VAR",
   "FLG_1",
   "FLG_2",
};

const static size_t INSTRUCTION_SIZE = sizeof(INSTRUCTION_IDS) / sizeof(INSTRUCTION_IDS[0]);

std::vector<int> getArgTypeList(std::vector<std::string>);

int getInsType(std::string);

std::vector<Instruction> parse(std::vector<std::vector<std::string>>);

#endif