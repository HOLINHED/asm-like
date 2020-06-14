#include "serialization.h"

#include <iostream>

void serialize(std::vector<Instruction> ins, std::string filename) {
   std::stringstream outstr;

   for (Instruction V : ins) {
      outstr << V.type << std::endl;
      outstr << V.size << std::endl;
      outstr << V.uid << std::endl;
      outstr << V.jmp_index << std::endl;
      outstr << V.args.size() << std::endl;
      for (std::string x : V.args) {
         outstr << x << std::endl;
      }
      outstr << V.arg_types.size() << std::endl;
      for (int y : V.arg_types) {
         outstr << y << std::endl;
      }
   }

   std::ofstream genfile(filename);

   if (!genfile) {
      std::cerr << "Error generating program file\n";
      exit(1);
   }

   genfile << outstr.str();
   genfile.close();
}

std::vector<Instruction> deserialize(std::stringstream* content) {
   std::vector<Instruction> retrived;
   
   size_t stepCounter = 0;
   Instruction toInsert;

   std::string line;
   while (std::getline(*content, line)) {
      
      switch(stepCounter) {
         case 0: {
            toInsert.type = std::stoi(line);
         }; break;
         case 1: {
            toInsert.size = std::stoi(line);
         }; break;
         case 2: {
            toInsert.uid = line;
         }; break;
         case 3: {
            toInsert.jmp_index = std::stoi(line);
         }; break;
         case 4: {

         }; break;
         case 5: {

            retrived.push_back(toInsert);
            stepCounter = 0;
         }
      }

      stepCounter += 1;
   }
   return retrived;
}