//
// Created by abrolhus on 26/06/2021.
//


#ifndef BSS_SOLUTION_H
#define BSS_SOLUTION_H
#include <vector>
#include <set>


class Solution {
public:
    Solution(int);
    std::vector<std::vector<int>> routes;
    std::set<int> bssLocations;
    float cost;
    Solution& operator=(Solution const& that){
        this->routes = std::vector<std::vector<int>>(that.routes.size());
        for(int k = 0; k < that.routes.size(); k++){
            this->routes[k] = that.routes[k];
        }
        this->bssLocations = that.bssLocations;
        this->cost = that.cost;
        return *this;
    }
};


#endif //BSS_SOLUTION_H
