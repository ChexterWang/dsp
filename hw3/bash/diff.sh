#!bin/bash

printf "" > ../out/diff.txt
for i in {1..10}
do
   diff ../out/result_srilm/$i.txt ../out/result_mydisambig/$i.txt -y | grep -a -e '|	' >> ../out/diff.txt
done
