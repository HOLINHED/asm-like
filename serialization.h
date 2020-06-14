#ifndef H_DK9SER
#define H_DK9SER

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "inst_obj.h"

void serialize(std::vector<Instruction>, std::string);

std::vector<Instruction> deserialize(std::stringstream*);

#endif