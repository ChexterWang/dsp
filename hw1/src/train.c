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
#define MAX_SEQ 10

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
   double** gamma   = new double*[SEQ_LENGTH];
   double** epsilon = new double*[SEQ_LENGTH];
   for(int i = 0; i < SEQ_LENGTH; ++i){
      alpha[i]   = new double[MAX_STATE];
      beta[i]    = new double[MAX_STATE];
      gamma[i]   = new double[MAX_STATE];
      epsilon[i] = new double[MAX_STATE];
   }

   // iteration
   for(int current_iter = 0; current_iter < iter; ++current_iter){

      cout << seqs[0] << endl;
      // initialization
      for(int current_state = 0; current_state < MAX_STATE; ++current_state){
         alpha[0][current_state] = hmm_initial->initial[current_state]
                                   *hmm_initial->observation[seqs[0][0]-'A'][current_state];
      }

      // induction
      for(int current_ob = 1; current_ob < SEQ_LENGTH; ++current_ob){
         for(int current_state = 0; current_state < MAX_STATE; ++current_state){
            double sum_alpha = 0;
            for(int s = 0; s < MAX_STATE; ++s){
               sum_alpha += alpha[current_ob-1][s]*hmm_initial->transition[s][current_state];
            }
            alpha[current_ob][current_state] = sum_alpha*hmm_initial->observation[seqs[0][current_ob]-'A'][current_state];
         }
      }

      // termination
      for(int current_state = 0; current_state < MAX_STATE; ++current_state){
         break;
      }

   }

   // test

   // dump the trained model to file
   FILE* pfile;
   pfile = fopen(argv[4], "w");
   dumpHMM( pfile, hmm_initial );
   return 0;

}
