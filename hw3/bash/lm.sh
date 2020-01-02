#!/bin/bash
echo 'Separating test data...'
for i in {1..10}
do
   perl ../separator_big5.pl ../test_data/$i.txt > ../out/separated_txt/$i.txt
done

echo 'Separating corpus...'
perl ../separator_big5.pl ../corpus.txt > ../out/corpus.txt

echo 'Generating count file...'
../srilm/bin/i686-m64/ngram-count -text ../out/corpus.txt -write ../out/corpus_count.txt -order 3

echo 'Generating language model file...'
../srilm/bin/i686-m64/ngram-count -read ../out/corpus_count.txt -lm ../out/corpus_lm.txt -order 3 -unk
