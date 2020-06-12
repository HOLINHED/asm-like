#ifndef H_DK9PARSER
#define H_DK9PARSER

#include <vector>

#include "inst_obj.h"
#include "ins_map.h"
#include "instruction_ids.h"

const static size_t INSTRUCTION_SIZE = sizeof(INSTRUCTION_IDS) / sizeof(INSTRUCTION_IDS[0]);

std::string parseStr(const std::string&);

std::string parseChr(const std::string&);

int isNum(std::string);

std::vector<int> getArgTypeList(std::vector<std::string>, size_t);

std::vector<int> getArgTypeList(std::vector<std::string>);

int getInsType(std::string);

int getInsType(std::string);

void parseVar(std::vector<std::string>, Instruction&, size_t);

void parseLabel(std::vector<std::string>, Instruction&, size_t);

void parseJmpGeneral(std::vector<std::string>, Instruction&, std::vector<size_t>&, size_t);

void parseGeneric(std::vector<std::string>, Instruction&, size_t);

std::vector<Instruction> parse(std::vector<std::vector<std::string>>);

#endif