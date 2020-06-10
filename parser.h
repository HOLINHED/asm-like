#ifndef H_DK9PARSER
#define H_DK9PARSER

#include <vector>

#include "inst_obj.h"
#include "ins_map.h"
#include "instruction_ids.h"

const static size_t INSTRUCTION_SIZE = sizeof(INSTRUCTION_IDS) / sizeof(INSTRUCTION_IDS[0]);

std::vector<int> getArgTypeList(std::vector<std::string>);

int getInsType(std::string);

std::vector<Instruction> parse(std::vector<std::vector<std::string>>);

#endif