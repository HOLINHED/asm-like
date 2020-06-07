#ifndef H_DK9PARSER
#define H_DK9PARSER

#include <vector>
#include <string>

struct Instruction {
   int type;
   std::string uid;
   size_t jmp_index;
   std::vector<std::string> args;
};

const static size_t INSTRUCTION_SIZE = 20;
const static std::string INSTRUCTION_IDS[] = 
{
   "li",
   "la",
   "mov",
   "syscall",
   "add",
   "sub",
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
   "LBL",
   "FLG_1",
   "FLG_2"
};

int getInsType(std::string);

std::vector<Instruction> parse(std::vector<std::vector<std::string>>);

#endif