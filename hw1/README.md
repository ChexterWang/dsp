# hw1
### Usage
- ```make clean```

- ```make```

- ```./train <iter> <model_init_path> <seq_path> <output_model_path>``` : train a single model

- ```./test <models_list_path> <seq_path> <output_result_path>``` : test test_seq with models in models_list

- ```./run train``` : trains initial model using train sequences in /data and output trained models to file model_xx.txt

- ```./run test``` : tests sequences using model listed in modellist.txt and output the most fit model to ./result.txt

- ```./run $1 $2 $3``` : trains model with number of iterations change from $1 to $3 with diff $2 and output accuracy to ./out.csv (<#>,<accuracy>)

- ```./accuracy``` : echo accuracy of current result.txt (compare to ./data/text_lbl.txt)

- ```python ./util/plot.py``` : use plotly to plot accuracy-iteration relation in out.csv file; will show in browser and store in ./plot.svg

- ```output.csv``` : example csv of accuracy-iteration relation