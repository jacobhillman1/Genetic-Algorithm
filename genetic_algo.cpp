#include <iostream>
#include "individual.h"
#include <unordered_map>

#define NUM_GENERATIONS 100
#define POPULATION_SIZE 100
#define GENE_LEN 4
#define CHROMO_LEN 28 // Must be a multiple of GENE_LEN
#define IDEAL_FIT 10

// Function Prototypes
void initializeValuesForMath(std::unordered_map<std::string, char>&);
void initializePopulation(Individual[]);
void assignFitness(std::unordered_map<std::string, char>, Individual[]);
int checkExpressionForm(char[]);
Individual rouletteSelection(Individual[]);
Individual crossOver(Individual, Individual);
Individual* createNewPopulation(Individual[]);
float getMeanFitness(Individual*);
int evaluateExpression(std::string);

int main(int argc, char* const argv[]) {
    std::unordered_map<std::string, char> geneValues;
    Individual* population = new Individual[POPULATION_SIZE]();

    initializeValuesForMath(geneValues);
    initializePopulation(population);

    for(int i = 0; i < NUM_GENERATIONS; i++) {
        assignFitness(geneValues, population);
        Individual* newPopulation = createNewPopulation(population);
        delete[] population;
        population = newPopulation;
        std::cout << "Mean fitness for generation" << i << " is " << getMeanFitness(population) << std::endl;
    }
}

/*
 * Create an unordered map of key-value pairs, containing the byte patterns
 * and their respective values.
 */
void initializeValuesForMath(std::unordered_map<std::string, char>& geneValues) {
    geneValues = {
        {"0000", '0'},
        {"0001", '1'},
        {"0010", '2'},
        {"0011", '3'},
        {"0100", '4'},
        {"0101", '5'},
        {"0110", '6'},
        {"0111", '7'},
        {"1000", '8'},
        {"1001", '9'},
        {"1010", '+'},
        {"1011", '-'},
        {"1100", '*'},
        {"1101", '-'}
    };
}

/*
 * Randomly create individuals for the starting population
 */
void initializePopulation(Individual population[]) {
    std::string chromo;
    std::string gene;
    std::srand((unsigned)std::time(0));
    bool legit = false; //set true of the byte pattern exists with a corresponding value
    int byte;

    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int n = 0; n < CHROMO_LEN / GENE_LEN; n++) { // Generate a unique chromo
            while(!legit) {
                for (int j = 0; j < GENE_LEN; j++) { // Generate each gene one byte at a time
                    byte = std::rand() % 2 + 0;
                    gene += std::to_string(byte);
                }
                if(gene == "1110" || gene == "1111") { // these genes don't exist
                    gene.clear();
                    legit = false;
                }
                else {
                    chromo += gene;
                    gene.clear();
                    legit = true;
                }
            }
            legit = false;
        }
        population[i] = Individual(chromo); // Create an individual with this chromo
        chromo.clear();
    }
}

/*
 * Calculate the fitness of each individual in the population, based on both
 * the form of the expression and the value it evaluates to
 */
void assignFitness(std::unordered_map<std::string, char> geneValues,
                                Individual population[]) {
    std::string tempGene;
    char value;
    char expression[CHROMO_LEN / GENE_LEN + 1]; // +1 for null terminator

    for (int k = 0; k < POPULATION_SIZE; k++) {
        // Convert the genes for each individual into their respectice values
        for (int i = 0; i < CHROMO_LEN / GENE_LEN; i++) {
            for (int n = 0; n < GENE_LEN; n++) {
                tempGene += population[k].getChromo()[GENE_LEN * i + n];
            }
            value = geneValues[tempGene];
            tempGene.clear();
            expression[i] = value;
        }

        expression[CHROMO_LEN / GENE_LEN] = '\0';

        // determine if the expression is in the correct form
        int expressionFromFitness = checkExpressionForm(expression);

        if(expressionFromFitness == 0) {
            population[k].setFitness(0.0);
        }
        else if (expressionFromFitness < CHROMO_LEN / GENE_LEN){
            population[k].setFitness(.1 * float(expressionFromFitness));
        }
        else { // if the expression has the correct form
            int expressionValue = evaluateExpression(expression);
            if(expressionValue - IDEAL_FIT == 0) {
                population[k].setFitness(5.0);
            }
            else {
                population[k].setFitness( 5 * (1 / std::abs((expressionValue - IDEAL_FIT))) );
            }
        }
    }
}

