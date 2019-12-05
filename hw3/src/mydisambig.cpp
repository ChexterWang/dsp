#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <vector>
#include "Ngram.h"
using namespace std;

int main(int argc, char** argv){
   if(argc!=5){
      cerr << "[ERROR] Usage: ./disambig <segemented file> <mapping file> <language model> <output file>\n";
      exit(1);
   }
   if(checkFile(string(argv[1]))){
      cerr << "[ERROR] Cannot open segemented file \"" << string(argv[1]) << "\"\n";
      exit(1);
   }
   if(checkFile(string(argv[2]))){
      cerr << "[ERROR] Cannot open mapping file \"" << string(argv[2]) << "\"\n";
      exit(1);
   }
   if(checkFile(string(argv[3]))){
      cerr << "[ERROR] Cannot open language model \"" << string(argv[3]) << "\"\n";
      exit(1);
   }

}

bool checkFile(string&& filename){
   ifstream* ifs = new ifstream(filename);
   bool ret = true;
   if(!ifs.good()) ret = false;
   ifs->close();
   delete ifs;
   return ret;
}
