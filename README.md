# MOSA-EA
Multi-Objective Self-Adaptive EA (MOSA-EA) *(Lehre and Qin, 2022)*, was proposed to optimise single-objective Pseudo-Boolean functions, which treats parameter control from multi-objectivisation. The algorithm maximises the fitness and the mutation rates simultaneously, allowing individuals in "dense" fitness valleys and on "sparse" local optima to co-exist on a non-dominated Pareto front.  


Please find more in the publication below.

* Publication:

  GECCO '22: [https://research.birmingham.ac.uk/en/publications/self-adaptation-via-multi-objectivisation-a-theoretical-study](https://research.birmingham.ac.uk/en/publications/self-adaptation-via-multi-objectivisation-a-theoretical-study)
  
  PPSN '22: [https://research.birmingham.ac.uk/en/publications/self-adaptation-via-multi-objectivisation-an-empirical-study](https://research.birmingham.ac.uk/en/publications/self-adaptation-via-multi-objectivisation-an-empirical-study)

## Example - Python Version
First install [IOHprofiler](https://iohprofiler.github.io) via pip. IOHprofiler is a benchmarking platform for evaluating the performance of iterative optimization heuristics.
```console
pip install ioh
```
The arguments the python code:  
>`-n` is problem size.  
 `-f` is id of fitness function (A int value from 1 to 25. Here we use PBO problem set in IOHprofiler, for information of function ids please see [IOHproblem](https://iohprofiler.github.io/IOHproblem/)).  
 `-A` is A in MOSA-EA (A float value greater than 1; we recommend to set as `1.01`).   
 `-p` is p<sub>inc</sub> in MOSA-EA (A float value between 0 and 1; we recommend to set as `0.4`).   
 `-c` is minimal mutation rate parameter χ<sub>min</sub> in MOSA-EA (A float value greater than 0; we recommend to set as `0.5/log(n)` under noise).  
 `-l` is population size (λ) in MOSA-EA (A int value greater than 1; we recommend to set as `5000 log(n)`).  
 `-m` is μ in (μ,λ) selection in MOSA-EA (A int value greater than 1 and less than λ; we recommend to set as `λ/8`).  
 `-e` is the maximum of the number of evaluations.  
 `-r` is the number of runs.  
 `-log` is to turn on logger to record optimisation progress via IOHprofiler (`1` is on and `0` is off).

To run it:  
```console
cd mosaea-python-version
python mosa-ea.py -n 50 -f 2 -A 1.01 -p 0.4 -c 0.1 -l 5000 -m 625 -e 100000000 -r 1 -log 0
```


## Example - C++ Version
To compile it:  
```console
cd mosaea-cpp-version
make
```

The arguments in the C++ code:  
>`-n` is problem size.  
 `-f` is id of fitness function (`1` is OneMax, and `2` is LeadingOnes).  
 `-A` is A in MOSA-EA (A float value greater than 1; we recommend to set as `1.01`).   
 `-p` is p<sub>inc</sub> in MOSA-EA (A float value between 0 and 1; we recommend to set as `0.4`).   
 `-c` is minimal mutation rate parameter χ<sub>min</sub> in MOSA-EA (A float value greater than 0; we recommend to set as `0.5/log(n)`).  
 `-l` is population size (λ) in MOSA-EA (A int value greater than 1; we recommend to set as `5000 log(n)`).  
 `-m` is μ in (μ,λ) selection in MOSA-EA (A int value greater than 1 and less than λ; we recommend to set as `λ/8`).  
 `-e` is the maximum of the number of evaluations.  
    
To run it:  
```console
./mosa-ea -n 50 -f 2 -A 1.01 -p 0.4 -c 0.1 -l 5000 -m 625 -e 100000000
```
which is to optimise LeadingOnes (n=100) in 100000000 evaluations by using the recommanded parameter setting.  
  
You also can add a customised fitness function by adding a global function and giving an id in Class `Evaluation`. Please follow examples of OneMax and LeadingOnes in the code.  


## Contact

If you have any questions, comments or suggestions, please don't hesitate to contact us:


* [Xiaoyu Qin](https://www.cs.bham.ac.uk/~xxq896/), *School of Computer Science, University of Birmingham*.
* [Per Kristian Lehre](https://www.cs.bham.ac.uk/~lehrepk/), *School of Computer Science, University of Birmingham*,



When using MOSA-EA and parts thereof, please kindly cite this work as

```bibtex
@inproceedings{mosa-ea-theory,
	title = {Self-adaptation to Multi-objectivisation: A Theoretical Study},
	booktitle = {Proceedings of the Genetic and Evolutionary Computation Conference},
	publisher = {ACM},
	author = {Lehre, Per Kristian and Qin, Xiaoyu},
	year = {2022},
}

@inproceedings{mosa-ea-empirical,
	title = {Self-adaptation to Multi-objectivisation: An Empirical Study},
	booktitle = {Proceedings of the Parallel Problem Solving from Nature},
	publisher = {Springer},
	author = {Qin, Xiaoyu and Lehre, Per Kristian},
	year = {2022},
}
```

## License

`MOSA-EA` is released under the MIT license. See [LICENSE](LICENSE) for additional details about it.
