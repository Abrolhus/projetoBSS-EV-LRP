//
// Created by abrolhus on 27/06/2021.
//

#ifndef BSS_ALGORITHM_H
#define BSS_ALGORITHM_H
#include "Solution.h"
#include "Problem.h"

Solution* greedyAlg(Problem*);
bool isFeasibleSolution(Solution* Sol, Problem* Prob, float& cost);
void localSearch(Solution* returnSolution, Solution* Sol, Problem* Prob);
bool isFeasibleRoute(std::vector<int>& route, Problem* Prob, Solution* Sol, float& cost);

#endif //BSS_ALGORITHM_H