/*
 * Check to see if an expression follows the form int - operator - int - operator - etc.
 * Returns the number of genes in the chromosome that match this pattern, starting
 * from the beginning.
 */
int checkExpressionForm(char expression[]) {
    // Start out by checking whether the first value in the expression is
    // a digit or operator.
    if (!isdigit(expression[0])) { // If it starts with an operator, it is incorrect form
        return 0; // fitness = 0.01
    }
    else {
        bool lookingForNumber = false;
        for(int j = 1; j < CHROMO_LEN / GENE_LEN; j++) {
            if (isdigit(expression[j])) { //if it's a digit
                if(!lookingForNumber) {
                    return j;
                }
                else {
                    lookingForNumber = false;
                }
            }
            else //if it's an operator
            {
                if(lookingForNumber) {
                    return j;
                }
                else {
                    lookingForNumber = true;
                }
            }
        }
        return (int)CHROMO_LEN / GENE_LEN;
    }
}

/*
 * Calculates and returns the result of the expression. Dependent on the
 * expression in the form value - operator - value - operator - etc.
 */
int evaluateExpression(std::string expression) {
    int value = expression[0] - '0';
    for (int i = 1; i < CHROMO_LEN / GENE_LEN; i++) {
        if (expression[i] == '+') {
            value += expression[++i] - '0';
        }
        else if (expression[i] == '-') {
            value -= expression[++i] - '0';
        }
        else if (expression[i] == '*') {
            value *= expression[++i] - '0';
        }
        else if (expression[i] == '/') {
            value /= expression[++i] - '0';
        }
    }
    return value;
}

float getMeanFitness(Individual* population) {
    float totalFitness = 0.0;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        totalFitness += population[i].getFitness();
    }
    return totalFitness / POPULATION_SIZE;
}

Individual* createNewPopulation(Individual population[]) {
    int n = 0;
    Individual* newPopulation = new Individual[POPULATION_SIZE]();
    for(int i = 0; i < POPULATION_SIZE / 4; i++) {
        Individual parent1 = rouletteSelection(population);
        Individual parent2 = rouletteSelection(population);

        // Ensure two individuals cannot crossOver with themselves
        //TODO: this might be wrong
        while(parent1.getChromo() == parent2.getChromo()) {
            parent2 = rouletteSelection(population);
        }

        for(int j = 0; j < 2; j++) {
            newPopulation[n++] = crossOver(parent1, parent2);
        }

        newPopulation[n++] = parent1;
        newPopulation[n++] = parent2;
        //TODO: run mutate on all four
    }
    return newPopulation;
}

/*
 * Returns an individual from the total population
 * The probability an individual will be chosen is based on the individual's
 * fitness in comparison to the total fitness score of the population
 */
Individual rouletteSelection(Individual population[]) {
    float totalFitness = 0.0;
    float randomNum = std::rand() % 1000 + 1;
    float ratio = randomNum / 1000;

    for (int i = 0; i < POPULATION_SIZE; i++) {
        totalFitness += population[i].getFitness();
    }

    float value = ratio * totalFitness;

    for (int i = 0; i < POPULATION_SIZE; i++) {
        value -= population[i].getFitness();
        if(value <= 0) {
            return population[i];
        }
    }
    return population[POPULATION_SIZE - 1];
}

/*
 * Crossover at the gene level, to be sure that the resulting
 * chromosome doesn't contain any illegitimate genes.
 * In different applications, where each byte represents a possible value,
 * it is better to choose a random location and swap all points after that value
 */
Individual crossOver(Individual p1, Individual p2) {
    std::string newChromo;
    std::string p1chromo = p1.getChromo();
    std::string p2chromo = p2.getChromo();
    for(int i = 0; i < CHROMO_LEN / GENE_LEN; i++) {
        int choice = std::rand() % 100;
        if (choice < 50) {
            newChromo += p1chromo.substr(i * GENE_LEN, GENE_LEN);
        }
        else {
            newChromo += p2chromo.substr(i * GENE_LEN, GENE_LEN);
        }
    }
    return Individual(newChromo);
};
