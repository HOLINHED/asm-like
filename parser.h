#ifndef H_DK9PARSER
#define H_DK9PARSER

#include <vector>

#include "ins_map.h"
#include "instruction_ids.h"

struct Instruction {
   int type{-1};
   size_t size{0};
   std::string uid;
   size_t jmp_index{0};
   std::vector<std::string> args;
   std::vector<int> arg_types;
};

const static size_t INSTRUCTION_SIZE = sizeof(INSTRUCTION_IDS) / sizeof(INSTRUCTION_IDS[0]);

std::vector<int> getArgTypeList(std::vector<std::string>);

int getInsType(std::string);

std::vector<Instruction> parse(std::vector<std::vector<std::string>>);

#endif