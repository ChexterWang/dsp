#!/bin/bash
echo 'Generating decoded file...'
for i in {1..10}
do
   ../mydisambig ../out/separated_txt/$i.txt ../ZhuYin-Big5.map \
   ../out/corpus_lm.txt ../out/result_mydisambig/$i.txt
done
