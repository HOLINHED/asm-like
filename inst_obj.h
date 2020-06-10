#ifndef H_DK9IOBJ
#define H_DK9IOBJ

#include <vector>
#include <string>

struct Instruction {
   int type{-1};
   size_t size{0};
   std::string uid;
   size_t jmp_index{0};
   std::vector<std::string> args;
   std::vector<int> arg_types;
};

#endif