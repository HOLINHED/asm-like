#ifndef H_DK9INSMAP
#define H_DK9INSMAP

#include <string>

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
   "mdel",
   "mcpy",
   "band",
   "bor",
   "bsl",
   "bsr",
   "return",
   "LBL",
   "VAR",
   "FLG_1",
   "FLG_2",
};

#endif