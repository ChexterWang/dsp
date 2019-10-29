//     ./test <models_list_path> <seq_path> <output_result_path>
// ex: ./test ./modellist.txt ./data/test_seq.txt ./result.txt
//
// test: put every line in test_seq.txt into 5 models and output
// the best fitting model of that line and its probability/
//
// ./result.txt
//     <trained_model_path> <max P(O|trained_model, state_sequence)>
// ex: model_01.txt 7.822367e-34
//     model_05.txt 1.094896e-40

#include <iostream>
#include <fstream>
#include <string>

#define MAX_MODELS 5
#define MAX_SEQ 2500
#define SEQ_LENGTH 50
#define MAX_STATE 6

#include "../inc/hmm.h"

using namespace std;

int main(int argc, char const *argv[]) {

   // argument exceptions handling
   if(argc != 4){
      cerr << "Error: Wrong ammount of arguments." << endl;
      exit(-1);
   }
   ifstream* modelList = new ifstream(argv[1]);
   if(!modelList->is_open()){
      cerr << "Error: Can't open model list file. (" << argv[1] << ")";
      exit(-1);
   }
   char(*models)[256] = new char[MAX_MODELS][256];
   for(int i = 0; i < MAX_MODELS; ++i){
      modelList->getline(models[i], 256);
   }
   modelList->close();

   ifstream* testSeq = new ifstream(argv[2]);
   if(!testSeq->is_open()){
      cerr << "Error: Can't open test sequence file. (" << argv[2] << ")";
      exit(-1);
   }
   char(*seqs)[SEQ_LENGTH+1] = new char[MAX_SEQ][SEQ_LENGTH+1];
   for(int i = 0; i < MAX_SEQ; ++i){
      testSeq->getline(seqs[i], SEQ_LENGTH+1);
   }
   testSeq->close();

   double* p = new double[MAX_SEQ];
   char(*result)[256] = new char[MAX_SEQ][256];
   HMM* hmm = new HMM[MAX_MODELS];
   load_models(argv[1], hmm, MAX_MODELS);

   // TODO: test
   for(int current_seq = 0; current_seq < MAX_SEQ; ++current_seq){

      for(int m = 0; m < MAX_MODELS; ++m){

         int c = 0;

         // initialization: delta
         double(*delta)[MAX_STATE] = new double[SEQ_LENGTH][MAX_STATE];
         c = seqs[current_seq][0]-'A';
         for(int i = 0; i < MAX_STATE; ++i){
            delta[0][i] = hmm[m].initial[i]
                          * hmm[m].observation[c][i];
         }

         // recursion
         for(int t = 1; t < SEQ_LENGTH; ++t){
            c = seqs[current_seq][t]-'A';
            for(int j = 0; j < MAX_STATE; ++j){
               double preMax = delta[t-1][0]*hmm[m].transition[0][j];
               for(int i = 1; i < MAX_STATE; ++i){
                  if(delta[t-1][i]*hmm[m].transition[i][j] > preMax)
                     preMax = delta[t-1][i]*hmm[m].transition[i][j];
               }
               delta[t][j] = preMax * hmm[m].observation[c][j];
            }
         }

         // termination
         double max = delta[SEQ_LENGTH-1][0];
         for(int i = 1; i < MAX_STATE; ++i){
            if(delta[SEQ_LENGTH-1][i] > max){
               max = delta[SEQ_LENGTH-1][i];
            }
         }

         // find the highest probability among all models
         if((m == 0) || max > p[current_seq]){
            p[current_seq] = max;
            for(int i = 0; i < 256; ++i){
               result[current_seq][i] = models[m][i];
            }
         }

      }
   }

   ofstream* resultFile = new ofstream(argv[3]);
   for(int i = 0; i < MAX_SEQ; ++i){
      *resultFile << result[i] << " " << p[i] << endl;
   }

}
