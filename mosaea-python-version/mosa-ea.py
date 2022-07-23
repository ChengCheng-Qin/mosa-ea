# Copyright (C) 2022 by Xiaoyu QIN @ Unversity of Birmingham
# This code require ioh>=0.3.2.8.2 in pip: "pip install ioh"

import random
import numpy as np
import ioh
import os
import copy
import heapq
import time
import argparse


# Class of Individual which is used to store bitstring and index of mutation rate.
class Individual:
    n: int
    mutation_rate_index: int
    bitstring = []
    fitness: float
    
    # Initialisation function for Individual
    def __init__(self, n, num_mutation_rates):
        self.n = n
        # Choose a mutation rate uniformly at random
        self.mutation_rate_index = random.randint(0, num_mutation_rates)
        # Generate a bitstring uniformly at random
        self.bitstring = np.random.randint(0, 2, (self.n))
        self.fitness = 0.0
    
    # Mutation of bitstring of an individual 
    def mutate_bistring(self, mutation_rate):
        # Mutate individual with bitwise mutation operator
        flip_position = np.random.binomial(1, mutation_rate, self.n)
        for i in range(0, self.n):
            if flip_position[i] == 1:
                self.bitstring[i] = 1 - self.bitstring[i]

    # Mutation of mutation rate (index) of a individual
    def mutate_mutation_rate_index(self, para_pinc, num_mutation_rates):
        # With probability pinc, increase the mutation rate by multiplying A, i.e., increase index, otherwise decrease by multiplying 1/A, i.e., decrease index
        if random.random() < para_pinc:
            self.mutation_rate_index = min(self.mutation_rate_index + 1, num_mutation_rates - 1)
        else:
            self.mutation_rate_index = max(0, self.mutation_rate_index - 1)

    # Compare individual only based on mutation rate, i.e., higher mutation rate index individual "less than " lower mutation rate index individual
    def __lt__(self, other):
         return self.mutation_rate_index > other.mutation_rate_index
        
# Class of population
class Population:
    n: int
    population_size: int
    individuals = []
    sorted_individuals = []
    offspring_individuals = []

    # Initialisation of population
    def __init__(self, n, num_mutation_rates, population_size):
        self.n = n 
        self. population_size = population_size
        # Generate population_size inidividuals
        self.individuals = [Individual(n, num_mutation_rates) for _ in range(0, self.population_size)]
        self.offspring_individuals = copy.deepcopy(self.individuals)
        self.sorted_individuals = copy.deepcopy(self.individuals)

    # Mutation of population by individual
    def mutating_population(self, para_pinc, num_mutation_rates, mutation_rates):
        # For each individual, mutate its mutation rate then mutate its bitstring
        for i in range(0, self.population_size):
            self.offspring_individuals[i].mutate_mutation_rate_index(para_pinc, num_mutation_rates)
            self.offspring_individuals[i].mutate_bistring(mutation_rates[self.offspring_individuals[i].mutation_rate_index])
        # Update individuals for the next generation
        self.individuals = self.offspring_individuals
    
    # Multiobjective sorting
    def multiobj_sroting(self):
        # Sort population based on fitness only
        self.individuals.sort(key=lambda s: s.fitness, reverse=True)
        
        heaps = list()

        # Make a heap for each fitness value
        current_fitness = self.individuals[0].fitness
        i = 0
        while (i < self.population_size):
            heap = list()
            while (i < self.population_size and self.individuals[i].fitness >= current_fitness):
                heap.append(self.individuals[i])
                i = i + 1
            heapq.heapify(heap)
            heaps.append(heap)            
            if (i < self.population_size):
                current_fitness = self.individuals[i].fitness

        # Sort population based on fronts
        i = 0
        while (i < self.population_size):
            # "Peal" a strict non-dominated front
            j = 0
            fitter_indi_mutation_rate_index = -1
            while (j < len(heaps)):
                if (len(heaps[j]) > 0 and heaps[j][0].mutation_rate_index > fitter_indi_mutation_rate_index):
                    # Add "top" individual into array sorted_individuals
                    self.sorted_individuals[i] = copy.deepcopy(heaps[j][0])
                    i = i + 1
                    
                    fitter_indi_mutation_rate_index = heaps[j][0].mutation_rate_index
                    
                    # Remove "top" individual from its heap
                    heapq.heappop(heaps[j])
                j = j + 1
        self.individuals = self.sorted_individuals

    # Select population using (mu, lambda) selection
    def comma_selection(self, pop_mu):
        for i in range(0, self.population_size):
            random_index = random.randint(0, pop_mu)
            self.offspring_individuals[i] = copy.deepcopy(self.individuals[random_index])

