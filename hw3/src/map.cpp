#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

size_t myStrGetTok(const string& str, string& tok, size_t pos = 0, const char del = ' ');

class ZChar{
public:
   ZChar(string& s){
      _c[0] = s[0];
      _c[1] = s[1];
   }
   ZChar(string&& s){ _c[0] = s[0]; _c[1] = s[1]; }
   ~ZChar(){}
   unsigned num() const { return (unsigned)(_c[0]-'0')+10000*(_c[1]-'0'); }
   char operator [] (size_t i) const { return _c[i]; }
   bool operator < (const ZChar& zc) const { return (num() < zc.num())?true:false; }
   bool operator == (const ZChar& zc) const { return (num() == zc.num())?true:false; }
   bool operator != (const ZChar& zc) const { return (num() != zc.num())?true:false; }
   ZChar& operator = (const ZChar& zc) { _c[0] = zc[0]; _c[1] = zc[1]; return *this; }
   friend ostream& operator << (ostream& os, ZChar& zc){ os << zc._c[0] << zc._c[1]; return os; }
private:
   char _c[2];
};

class BChar{
public:
   BChar(string& sb, string& sz):_z(sz) { _c[0] = sb[0]; _c[1] = sb[1]; }
   ~BChar(){}
   friend ostream& operator << (ostream& os, BChar& bc){ os << bc._c[0] << bc._c[1]; return os; }
   unsigned znum() const { return _z.num(); }
   unsigned num() const { return (unsigned)(_c[0]-'0')+10000*(_c[1]-'0'); }
   bool compare(char* str){ return (_c[0] == str[0] && _c[1] == str[1])?true:false; }
   char operator [] (size_t i) const { return _c[i]; }
   bool operator < (const BChar& bc) const {
      if(znum() < bc.znum()) return true;
      else if(znum() == bc.znum() && num() < bc.num()) return true;
      else return false;
   }
   BChar& operator = (const BChar& bc) { _c[0] = bc[0]; _c[1] = bc[1]; _z = bc.ztype(); return *this; }
   bool operator == (const BChar& bc) const { return (_c[0]==bc[0] && _c[1]==bc[1])?true:false; }
   bool operator != (const BChar& bc) const { return (_c[0]!=bc[0] || _c[1]!=bc[1])?true:false; }
   ZChar ztype() const { return _z; }
   void str(char* in) const { in[0] = _c[0]; in[1] = _c[1]; }
private:
   char _c[2];
   ZChar _z;
};


int main(int argc, char **argv){
   if(argc != 3){
      cerr << "[ERROR] Please enter 2 filenames. Exiting..." << '\n';
      exit(1);
   }
   string filename(argv[1]);
   ifstream* ifs = new ifstream(filename);
   if(!ifs->good()){
      cerr << "[ERROR] Cannot open file. Exiting..." << '\n';
      exit(1);
   }
   filename = string(argv[2]);
   ofstream* ofs = new ofstream(filename);
   string str, tok0, tok1, tok2;
   size_t pos = 0, id = 0;
   vector<BChar> dic;
   ZChar zc("  ");
   char g[2];
   stringstream ss;
   while(ifs->good()){
      getline(*ifs, str);
      if(str.size()){
         pos = myStrGetTok(str, tok0);
         pos = myStrGetTok(str, tok1, pos);
         pos = myStrGetTok(tok1, tok2, 0, '/');
         while(tok2.size()){
            dic.push_back(BChar(tok0, tok2));
            pos = myStrGetTok(tok1, tok2, pos, '/');
            ++id;
         }
      }
   }
   ifs->close();
   sort(dic.begin(), dic.end());
   zc = dic[0].ztype();
   ss << zc;
   for(size_t i = 0; i < dic.size(); ++i){
      if(dic[i].ztype()!=zc){
         zc = dic[i].ztype();
         *ofs << ss.str() << '\n';
         ss.str("");
         ss << zc;
      }
      ss << ' ' << dic[i];
   }
   *ofs << ss.str() << '\n';
   vector<BChar> tmp;
   for(size_t i = 0; i < dic.size(); ++i){
      ss.str("");
      if(find(tmp.begin(), tmp.end(), dic[i])==tmp.end()){
         ss << dic[i] << ' ' << dic[i] << '\n';
         *ofs << ss.str();
         tmp.push_back(dic[i]);
      }
   }
   ofs->close();
}

size_t myStrGetTok(const string& str, string& tok, size_t pos, const char del){
   size_t begin = str.find_first_not_of(del, pos);
   if (begin == string::npos) { tok = ""; return begin; }
   size_t end = str.find_first_of(del, begin);
   tok = str.substr(begin, end - begin);
   return end;
}
