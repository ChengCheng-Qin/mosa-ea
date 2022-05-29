#include<iostream>
#include<cmath>
#include<cstdlib>
#include <algorithm>
#include<vector>
#include<string.h>
#include<time.h>
#include <random>
#include <unistd.h>
#include <getopt.h>

// #include "functions.cpp"

using namespace std;

// Global parameters
int max_evaluation;
int population_size;
int pop_mu;
int n;

float para_A;
float para_pinc;
float para_min_mutation_para;
float *mutation_rates;
int num_mutation_rates;

// Set random real number function
random_device rd;
default_random_engine eng(rd());
uniform_real_distribution<> random_real(0, 1);

// Define fitness functions here as global functions
// ****** ADD CUSTOMISED FITNESS FUNCTIONS HERE ******
float leadingones(int n, int *bitstring)
{
    float fitness = 0.0;
    for(int i = 0; i < n; i++)
    {
        if (bitstring[i] == 0)
        {
            break;
        }
        fitness = fitness + 1.0;
    }
    return fitness;
}

float onemax(int n, int *bitstring)
{
    float fitness = 0.0;
    for(int i = 0; i < n; i++)
    {
        fitness = fitness + (float) bitstring[i];
    }
    return fitness;
}

// Class of Evaluation which use to set objective function, e.g., maximal fitness value.
class Evaluation
{
    public:
        float max_fitness;
        int id_function;
        Evaluation(int id);
        float evaluation_solution(int *bitstring);
};

// Initialisation function for Individual Evaluation (Set maximal fitness value and id of fitness function)
Evaluation::Evaluation(int id)
{
    id_function = id;
    max_fitness = float(n);
}

// Member function which use to evaluate a solution
// ****** ADD CUSTOMISED FITNESS FUNCTION ID HERE ******
float Evaluation::evaluation_solution(int *bitstring)
{
    float fitness;
    switch (id_function)
    {
        case 1:
            fitness = onemax(n, bitstring);
            break;
        case 2:
            fitness = leadingones(n, bitstring);
            break;
        default:
            cout << "No fitness function defined." << endl;
            exit(1);
    }
    return (fitness);
}


// Class of Individual which is used to store bitstring and index of mutation rate.
class Individual
{
    public:
        int index_mutation_rate;
        int *bitstring;
        float fitness;
        Individual();
        void mutating_bitstring(float mutation_rate);
        void mutating_index_mutation_rate(float pinc);
        void copy_individual(Individual & indi);   
};

// Initialisation function for Individual
Individual::Individual()
{
    // Choose a mutation rate uniformly at random
    index_mutation_rate = rand() % num_mutation_rates;
    bitstring = new int[n];
    // Generate a bitstring uniformly at random
    for (int i = 0; i < n; i++)
    {
        bitstring[i] = (rand() % 2);
    }
}

// Mutation of bitstring of a individual 
void Individual::mutating_bitstring(float mutation_rate)
{
    // Bitwisely mutate bitstring with mutation rate
    for (int i = 0; i < n; i++)
    {
        if (random_real(eng) < mutation_rate)
        {
            bitstring[i] = (bitstring[i] + 1) % 2;
        }
    }
}

void Individual::copy_individual(Individual & indi)
{
    index_mutation_rate = indi.index_mutation_rate;
    fitness = indi.fitness;
    for (int i = 0; i < n; i++)
    {
        bitstring[i] = indi.bitstring[i];
    }
}

// Mutation of mutation rate (index) of a individual
void Individual::mutating_index_mutation_rate(float para_pinc)
{
    // With probability pinc, increase mutation rate by timing A (increase index), otherwise decrease by timing 1/A (decrease index)
    if (random_real(eng) < para_pinc)
    {
        index_mutation_rate = min(index_mutation_rate + 1, num_mutation_rates - 1);
    }
    else
    {
        index_mutation_rate = max(0, index_mutation_rate - 1);
    }
}

// Class of population
class Population
{
    public:
        vector <Individual> individuals;
        vector <Individual> sorted_individuals;
        vector <Individual> offspring_individuals;
        Population();
        void mutating_population();
        void multiobj_sorting();
        void comma_selection();
};

// Initialisation of population
Population::Population()
{
    // Generate population_size inidividuals
    for (int i = 0; i < population_size; i++)
    {
        individuals.push_back(Individual());
        sorted_individuals.push_back(Individual());
        offspring_individuals.push_back(Individual());
    }
}

// Mutation of population by individual
void Population::mutating_population()
{
    // For each individual, mutate its mutation rate then mutate its bitstring
    for (int i = 0; i < population_size; i++)
    {
        offspring_individuals[i].mutating_index_mutation_rate(para_pinc);
        offspring_individuals[i].mutating_bitstring(mutation_rates[offspring_individuals[i].index_mutation_rate]);
    }
    // Update individuals for the next generation
    individuals = offspring_individuals;
}

// Compare individual only based on fitness
bool compare_individual_based_fitness(Individual x, Individual y)
{
    return (x.fitness > y.fitness);
}

// Compare individual only based on mutation rate
bool compare_individual_based_index_mutation_rate(Individual x, Individual y)
{
    return (x.index_mutation_rate < y.index_mutation_rate);
}

