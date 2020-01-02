#!/bin/bash

echo 'mkdir...'
mkdir ../out
mkdir ../out/result_mydisambig

echo 'Generating decoded file...'
for i in {1..10}
do
   ../mydisambig ../out/separated_txt/$i.txt ../ZhuYin-Big5.map \
   ../out/corpus_lm.txt ../out/result_mydisambig/$i.txt
done
echo 'done!'
