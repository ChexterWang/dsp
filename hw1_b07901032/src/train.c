// usage: ./train <iter> <model_init_path> <seq_path> <output_model_path>
// ex:    ./train 100 ./model_init.txt ./data/train_seq_01.txt ./model_01.txt
//
// HMM model
// observation sequence = {A, B, C, D, E, F} i.e. one alphabet
// initial[x] = [initial probility of state x]
// transition[x][y] = [transition probility from state x to state y]
// observation[x][y] = [observation probility of observation sequence x in state y]

#include <iostream>
#include <fstream>
#include <cstdio>

#define MAX_STATE 6
#define MAX_OBSERV 6
#define SEQ_LENGTH 50
#define MAX_SEQ 10000

#include "../inc/hmm.h"
#include "../inc/myString.h"

using namespace std;

int main(int argc, char const *argv[]) {

   // argument exceptions handling
   if(argc != 5){
      cerr << "Error: Wrong ammount of arguments." << endl;
      exit(-1);
   }
   int iter;
   if(!myStr2Int(argv[1], iter)){
      cerr << "Error: # of iteration should be integer." << endl;
      exit(-1);
   }
   ifstream* trainSeq = new ifstream(argv[3]);
   if(!trainSeq->is_open()){
      cerr << "Error: Can't open train sequence file.";
      exit(-1);
   }
   char(*seqs)[SEQ_LENGTH] = new char[MAX_SEQ][SEQ_LENGTH]{};
   for(int i = 0; i < MAX_SEQ; ++i){
      trainSeq->getline(seqs[i], SEQ_LENGTH+1);
   }
   trainSeq->close();
   delete trainSeq;

   // load initial model
   HMM* hmm = new HMM();
   loadHMM( hmm, argv[2] );

   for(int i = 0; i < iter; ++i){

      // run every iteration
      double alpha[SEQ_LENGTH][MAX_STATE]{};
      double beta[SEQ_LENGTH][MAX_STATE]{};
      double epsilon[MAX_STATE][MAX_STATE]{};
      double gamma[MAX_STATE]{};
      double pi[MAX_STATE]{};
      double b[MAX_OBSERV][MAX_STATE]{};

      // run every sequences
      for(int current_seq = 0; current_seq < MAX_SEQ; ++current_seq){

         // initialization: alpha, beta
         int c = seqs[current_seq][0]-'A';
         for(int i = 0; i < MAX_STATE; ++i){
            alpha[0][i] = hmm->initial[i]*hmm->observation[c][i];
            beta[SEQ_LENGTH-1][i] = 1;
         }

         // induction, termination: alpha
         for(int t = 0; t < SEQ_LENGTH-1; ++t){
            c = seqs[current_seq][t+1]-'A';
            for(int j = 0; j < MAX_STATE; ++j){
               double nextAlpha = 0;
               for(int i = 0; i < MAX_STATE; ++i){
                  nextAlpha += alpha[t][i]
                               * hmm->transition[i][j];
               }
               nextAlpha *= hmm->observation[c][j];
               alpha[t+1][j] = nextAlpha;
            }
         }
         // induction, termination: beta
         for(int t = SEQ_LENGTH-1; t > 0; --t){
            c = seqs[current_seq][t]-'A';
            for(int i = 0; i < MAX_STATE; ++i){
               double nextBeta = 0;
               for(int j = 0; j < MAX_STATE; ++j){
                  nextBeta += beta[t][j]
                              * hmm->transition[i][j]
                              * hmm->observation[c][j];
               }
               beta[t-1][i] = nextBeta;
            }
         }

         // TODO: create: gamma, epsilon
         // p = P(O|Model)
         double p = 0;
         c = SEQ_LENGTH-1;
         for(int i = 0; i < MAX_STATE; ++i){
            p += alpha[c][i];
         }

         // pi[i] = gamma[0][i]/N
         for(int i = 0; i < MAX_STATE; ++i){
            pi[i] += alpha[0][i] * beta[0][i] / p;
         }

         // gamma = alpha*beta/p
         double gamma_sum[MAX_STATE]{};
         for(int t = 0; t < SEQ_LENGTH-1; ++t){
            c = seqs[current_seq][t]-'A';
            for(int i = 0; i < MAX_STATE; ++i){
               gamma[i] += alpha[t][i] * beta[t][i] / p;
               b[c][i] += alpha[t][i] * beta[t][i] / p;
            }
         }

         // epsilon = alpha[t][i] * a[i][j] * b[t+1][j] * beta[t+1][j]
         double epsilon_sum[MAX_STATE][MAX_STATE]{};
         for(int t = 0; t < SEQ_LENGTH-1; ++t){
            c = seqs[current_seq][t+1]-'A';
            for(int i = 0; i < MAX_STATE; ++i){
               for(int j = 0; j < MAX_STATE; ++j){
                  epsilon[i][j] += alpha[t][i]
                                       * hmm->transition[i][j]
                                       * hmm->observation[c][j]
                                       * beta[t+1][j]
                                       / p;
               }
            }
         }

      }

      // MAX_SEQ is done. make next model:
      for(int i = 0; i < MAX_STATE; ++i){
         hmm->initial[i] = pi[i] / MAX_SEQ;
         for(int j = 0; j < MAX_STATE; ++j){
            hmm->transition[i][j] = epsilon[i][j]/gamma[i];
         }
         for(int k = 0; k < MAX_OBSERV; ++k){
            hmm->observation[k][i] = b[k][i]/gamma[i];
         }
      }

      // one iteration done.
   }

   // test


   // dump the trained model to file
   FILE* pfile;
   pfile = fopen(argv[4], "w");
   dumpHMM( pfile, hmm );
   return 0;

}