class MOSAEA:
    'MOSA-EA'
    
    # Parameter settings in MOSA-EA
    n: int
    population_size: int
    pop_mu: int
    para_pinc: float
    para_A: float
    para_min_mutation_para: float
    mutation_rates = []
    num_mutation_rates = []
    population = []
    max_evaluations: int
    
    def __init__(self, n, population_size, pop_mu, para_pinc, para_A, para_min_mutation_para, max_evaluations):
        self.n = n
        self.population_size = population_size
        self.pop_mu = pop_mu
        self.para_pinc = para_pinc
        self.para_A = para_A
        self.para_min_mutation_para = para_min_mutation_para
        self.max_evaluations = max_evaluations
        
        # Build a array containing all possible mutation rates
        i = 0
        mutation_rate = self.para_min_mutation_para / self.n
        # Compute number of possible mutation rates
        while (mutation_rate * self.para_A <= 0.5):
            mutation_rate = mutation_rate * self.para_A
            i = i + 1
        self.num_mutation_rates = i + 1
        
        # Enter each mutation rate into array
        self.mutation_rates = [0.0] * self.num_mutation_rates
        mutation_rate = self.para_min_mutation_para / self.n
        for i in range(0, self.num_mutation_rates):
            self.mutation_rates[i] = mutation_rate
            mutation_rate = mutation_rate * self.para_A

        # Initialise population
        self.population = Population(self.n, self.num_mutation_rates, self.population_size)


    def __call__(self, problem: ioh.problem.PBO) -> None:
        num_generation = 0

        # Set a clock to record running time
        start = time.time()

        # Reset MOSA-EA
        self.__init__(self.n, self.population_size, self.pop_mu, self.para_pinc, self.para_A, self.para_min_mutation_para, self.max_evaluations)
        

        while(problem.state.evaluations <= self.max_evaluations):
            num_generation = num_generation + 1
            print("Gen' " + str(num_generation) + " (Evaluation: " + str(problem.state.evaluations) + ")" + ": ", end = "", flush=True)
            # Evaluate population by given fitness function of PBO in ioh
            for i in range(0, self.population_size):
                fitness_value = problem(self.population.individuals[i].bitstring)
                self.population.individuals[i].fitness = fitness_value
            # Sort population by multi-objective sorting
            self.population.multiobj_sroting()
            print("Best Fitness: " + str(problem.state.current_best.y.__float__()) + ", its mutation para': " + str((self.mutation_rates[self.population.individuals[0].mutation_rate_index] * self.n)), flush=True)

            # Check if optimal found
            if (problem.state.optimum_found):
                break
            
            # Select population by (mu, lambda) selection
            self.population.comma_selection(self.pop_mu)

            # Mutate selected population
            self.population.mutating_population(self.para_pinc, self.num_mutation_rates, self.mutation_rates)

        # Show running time for each run
        end = time.time()
        print("Total Running Time: \t\t\t" + str(end - start))
        print("Runing Time Per Generation: \t" + str((end - start) / num_generation))

            
def run_experiment(problem, algorithm, n_runs=1):
    for run in range(n_runs):
        # Run the algorithm on the problem
        algorithm(problem)
        # print the best found for this run
        print(f"run: {run+1} - best found:{problem.state.current_best.y: .3f}")
        # Reset the problem
        problem.reset()


# Main function
if __name__ == '__main__':
    # Read parameters from command line
    # -n is problem size n
    # -f is id of fitness function
    # -A is A in MOSA-EA para_A
    # -p is pinc in MOSA-EA para_pinc
    # -c is minimal mutation rate parameter in MOSA-EA para_min_mutation_para
    # -l is population size (lambda) in MOSA-EA population_size
    # -m is mu in (mu, lambda) in MOSA-EA pop_mu
    # -e is maximum of number of evaluations
    # -r is number of runs
    # -log is if turn on ioh logger: 1 is on and 0 is off

    parser = argparse.ArgumentParser()
    parser.add_argument("-n", help="Problem size", type=int)
    parser.add_argument("-f", help="Function ID of PBO in ioh", type=int)
    parser.add_argument("-A", help="MOSA-EA para_A", type=float, nargs='?', const=1.01, default=1.01)
    parser.add_argument("-p", help="MOSA-EA para_pinc", type=float, nargs='?', const=0.4, default=0.4)
    parser.add_argument("-c", help="MOSA-EA para_min_mutation_para", type=float, nargs='?', const=0.1, default=0.1)
    parser.add_argument("-l", help="MOSA-EA population_size (lambda in (mu, lambda) selection)", type=int, nargs='?', const=5000, default=5000)
    parser.add_argument("-m", help="MOSA-EA pop_mu (lambda in (mu, lambda) selection)", type=int, nargs='?', const=625, default=625)
    parser.add_argument("-e", help="Maximum of number of evaluations", type=int)
    parser.add_argument("-r", help="Runs", type=int, nargs='?', const=1, default=1)
    parser.add_argument("-log", help="Turn on ioh logger and store log into a file", type=int, nargs='?', const=0, default=0)
    args = parser.parse_args()

    n = args.n
    ffid = args.f
    para_A = args.A
    para_pinc = args.p
    para_min_mutation_para = args.c
    population_size = args.l
    pop_mu = args.m
    max_evaluations = args.e
    islog = args.log
    runs = args.r

    print("MOSA-EA with population_size=" + str(population_size) + ", pop_mu=" + str(pop_mu) + " para_A=" + str(para_A) + " para_pinc=" + str(para_pinc) + " para_min_mutation_para=" + str(para_min_mutation_para))
    print("On functions id=" + str(ffid) + " n=" + str(n))
    print("With max_evaluations=" + str(max_evaluations))
    print("Runs=" + str(runs) + "\n")

    # Set function of PBO in ioh
    problem = ioh.get_problem(
        fid=ffid, 
        instance=1,
        dimension=n,
        problem_type="PBO"
    )
    problem

    # Add logger
    if (islog != 0):
        logger = ioh.logger.Analyzer(
            root=os.getcwd(),
            folder_name="Exp-PBO-Fid-" + str(ffid),
            # Meta-data for the algorithm used to generate these results
            algorithm_name="mosaea",
            # Store x-variables in the logged files
            store_positions=False
        )
        problem.attach_logger(logger)

    # Start running
    run_experiment(problem, MOSAEA(n, population_size, pop_mu, para_pinc, para_A, para_min_mutation_para, max_evaluations), runs)