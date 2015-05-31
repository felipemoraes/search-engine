./trec_eval/trec_eval ../data/relevantes-34/results.test ../data/relevantes-34/results.vsm > output.vsm
./trec_eval/trec_eval ../data/relevantes-34/results.test ../data/relevantes-34/results.vsm_pgm > output.vsm_pgm
./trec_eval/trec_eval ../data/relevantes-34/results.test ../data/relevantes-34/results.vsm_atm > output.vsm_atm
./trec_eval/trec_eval ../data/relevantes-34/results.test ../data/relevantes-34/results.linear > output.linear
python plot-trec_eval/plot_pr_curve.py -f pr_curve.pdf output.vsm output.vsm_pgm output.vsm_atm output.linear
