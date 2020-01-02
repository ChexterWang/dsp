#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <set>
#include <map>
#include "File.h"
#include "Ngram.h"
#include "Vocab.h"

using namespace std;

typedef map<string, set<string> > Mymap;
typedef pair<vector<string>,float> Mynode;
typedef vector<pair<vector<string>,float> > Mygraph;

bool checkFile(string&& filename);
size_t myStrGetTok(const string& str, string& tok, size_t pos = 0, const char del = ' ');
void readMap(ifstream& ifs, Mymap& mp);
void findCandy(string str, Mymap& map, set<string>& set);
Mynode nextNode(Vocab& v, Ngram& lm, Mygraph& g, string str);
float biProb(Vocab& v, Ngram& lm, const char* c1, const char* c2);

int main(int argc, char** argv){
   // file error handling
   if(argc!=5){
      cerr << "[ERROR] Usage: ./disambig <segemented file> <mapping file> <language model> <output file>\n";
      exit(1);
   }
   if(!checkFile(string(argv[1]))){
      cerr << "[ERROR] Cannot open segemented file \"" << string(argv[1]) << "\"\n";
      exit(1);
   }
   if(!checkFile(string(argv[2]))){
      cerr << "[ERROR] Cannot open mapping file \"" << string(argv[2]) << "\"\n";
      exit(1);
   }
   if(!checkFile(string(argv[3]))){
      cerr << "[ERROR] Cannot open language model \"" << string(argv[3]) << "\"\n";
      exit(1);
   }

   // declaration: language model
   Vocab voc;
   Ngram lm(voc, 2);

   // read in map file
   Mymap map;
   ifstream* mapfs = new ifstream(string(argv[2]));
   readMap(*mapfs, map);
   mapfs->close();

   // read in lm file from corpus
   FILE* file = fopen(argv[3], "r");
   File lmfs(file);
   lm.read(lmfs);
   lmfs.close();

   // read in test data file
   ifstream* ifs = new ifstream(string(argv[1]));
   string str, tok;
   getline(*ifs, str);
   size_t pos = 0;
   stringstream ss;
   vector<char*> vstr;

   // declaration: viterbi
   set<string> tmpset;
   Mynode node;
   Mygraph graph;

   // declaration ofstream
   ofstream* ofs = new ofstream(string(argv[4]));

   // parse every line in test data
   while(ifs->good()){

      // initialize all extern var
      tok = "0";
      for(size_t i = 0; i < vstr.size(); ++i) delete vstr[i];
      vstr.clear();
      tmpset.clear();
      graph.clear();
      node = Mynode({}, 0);

      // read in a line to vstr
      pos = myStrGetTok(str, tok);
      char* t = new char[2];
      strcpy(t, tok.c_str());
      vstr.push_back(t);
      while(tok.size()){
         pos = myStrGetTok(str, tok, pos);
         t = new char[2];
         strcpy(t, tok.c_str());
         vstr.push_back(t);
      }

      // initialization
      findCandy(vstr[0], map, tmpset);
      for(auto itt = tmpset.begin(); itt != tmpset.end(); ++itt){
         graph.push_back(pair<vector<string>,float>({*itt}, 0));
      }

      // induction
      for(size_t i = 1; i < vstr.size(); ++i){
         Mygraph tmpgraph;
         findCandy(vstr[i], map, tmpset);
         for(auto itt = tmpset.begin(); itt != tmpset.end(); ++itt)
            tmpgraph.push_back(nextNode(voc, lm, graph, *itt));
         graph.clear();
         graph = tmpgraph;
      }

      // termination
      float maxprob = -100;
      size_t idx = 0;
      for(size_t i = 0; i < graph.size(); ++i){
         if(graph[i].second > maxprob){ idx = i; maxprob = graph[i++].second; }
      }

      // output the line
      ss.str("");
      ss << "<s>";
      for(size_t i = 0; i < graph[idx].first.size(); ++i)
         ss << ' ' << graph[idx].first[i];
      ss << "</s>\n";
      *ofs << ss.str();

      getline(*ifs, str);
   }
   ifs->close();
   ofs->close();
}

bool checkFile(string&& filename){
   ifstream* ifs = new ifstream(filename);
   bool ret = true;
   if(!ifs->good()) ret = false;
   ifs->close();
   delete ifs;
   return ret;
}

size_t myStrGetTok(const string& str, string& tok, size_t pos, const char del){
   size_t begin = str.find_first_not_of(del, pos);
   if (begin == string::npos) { tok = ""; return begin; }
   size_t end = str.find_first_of(del, begin);
   tok = str.substr(begin, end - begin);
   return end;
}

void readMap(ifstream& ifs, Mymap& mp){
   size_t pos = 0;
   string str, tok0, tok1, tok2;
   getline(ifs, str);
   pos = myStrGetTok(str, tok0);
   pos = myStrGetTok(str, tok1, pos);
   mp[tok0].insert(tok1);
   while(str.size()){
      pos = myStrGetTok(str, tok1, pos);
      while(tok1.size()){
         mp[tok0].insert(tok1);
         pos = myStrGetTok(str, tok1, pos);
      }
      getline(ifs, str);
      if(str.size()){
         pos = myStrGetTok(str, tok0);
         pos = myStrGetTok(str, tok1, pos);
         mp[tok0].insert(tok1);
      }
   }
}

void findCandy(string str, Mymap& map, set<string>& set){
   Mymap::iterator it = map.find(str);
   if(it != map.end()) set = it->second;
   else { map[str].insert(str); set = map[str]; }
}

Mynode nextNode(Vocab& v, Ngram& lm, Mygraph& g, string str){
   size_t max = 0;
   float prob = 0, maxprob = -100;
   for(size_t i = 0; i < g.size(); ++i){
      prob = g[i].second + biProb(v, lm, (--g[i].first.end())->c_str(), str.c_str());
      if(prob > maxprob) { maxprob = prob; max = i; }
   }
   Mynode node = g[max];
   node.first.push_back(str);
   node.second = maxprob;
   return node;
}

float biProb(Vocab& v, Ngram& lm, const char* c0, const char* c1){
   VocabIndex i0 = v.getIndex(c0);
   VocabIndex i1 = v.getIndex(c1);
   if(i0 == Vocab_None) i0 = v.getIndex(Vocab_Unknown);
   if(i1 == Vocab_None) i1 = v.getIndex(Vocab_Unknown);
   VocabIndex context[] = { i0, Vocab_None };
   return lm.wordProb(i1, context);
}
