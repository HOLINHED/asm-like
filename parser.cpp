#include "parser.h"
#include <iostream>
#include <sstream>
#include <map>

#define INS_ARR INSTRUCTION_IDS
#define INS_SIZE INSTRUCTION_SIZE

std::string parseStr(const std::string& s) {
    std::stringstream sout;

    for(size_t i = 1; i < s.length() - 1; i++) {
        if (s.at(i) == '\\') {
            switch(s.at(i + 1)) {
                case 'n': sout << "\n"; i++; break;
                case '"': sout << "\""; i++; break;
                case 't': sout << "\t"; i++; break;
                default:  sout << "\\";      break;
            }       
        } else {
            sout << s.at(i);
        }
    }

    return sout.str();
}

std::string parseChr(const std::string& s) {
   if (s.size() == 4) { //'\x'
      switch(s[2]) {
         case 'n': return "\n"; break;
         case '"': return "\""; break;
         case 't': return "\t"; break;
         case '\\': return "\\"; break;
         default: return "err";
      }   
   }
   return s.substr(1,1);
}

// 0 - is not num,   1 - is num,   2 - is float
int isNum(std::string str) {
   bool com = false;
   if (str == "-") return 0;
   for (size_t i = str[0] == '-' ? 1 : 0; i < str.size(); i++) {
      if (str[i] == '.') {
         if (com) return 0;
         com = true;
         continue;
      }
      if (!std::isdigit(str[i])) {
         return 0;
      }
   }
   if (com) return 2;
   return 1;
}

std::vector<int> getArgTypeList(std::vector<std::string> args, size_t ln = 1) {
   std::vector<int> argtypes;
      for (std::string arg : args) {
         if (arg.substr(0, 2) == "*$") {
            if (arg.size() < 3) {
               std::cerr << "[Parse Error] Invalid symbol on line " << ln << std::endl;
               exit(1);
            }
            argtypes.push_back(v_MEMPTR);
            continue;
         }
         if (arg.size() == 3 && arg[0] == '\'' && arg[2] == '\'') {
            argtypes.push_back(v_CHR);
            continue;
         }
         if (arg.size() == 4 && arg[0] == '\'' && arg[1] == '\\' && arg[3] == '\'') {
            argtypes.push_back(v_CHR);
            continue;
         }
         const int isnres = isNum(arg);
         if (isnres != 0) {
            int toPush = v_NUM;
            if (isnres == 2) toPush = v_NUM_F; 
            argtypes.push_back(toPush);
            continue;
         }
         if (arg.size() < 2) {
            std::cerr << "[Parse Error] Invalid symbol on line " << ln << std::endl;
            exit(1);
         }
         int argid = v_DAT;
         switch(arg[0]) {
            case '"': argid = v_STR; break;
            case '$': argid = v_REG; break;
            case '#': argid = v_VAR; break;
            case '*': argid = v_MEM; break;
         }
         argtypes.push_back(argid);
   }
   return argtypes;
}

int getInsType(std::string str) {

   std::string search = str;

   if (str[0] == ':') search = "LBL";
   if (str == ".data") search = "FLG_1";
   if (str == ".main") search = "FLG_2";
   if (str[str.size() - 1] == ':') search = "VAR";

   for (size_t i = 0; i < INS_SIZE; i++) {
      if (INS_ARR[i] == search) return i;
   }

   return -1;
}

void parseVar(std::vector<std::string> line, Instruction& result, size_t ln = 1) {
   if (line.size() != 2) {
      std::cerr << "[Parse Error] variable name expected 1 argument. On line " << ln << std::endl;
      exit(1);
   }

   if (line[0].substr(0, line[0].size() - 1).find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos) {
      std::cerr << "[Parse Error] Variable name contains invalid characters. On line " << ln << std::endl;
      exit(1);
   }

   if (line[0] == ":") {
      std::cerr << "[Parse Error] Variable does not have a name. On line " << ln << std::endl;
      exit(1); 
   }

   result.uid = line[0].substr(0, line[0].size() - 1);

   line.erase(line.begin());

   result.arg_types = getArgTypeList(line, ln);

   if (result.arg_types[0] == v_STR) {
      line[0] = parseStr(line[0]);
      result.args = line;
   } else if (result.arg_types[0] == v_CHR) {
      line[0] = parseChr(line[0]);
      result.args = line;
   } else {
      result.args = line;
   }

   result.size = result.args[0].size();
}