// Multiobjective sorting
void Population::multiobj_sorting()
{
    int i, j;

    // Sort population based on fitness only
    sort(individuals.begin(), individuals.end(), compare_individual_based_fitness);

    // vector<Individual> sorted_individuals;
    vector<vector<Individual> > heaps;
    
    // Make a heap for each value of fitness 
    float current_fitness = individuals[0].fitness;
    i = 0;
    while (i < population_size)
    {
        vector<Individual> heap;
        while (i < population_size && individuals[i].fitness >= current_fitness)
        {
            heap.push_back(individuals[i]);
            i++;
        }
        make_heap(heap.begin(), heap.end(), compare_individual_based_index_mutation_rate);
        heaps.push_back(heap);
        current_fitness = individuals[i].fitness;
    }

    // Sort population based on fronts
    i = 0;
    int fitter_indi_index_mutation_rate;
    while (i < population_size)
    {
        // "Peal" a strict non-dominated front
        j = 0;
        fitter_indi_index_mutation_rate = -1;
        while (j < heaps.size())
        {
            if (heaps[j].size() > 0 && heaps[j].front().index_mutation_rate > fitter_indi_index_mutation_rate)
            {
                // Add "top" individual into array sorted_individuals
                sorted_individuals[i].copy_individual(heaps[j].front());
                i++;

                fitter_indi_index_mutation_rate = heaps[j].front().index_mutation_rate;

                // Remove "top" individual from its heap
                pop_heap(heaps[j].begin(), heaps[j].end(), compare_individual_based_index_mutation_rate);
                heaps[j].pop_back();
            }
            j++;
        }
    }
    individuals = sorted_individuals;
}

// Select population using (mu, lambda) selection
void Population::comma_selection()
{
    for (int i = 0; i < population_size; i++)
    {
        int random_index = rand() % pop_mu;
        offspring_individuals[i].copy_individual(individuals[random_index]);
    }
}


int main (int argc, char* argv[])
{
    int i;

    // We set a Default Parameter Setting and an example problem here

    // Set maximum of number of evaluations
    max_evaluation = 100000000;

    // Problem setting
    n = 100;

    // Parameter setting in MOSA-EA (Default)
    para_A = 1.01;
    para_pinc = 0.4;
    para_min_mutation_para = 0.5 / log(n);
    population_size = 20000;
    pop_mu = population_size / 8;

    // Read parameters from command line
    // -n is problem size n
    // -f is id of fitness function
    // -A is A in MOSA-EA para_A
    // -p is pinc in MOSA-EA para_pinc
    // -c is minimal mutation rate parameter in MOSA-EA para_min_mutation_para
    // -l is population size (lambda) in MOSA-EA population_size
    // -m is mu in (mu, lambda) in MOSA-EA pop_mu
    // -e is maximum of number of evaluations
    int o;
    int id_function;
    const char *optstring = "n:A:p:c:l:m:f:e:";
    while ((o = getopt(argc, argv, optstring)) != -1) 
    {
        switch (o) {
            case 'n':
                n = atoi(optarg);
                break;
            case 'A':
                para_A = atof(optarg);
                break;
            case 'p':
                para_pinc = atof(optarg);
                break;
            case 'c':
                para_min_mutation_para = atof(optarg);
                break;
            case 'l':
                population_size = atoi(optarg);
                break;
            case 'm':
                pop_mu = atoi(optarg);
                break;                
            case 'f':
                id_function = atoi(optarg);
                break;
            case 'e':
                max_evaluation = atoi(optarg);
                break;
        }
    }
    
    // Set a clock to record running time
    clock_t start_all, end_all;
    start_all = clock();
    srand(clock());

    // Build a array containing all possible mutation rates
    float mutation_rate = para_min_mutation_para / n;
    // Compute number of possible mutation rates
    for (i = 0; mutation_rate * para_A <= 0.5; i++)
    {
        mutation_rate = mutation_rate * para_A;
    }    
    num_mutation_rates = i;
    // Enter each mutation rate into array
    mutation_rates = new float[num_mutation_rates];
    mutation_rate = para_min_mutation_para / n;
    for (i = 0; i < num_mutation_rates; i++)
    {
        mutation_rates[i] = mutation_rate;
        mutation_rate = mutation_rate * para_A;
    }

    // Variables to control optimisation
    int num_generation = 0;
    int num_evaluation = 0;
    bool is_find_optimal = false;
    float best_fitness = 0;

    // Initialise population
    Population population = Population();

    // Initialise evaluation
    Evaluation evaluation = Evaluation(id_function);

    while(num_evaluation <= max_evaluation && is_find_optimal == false)
    {
        num_generation++;
        cout<<"Gen' "<< num_generation << " (Evaluation: " << num_evaluation << ")" <<" : ";

        // Evaluate population by given fitness function
        for (i = 0; i < population_size; i++)
        {
            population.individuals[i].fitness = evaluation.evaluation_solution(population.individuals[i].bitstring);
        }

        // Sort population by multi-objective sorting
        population.multiobj_sorting();

        best_fitness = population.individuals[0].fitness;
        cout << "Best Fitness: " << best_fitness << ", its mutation para': " << mutation_rates[population.individuals[0].index_mutation_rate] * n << flush << endl;

        // Check if optimal found
        if (best_fitness >= evaluation.max_fitness)
        {
            is_find_optimal = true;
            break;
        }

        // Select population by (mu, lambda) selection
        population.comma_selection();

        // Mutate selected population
        population.mutating_population();

        // Update current number of evalutions
        num_evaluation = num_generation * population_size;
    }

    // Show running time
    end_all = clock();
    cout << fixed << "Total Running Time: \t\t\t"<< (double) (end_all - start_all) / CLOCKS_PER_SEC << "s\n";
    cout << fixed << "Running Time Per Generation: \t"<< (double) (end_all - start_all) / CLOCKS_PER_SEC / num_generation << "s" << endl;
}
