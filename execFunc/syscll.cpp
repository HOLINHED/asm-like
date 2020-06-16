#include "../executor.h"

/*
0 - print num no newline
1 - print char no newline
2 - print string
3 - input num (non float)
4 - input num (float)
5 - input char
6 - input string (non format)
7 - input string (format)
8 - generate random number
9 - reset (clear registers and memory)
10 - print num newline
11 - print char newline
*/

void outNum() {
   if (pointer == REG_PTR) std::cout << pointer;
   else if (pointer == REG_FLG) std::cout << flag;
   else if (pointer < 0) {
      std::cout << registers_f[(pointer * -1) - 1];
   } else {
      std::cout << registers[pointer];
   }   
}

void outChar() {
   if (pointer == REG_PTR) std::cout << ((char)pointer);
   else if (pointer == REG_FLG) std::cout << ((char)flag);
   else if (pointer < 0) {
      std::cout << ((char)registers_f[(pointer * -1) - 1]);
   } else {
      std::cout << ((char)registers[pointer]);
   }   
}

void inputStr(short type) { //1 - noformat, 2 - format
   std::string input;
   if (type == 1) std::getline(std::cin, input);
   if (type == 2) std::cin >> input;

   if (pointer == REG_PTR) {
      pointer = memory.size();
   } else if (pointer == REG_FLG) {
      flag = memory.size();
   } else if (pointer < 0) {
      registers_f[(pointer * -1) - 1] = memory.size();
   } else {
      registers[pointer] = memory.size();
   }

   MemoryUnit ptomem;
   ptomem.value = input;
   ptomem.type = v_STR;

   memory.push_back(ptomem);
}

template<class T>
void inputNum() {
   T input;
   std::cin >> input;

   if (pointer == REG_PTR) {
      pointer = input;
   } else if (pointer == REG_FLG) {
      flag = input;
   } else if (pointer < 0) {
      registers_f[(pointer * -1) - 1] = input;
   } else {
      registers[pointer] = input;
   }
}

int syscll() {

   switch(flag) {
      case 0: {
         outNum();
      }; break;
      case 1: {
         outChar();
      }; break;
      case 2: {
         int memaddr = pointer;

         if (memaddr == REG_PTR) return INVALID_ADDRESS;

         if (memaddr == REG_PTR || memaddr >= 0) {
            if (memaddr == REG_PTR) {
               memaddr = flag;
            } else {
               memaddr = registers[memaddr];
            }
         } else {
            memaddr = registers_f[(memaddr * -1) - 1];
         }

         if (memaddr < 0 || memaddr >= static_cast<int>(memory.size())) {
            return INVALID_ADDRESS;
         }

         std::cout << std::any_cast<std::string>(memory[memaddr].value);
      }; break;
      case 3: {
         inputNum<long>();
      }; break;
      case 4: {
         inputNum<double>();
      }; break;
      case 5: {
         inputNum<char>();
      }; break;
      case 6: {
         inputStr(1);
      }; break;
      case 7: {
         inputStr(2);
      }; break;
      case 8: {
         int r = rand();

         if (pointer == REG_PTR) {
            pointer = r;
         } else if (pointer == REG_FLG) {
            flag = r;
         } else if (pointer < 0) {
            registers_f[(pointer * -1) - 1] = r;
         } else {
            registers[pointer] = r;
         }
      }; break;
      case 9: {
         vars.clear();
         memory.clear();
         for (size_t i = 0; i < 10; i++) registers[i] = 0;
         for (size_t i = 0; i < 5; i++) registers_f[i] = 0;
         pointer = 0;
         flag = 0;
      }; break;
      case 10: {
         outNum();
         std::cout << '\n';
      }; break;
      case 11: {
         outChar();
         std::cout << '\n';
      }; break;
      default: {
         return INVALID_FLAG;
      }
   }

   return 0;
}