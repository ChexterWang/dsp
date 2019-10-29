#include <iostream>
#include <fstream>
#include <string>

#define MAX_SEQ 2500

#include "../inc/myString.h"

using namespace std;

int main(){

   ifstream* fr = new ifstream("./result.txt");
   char(*res)[256] = new char[MAX_SEQ][256];
   for(int i = 0; i < MAX_SEQ; ++i){
      fr->getline(res[i], 256);
      res[i][12] = '\0';
   }
   fr->close();

   ifstream* fl = new ifstream("./data/test_lbl.txt");
   char(*lbl)[256] = new char[MAX_SEQ][256];
   for(int i = 0; i < MAX_SEQ; ++i){
      fl->getline(lbl[i], 256);
   }

   float count = 0;
   for(int i = 0; i < MAX_SEQ; ++i){
      if(res[i][7] != lbl[i][7]){
         ++count;
      }
   }
   cout << 1 - count/MAX_SEQ << endl;
}
