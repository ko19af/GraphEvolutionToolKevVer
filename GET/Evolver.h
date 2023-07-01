#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "SDA.h"
#include "GET/EEOPS.h"
#include "Graph.h"

 // Kevin, replace the variables without declaration either with global definitions or instance variables in Evolver.


 int fits[POPULATION_SIZE];// array holding the fitness values of the population members
 SDA *SDAPop;// array containing the members of the population
 bool dead[POPULATION_SIZE];// array holding information on whether a member of the population is dead

using namespace std;

template<class representation>
class Evolver {
public:
    explicit Evolver();

private:
   
    float crossoverProb;// probability of a crossover happening
    float mutationProb;// probability of a mutation occuring
    pair<int, int> numMuts;

    vector<representation> population;
    vector<float> fitnessVals;
};

/**This method initializes the population
 * 
*/

vector<SDA> initPop(){
    // Generate the Initial Population
    for (int idx = 0; idx < POPULATION_SIZE; idx++) {// generate a population based on the populaiton size defined
         dead[idx] = false;// initialize ll members of the population to be alive
         SDAPop[idx].fillOutput(SDAOutput);// The "fillOutput" method in the SDA class needs to be created or changed to what it is now!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
         while (necroticFilter()) {
             SDAPop[idx].randomize();
             SDAPop[idx].fillOutput(SDAOutput);
         }
         fits[idx] = fitness(SDAPop[idx], false, false);// get the fitness of the initial population
    }
}

/** This method performs a fitness evaluation of an SDA by examining the ...!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * 
*/

double fitness(SDA &evaluate, bool eval1, bool custom){
   // if(evaluate.transitions.size() > best){// Create function in SDA class to return number of transitions ?????
        //return evaluate.transitions.size();
    //}else return 50;
    return 10;
}// fitness

vector<int> tournSelect(int size, bool decreasing) {
    vector<int> tournIdxs;// initialize vector for holding placement of population in tournament
    int idxToAdd;

    tournIdxs.reserve(size);// reserve space for tournament vector
    if (size == POPULATION_SIZE) {// if tournament size fed into method is same as initial popsize
        for (int idx = 0; idx < size; idx++) {//push each meber of population into tournament index vector
            tournIdxs.push_back(idx);
        }
    } else {// if the popsizes are different
        do {// do this while the tournament vector size is less than the tournament size
            idxToAdd = (int) lrand48() % POPULATION_SIZE;// randomly chooses a number within the population range
            if (count(tournIdxs.begin(), tournIdxs.end(), idxToAdd) == 0) {// check if the number is not already in tournament vector
                tournIdxs.push_back(idxToAdd);// push into tournament vector
            }
        } while (tournIdxs.size() < tournSize);
    }

    sort(tournIdxs.begin(), tournIdxs.end(), compareFitness);
    if (decreasing) {
        reverse(tournIdxs.begin(), tournIdxs.end());
    }
    return tournIdxs;
}

void matingEvent() {
    int numMuts;// number of mutation that will be perofrmed
    vector<int> tournIdxs;// vector holding the results from the tournament selection
    // Selection
    tournIdxs = tournSelect(TOURNAMENT_SIZE, ctrlFitnessFctn == 1);// perform tournament selection

    // Copy the Parents -> Children
    SDAPop[tournIdxs[0]].copy(SDAPop[tournIdxs[TOURNAMENT_SIZE - 2]]);
    SDAPop[tournIdxs[1]].copy(SDAPop[tournIdxs[TOURNAMENT_SIZE - 1]]);

    // perform two point crossover on the selected parents to produce the children
    if (drand48() < CROSSOVER_RATE) SDAPop[tournIdxs[0]].twoPointCrossover(SDAPop[tournIdxs[1]]);

    // Mutation
    if (drand48() < MUTATION_RATE) {// if mutation occurs, perform mutation on the children generated
        numMuts = (int) lrand48() % maxMuts + 1;
        SDAPop[tournIdxs[0]].mutate(numMuts);
        numMuts = (int) lrand48() % maxMuts + 1;
        SDAPop[tournIdxs[1]].mutate(numMuts);
    }

    // Reset dead SDAs
    SDAPop[tournIdxs[0]].fillOutput(SDAOutput);
    //dead[tournIdxs[0]] = necroticFilter();
    SDAPop[tournIdxs[1]].fillOutput(SDAOutput);
    //dead[tournIdxs[1]] = necroticFilter();

    // if (!dead[tournIdxs[0]]) {
    //     fits[tournIdxs[0]] = fitness(tournIdxs[0], false);
    // } else {
    //     fits[tournIdxs[0]] = globalWorstFit;
    // }

    // if (!dead[tournIdxs[1]]) {
    //     fits[tournIdxs[1]] = fitness(tournIdxs[1], false);
    // } else {
    //     fits[tournIdxs[1]] = globalWorstFit;
    // }
}

