//     ./test <models_list_path> <seq_path> <output_result_path>
// ex: ./test ./modellist.txt ./data/test_seq.txt ./result.txt
// ex: ./test ./test_lbl.txt ./data/test_seq.txt ./result.txt
//
// ./result.txt
//     <trained_model_path> <max P(O|trained_model, state_sequence)>
// ex: model_01.txt 7.822367e-34
//     model_05.txt 1.094896e-40

#include <iostream>
#include <fstream>
#include "../inc/hmm.h"

using namespace std;

int main(int argc, char const *argv[]) {
   cout << argc << argv[0] << argv[1] << endl;
   return 0;
}
