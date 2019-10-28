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
   char** seqs = new char*[MAX_SEQ];
   for(int i = 0; i < MAX_SEQ; ++i){
      seqs[i] = new char[SEQ_LENGTH];
      trainSeq->getline(seqs[i], SEQ_LENGTH+1);
   }
   trainSeq->close();
   delete trainSeq;

   // load initial model
   HMM* hmm_initial = new HMM();
   loadHMM( hmm_initial, argv[2] );

   // declare forward/backward etc. variables
   double** alpha   = new double*[SEQ_LENGTH];
   double** beta    = new double*[SEQ_LENGTH];
   double* gamma    = new double[MAX_STATE]{};
   double** epsilon = new double*[MAX_STATE];
   for(int i = 0; i < SEQ_LENGTH; ++i){
      alpha[i]   = new double[MAX_STATE];
      beta[i]    = new double[MAX_STATE];
      if(i < MAX_STATE) epsilon[i] = new double[MAX_STATE]{};
   }

   for(int i = 0; i<iter; ++i){
      // run every iteration
      double pi[MAX_STATE]{};
      double b[MAX_OBSERV][MAX_STATE]{};
      // run every sequences
      for(int current_seq = 0; current_seq < MAX_SEQ; ++current_seq){

         double p = 0;

         // TODO: initializtion: beta
         // initialization: alpha, beta
         for(int current_state = 0; current_state < MAX_STATE; ++current_state){
            alpha[0][current_state] = hmm_initial->initial[current_state]
                                      *hmm_initial->observation[seqs[current_seq][0]-'A'][current_state];
            beta[SEQ_LENGTH-1][current_state] = 1;
         }

         // TODO: induction, termination: beta
         // induction, termination: alpha, beta
         for(int current_ob = 1; current_ob < SEQ_LENGTH; ++current_ob){
            for(int current_state = 0; current_state < MAX_STATE; ++current_state){
               double sum_alpha = 0, sum_beta = 0;
               for(int s = 0; s < MAX_STATE; ++s){
                  sum_alpha += alpha[current_ob-1][s]*hmm_initial->transition[s][current_state];
                  sum_beta += beta[SEQ_LENGTH-current_ob][s]
                              *hmm_initial->transition[current_state][s]
                              *hmm_initial->observation[seqs[current_seq][SEQ_LENGTH-current_ob]-'A'][s];
               }
               alpha[current_ob][current_state] = sum_alpha*hmm_initial->observation[seqs[current_seq][current_ob]-'A'][current_state];
               beta[SEQ_LENGTH-1-current_ob][current_state] = sum_beta;
            }
         }

         // TODO: create: gamma, epsilon
         // p = P(O|Model)
         for(int current_state = 0; current_state < MAX_STATE; ++current_state){
            p += alpha[SEQ_LENGTH-1][current_state];
         }
         // for gamma, epsilon
         for(int current_ob = 0; current_ob < SEQ_LENGTH-1; ++current_ob){
            for(int current_state = 0; current_state < MAX_STATE; ++current_state){
               gamma[current_state] += alpha[current_ob][current_state]
                                       * beta[current_ob][current_state]
                                       / p;
               b[seqs[current_seq][current_ob]-'A'][current_state] += alpha[current_ob][current_state]
                                                                      * beta[current_ob][current_state]
                                                                      / p;
               if(current_ob == 0) pi[current_state] += alpha[current_ob][current_state]
                                                        * beta[current_ob][current_state]
                                                        / p;
               for(int s = 0; s < MAX_STATE; ++s){
                  epsilon[current_state][s] += alpha[current_ob][current_state]
                                               * hmm_initial->transition[current_state][s]
                                               * hmm_initial->observation[seqs[current_seq][current_ob+1]-'A'][s]
                                               * beta[current_ob+1][s]
                                               / p;
               }
            }
         }
         // for last gamma
         for(int current_state = 0; current_state < MAX_STATE; ++current_state){
            gamma[current_state] += alpha[SEQ_LENGTH-1][current_state]
                                    * beta[SEQ_LENGTH-1][current_state]
                                    / p;
         }

      }
      for(int current_state = 0; current_state < MAX_STATE; ++current_state){
         hmm_initial->initial[current_state] = pi[current_state] / MAX_SEQ;
         for(int s = 0; s < MAX_STATE; ++s){
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
