#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <iostream>
#include <cmath>

class Individual {
private:
    float fitness;
    float calculatedValue;
    std::string chromo;

public:
    Individual();
    Individual(std::string chromo);
    void setFitness(float fitness);
    std::string getChromo();
    float getFitness();
    void printChromo();
    void calculateFitness(float idealFit);
};

#endif
