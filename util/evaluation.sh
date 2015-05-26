./trec_eval/trec_eval ../data/relevantes-34/results.test ../data/relevantes-34/results.vsm > output.vsm
./trec_eval/trec_eval ../data/relevantes-34/results.test ../data/relevantes-34/results.pgm > output.pgm
./trec_eval/trec_eval ../data/relevantes-34/results.test ../data/relevantes-34/results.atm > output.atm
./trec_eval/trec_eval ../data/relevantes-34/results.test ../data/relevantes-34/results.linear > output.linear
python plot-trec_eval/plot_pr_curve.py -f pr_curve.pdf output.linear output.vsm output.pgm output.atm
