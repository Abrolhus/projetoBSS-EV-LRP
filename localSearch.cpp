//
// Created by abrolhus on 04/07/2021.
//

#include "localSearch.h"
#include "algorithm.h"
void localSearch(Solution* BestSol, Solution* Sol, Problem* Prob){
    void opt2(std::vector<int>& route, int a, int b);
    // 2 opt
    std::vector<float> bestRouteCosts(Prob->getNVehicles(), 1e7);
    for(int k = 0; k < bestRouteCosts.size(); k++){
        float cost;
        isFeasibleRoute(Sol->routes[k], Prob, Sol, cost);
        bestRouteCosts[k] =  cost;
    }
    Solution* NewSol = new Solution(Prob->getNVehicles());
    *NewSol = *Sol;
    *BestSol = *Sol;
    // for(int k = 0; k < Sol->routes.size(); k++){
    // NewSol->routes[k].clear();
    // BestSol->routes[k].clear();
    // for(auto const& node : Sol->routes[k]){
    // NewSol->routes[k].push_back(node);
    // BestSol->routes[k].push_back(node);
    // }
    // }
    // NewSol->bssLocations = Sol->bssLocations;
    // BestSol->bssLocations = Sol->bssLocations;
    // NewSol->cost = Sol->cost;
    // BestSol->cost = Sol->cost;
    bool improving = true;
    while(improving) {
        improving = false;
        for (int k = 0; k < Sol->routes.size(); k++) {
            std::vector<int> route(BestSol->routes[k]); // N
            float cost;
            float bestCost = 1e7;

            if (route.size() < 5) { continue; }
            for (int i = 1; i < route.size() - 1; i++) {
                for (int j = 2; j < route.size() - 1; j++) {
                    opt2(route, i, j);
                    // NewSol->routes[k] = route;
                    //for(int l = 0; l < route.size(); l++){
                    //NewSol->routes[k][l] = route[l];
                    //}
                    bool isFeasible = isFeasibleRoute(route, Prob, Sol, cost);
                    if (isFeasible) {
                        if (cost < bestRouteCosts[k]) {
                            bestRouteCosts[k] = cost;
                            BestSol->routes[k] = route;
                            improving = true;
                        }
                    }
                    opt2(route, i, j); // revert 2opt
                }
            }
        }
    }
    return;
}
void opt2(std::vector<int>& route, int a, int b){
    void swap(std::vector<int>& v, int i, int j);
    if(route.size() < 4){ return; }
    for(int i = 1; i < route.size(); i++){
        if(i >=a && i <= a + (b - a)/2){
            swap(route, i, a + b - i);
        }
    }
}

void skipBss(std::vector<int>& route, int bss, Problem* Prob){
    void swap(std::vector<int>& v, int i, int j);
    if(route.size() < 3){ return; }
    int lastNode = 0;
    for(int i = 1; i < route.size()-1; i++){
        if(i == bss){
        }

    }


}

void swap(std::vector<int>& v, int i, int j){
    int aux;
    aux = v[i];
    v[i] = v[j];
    v[j] = aux;
}

