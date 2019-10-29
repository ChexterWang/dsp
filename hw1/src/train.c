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
      // cout << trainSeq->bad() << endl;
   }
   trainSeq->close();
   delete trainSeq;

   // load initial model
   HMM* hmm_initial = new HMM();
   loadHMM( hmm_initial, argv[2] );


   for(int i = 0; i<iter; ++i){

      // run every iteration
      double alpha[SEQ_LENGTH][MAX_STATE]{};
      double beta[SEQ_LENGTH][MAX_STATE]{};
      double epsilon[MAX_STATE][MAX_STATE]{};
      double gamma[MAX_STATE]{};
      double pi[MAX_STATE]{};
      double b[MAX_OBSERV][MAX_STATE]{};

      // run every sequences
      for(int current_seq = 0; current_seq < MAX_SEQ; ++current_seq){

         // TODO: initializtion: beta
         // initialization: alpha, beta
         int c = seqs[current_seq][0]-'A';
         for(int i = 0; i < MAX_STATE; ++i){
            alpha[0][i] = hmm_initial->initial[i]*hmm_initial->observation[c][i];
            beta[SEQ_LENGTH-1][i] = 1;
         }

         // TODO: induction, termination: beta
         // induction, termination: alpha, beta
         for(int t = 1; t < SEQ_LENGTH; ++t){
            c = seqs[current_seq][t-1]-'A';
            int d = seqs[current_seq][SEQ_LENGTH-t]-'A';
            int e = SEQ_LENGTH-t;
            int g = SEQ_LENGTH-1-t;
            for(int j = 0; j < MAX_STATE; ++j){
               double sum_alpha = 0, sum_beta = 0;
               for(int i = 0; i < MAX_STATE; ++i){
                  sum_alpha += alpha[t-1][i]*hmm_initial->transition[i][j];
                  sum_beta += beta[e][i]
                              * hmm_initial->transition[j][i]
                              * hmm_initial->observation[d][i];
               }
               alpha[t][j] = sum_alpha*hmm_initial->observation[c][j];
               beta[g][j] = sum_beta;
            }
         }

         // TODO: create: gamma, epsilon
         // p = P(O|Model)
         double p = 0;
         c = SEQ_LENGTH-1;
         for(int current_state = 0; current_state < MAX_STATE; ++current_state){
            p += alpha[c][current_state];
         }
         // for gamma, epsilon
         for(int current_ob = 0; current_ob < SEQ_LENGTH-1; ++current_ob){
            c = seqs[current_seq][current_ob+1]-'A';
            int d = current_ob+1;
            int e = seqs[current_seq][current_ob]-'A';
            for(int current_state = 0; current_state < MAX_STATE; ++current_state){
               double GAMMA = alpha[current_ob][current_state]*beta[current_ob][current_state]/p;
               gamma[current_state] += GAMMA;
               b[e][current_state] += GAMMA;
               if(current_ob == 0) pi[current_state] += GAMMA;
               for(int s = 0; s < MAX_STATE; ++s){
                  epsilon[current_state][s] += alpha[current_ob][current_state]
                                               * hmm_initial->transition[current_state][s]
                                               * hmm_initial->observation[c][s]
                                               * beta[d][s]
                                               / p;
               }
            }
         }
         // for last gamma
         // c = SEQ_LENGTH-1;
         // for(int current_state = 0; current_state < MAX_STATE; ++current_state){
         //    gamma[current_state] += alpha[c][current_state]
         //                            * beta[c][current_state]
         //                            / p;
         // }

      }
      for(int current_state = 0; current_state < MAX_STATE; ++current_state){
         hmm_initial->initial[current_state] = pi[current_state] / MAX_SEQ;
         for(int s = 0; s < MAX_STATE; ++s){
            //cout << epsilon[current_state][s] << ", " << gamma[current_state] << endl;
            hmm_initial->transition[current_state][s] = epsilon[current_state][s] / gamma[current_state];
         }
         for(int o = 0; o < MAX_OBSERV; ++o){
            hmm_initial->observation[o][current_state] = b[o][current_state] / gamma[current_state];
         }
      }
   }

   // test


   // dump the trained model to file
   FILE* pfile;
   pfile = fopen(argv[4], "w");
   dumpHMM( pfile, hmm_initial );
   return 0;

}
