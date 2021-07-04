//
// Created by abrolhus on 23/06/2021.
//
#include <iostream>
#include <sstream>
#include <fstream>
#include "utils.h"
void solutionToCsv(Solution* Sol, Problem* Prob){
    for(int k = 0; k < Sol->routes.size(); k++){
        std::ofstream file("route" + std::to_string(k) + ".csv");
        file << "x,y,no,estacao" << std::endl;
        for(int node : Sol->routes[k]){
            auto coords = Prob->getCoords(node);
            int isBss = 0;
            if(Sol->bssLocations.find(node) != Sol->bssLocations.end()){
                isBss = 1;
            }
            file << coords.first << "," << coords.second << "," << node << "," << isBss << std::endl;
        }
        file.close();
    }
}
