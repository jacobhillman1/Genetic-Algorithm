#include "individual.h"

Individual::Individual() {
    fitness = 0.0;
    calculatedValue = 0.0;
}

Individual::Individual(std::string chromo) {
    this->chromo = chromo;
    fitness = 0.0;
    calculatedValue = 0.0;
}

void Individual::setFitness(float fitness) {
    this->fitness = fitness;
}

std::string Individual::getChromo() {
    return chromo;
}

float Individual::getFitness() {
    return fitness;
}


void Individual::printChromo() {
    std::cout << "Chromo inside object: " << chromo << std::endl;
}

/*
 * Function to be used with internal calculation of fitness
 */
void Individual::calculateFitness(float idealFit) {
    if (idealFit - calculatedValue == 0) {
        fitness = 1.0;
    }
    else {
        fitness = 1 / fabs(idealFit - calculatedValue);
    }
}
