//
// Created by abrolhus on 26/06/2021.
//


#ifndef BSS_SOLUTION_H
#define BSS_SOLUTION_H
#include <vector>


class Solution {
public:
    Solution(int);
    std::vector<std::vector<int>> routes;
    std::vector<int> bssLocations;
    float cost;
};


#endif //BSS_SOLUTION_H