std::map<std::string, size_t> labelList;

void parseLabel(std::vector<std::string> line, Instruction& result, size_t ln = 1) {
   
   if (line.size() != 1) {
      std::cerr << "[Parse Error] Label expects no arguments. On line " << ln << std::endl;
      exit(1);
   }

   if (line[0] == ":") {
      std::cerr << "[Parse Error] Label declaration expects a name. On line " << ln << std::endl;
      exit(1);
   }

   result.uid = line[0].substr(1);

   if (labelList.find(result.uid) != labelList.end()) {
      std::cerr << "[Parse Error] Label \"" << result.uid << "\" has alredy been declared. On line " << ln << std::endl;
      exit(1);
   }

   labelList[result.uid] = ln - 1;
}

void parseJmpGeneral(std::vector<std::string> line, Instruction& result, std::vector<size_t>& toLink, size_t ln = 1) {
   
   line.erase(line.begin());

   if (line.size() != 1) {
      std::cerr << "[Parse Error] Jump instruction expected a label name. On line " << ln << std::endl;
      exit(1);
   }

   result.args = line;
   result.arg_types.push_back(v_LBL);
   
   if (labelList.find(result.args[0]) == labelList.end()) {
      result.jmp_index = 0;
      toLink.push_back(ln - 1);
   } else {
      result.jmp_index = labelList[result.args[0]];
   }
}

void parseGeneric(std::vector<std::string> line, Instruction& result, size_t ln = 1) {

   const std::string iname(line[0]);

   line.erase(line.begin());

   if (line.size() == 0) {
      std::cout << "Instruction " << iname << " expected at least 1 argument. On line " << ln << std::endl;
      exit(1);
   }

   result.arg_types = getArgTypeList(line, ln);

   std::vector<std::string> arglist;
   for (size_t i = 0; i < line.size(); i++) {
      if (result.arg_types[i] == v_STR) {
         std::string pstring = parseStr(line[i]);
         arglist.push_back(pstring);
         continue;
      } 
      if (result.arg_types[i] == v_CHR) {
         std::string pstring = parseChr(line[i]);
         arglist.push_back(pstring);
         continue;
      } 
      arglist.push_back(line[i]);
   }

   result.args = arglist;
}

std::vector<Instruction> parse(std::vector<std::vector<std::string>> adt) {

   std::vector<Instruction> result;
   std::vector<size_t> toLink;

   size_t ln = 0;
   for (std::vector<std::string> line : adt) {
      ln += 1;

      if (line.size() == 1 && line[0].empty()) continue;

      if (line.size() == 0) continue;

      const int ins = getInsType(line[0]);

      if (ins == -1) {
         std::cerr << "[Parse Error] Instruction \"" << line[0] << "\" undefined on line " << ln << ".\n";
         exit(1);
      }

      Instruction toPush;

      toPush.type = ins;

      bool ccomp = false;

      if (ins == i_VAR) {
         parseVar(line, toPush, ln);
         ccomp = true;
      }
      if (ins == i_LBL) { 
         parseLabel(line, toPush, ln);
         ccomp = true;
      }
      // jumps
      if (ins >= i_jeq && ins <= i_jmp) {
         parseJmpGeneral(line, toPush, toLink, ln);
         ccomp = true;
      }

      if (!ccomp) {
         if (!(ins == i_return || ins == i_syscall || ins == i_FLG_1 || ins == i_FLG_2)) {
            parseGeneric(line, toPush, ln);
         } else {
            if (line.size() != 1) {
               std::cout << "[Parse Error] Instruction " << line[0] << " expected no arguments. On line " << ln << std::endl;
               exit(1);
            }
         }
      }

      result.push_back(toPush);
   }

   for (size_t i : toLink) {
     if (labelList.find(result[i].args[0]) == labelList.end()) {
        std::cerr << "[Parse Error] Label \"" << result[i].args[0] << "\" undefined. On line " << (i + 1) << std::endl;;
        exit(1); 
     } else {
        result[i].jmp_index = labelList[result[i].args[0]];
     }
   }

   return result;
}