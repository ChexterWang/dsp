#!/bin/bash

echo 'mkdir...'
mkdir ../out
mkdir ../out/result_srilm

echo 'Generating decoded file...'
for i in {1..10}
do
   ../srilm-1.5.10/bin/i686-m64/disambig -text ../out/separated_txt/$i.txt -map ../ZhuYin-Big5.map \
   -lm ../out/corpus_lm.txt -order 3 > ../out/result_srilm/$i.txt
done
echo 'done!'
