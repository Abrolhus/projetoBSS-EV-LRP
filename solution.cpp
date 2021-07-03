//
// Created by abrolhus on 26/06/2021.
//

#include "Solution.h"


Solution::Solution(int nVehicles){
    this->cost = 0;
    this->routes = std::vector<std::vector<int>>(nVehicles);
    this->bssLocations = std::vector<int>();
}