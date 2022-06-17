# MOSA-EA
Multi-Objective Self-Adaptive EA (MOSA-EA) *(Lehre and Qin, 2022)*, was proposed to optimise single-objective functions, which treats parameter control from multi-objectivisation. The algorithm maximises the fitness and the mutation rates simultaneously, allowing individuals in "dense" fitness valleys and on "sparse" local optima to co-exist on a non-dominated Pareto front.  

Please find more in the publication below.

* **publication**:

  [https://www.cs.bham.ac.uk/~xxq896/gecco2022-mosaea.pdf](https://www.cs.bham.ac.uk/~xxq896/gecco2022-mosaea.pdf)
  

## Example
To compile it:  
```console
make
```

The arguments in this code:  
>`-n` is problem size.  
 `-f` is id of fitness function (`1` is OneMax, and `2` is LeadingOnes).  
 `-A` is A in MOSA-EA (A float value greater than 1; we recommend to set as `1.01`).   
 `-p` is p<sub>inc</sub> in MOSA-EA (A float value between 0 and 1; we recommend to set as `0.4`).   
 `-c` is minimal mutation rate parameter χ<sub>min</sub> in MOSA-EA (A float value greater than 0; we recommend to set as `0.5/log(n)`).  
 `-l` is population size (λ) in MOSA-EA (A int value greater than 1; we recommend to set as `10000 ln(n)`).  
 `-m` is μ in (μ,λ) selection in MOSA-EA (A int value greater than 1 and less than λ; we recommend to set as `λ/8`).  
 `-e` is the maximum of the number of evaluations.  
    
To run it:  
```console
./mosa-ea -n 100 -A 1.01 -p 0.4 -c 0.1 -l 20000 -m 2500 -f 2 -e 100000000
```
which is to optimise LeadingOnes (n=100) in 100000000 evaluations by using the recommanded parameter setting.  
  
You also can add a customised fitness function by adding a global function and giving an id in Class `Evaluation`. Please follow examples of OneMax and LeadingOnes in the code.  


## Contact

If you have any questions, comments or suggestions, please don't hesitate to contact us:


* [Per Kristian Lehre](https://www.cs.bham.ac.uk/~lehrepk/), *School of Computer Science, University of Birmingham*,
* [Xiaoyu Qin](https://www.cs.bham.ac.uk/~xxq896/), *School of Computer Science, University of Birmingham*.


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
