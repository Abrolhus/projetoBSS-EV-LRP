//
// Created by abrolhus on 04/07/2021.
//

#ifndef BSS_LOCALSEARCH_H
#define BSS_LOCALSEARCH_H
#include <vector>
#include "algorithm.h"


void swap(std::vector<int>& v, int i, int j);
void opt2(std::vector<int>& route, int a, int b);
void skipBss(std::vector<int>& route, int bss, Problem*);
void localSearch(Solution* BestSol, Solution* Sol, Problem*);

#endif //BSS_LOCALSEARCH_H
