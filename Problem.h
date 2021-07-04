//
// Created by abrolhus on 26/06/2021.
//

#ifndef BSS_PROBLEM_H
#define BSS_PROBLEM_H
#include <vector>

// BSS = Battery Switch Station
class Problem {
public:
    Problem(int vehicleCapacity, int nVehicles, int maxBss, int maxBattery, int nClients, int bssCost, std::vector<float> nodeDemands, std::vector<std::vector<float>> distances, std::vector<std::pair<float, float>> coords);
    int getNVehicles(){ return this->nVehicles; }
    int getVehicleCapacity(){ return this->vehicleCapacity; }
    int getMaxBss(){ return this->maxBss; }
    int getNClients(){ return this->nClients; }
    int getVehicleMaxBattery(){ return this->vehicleMaxBattery; }
    int getNNodes(){ return this->distances.size(); }
    float getBssCost(){ return (float)this->bssCost; }
    float getDistance(int from, int to);
    std::pair<float, float> getCoords(int i){ return this->coords[i]; }
    float getDemand(int i){
        if(i >= this->getFirstBssIndex())
            return 0;
        return this->nodeDemands[i];
    }
    int getFirstBssIndex(){ return nClients + 1; }
    bool isPossibleBssLocation(int i){ return i >= this->getFirstBssIndex();}
private:
    int vehicleCapacity;
    int nVehicles;
    int maxBss;
    int vehicleMaxBattery;
    int nClients;
    int bssCost;

    std::vector<float> nodeDemands; // index = node index;
    std::vector<std::vector<float>> distances; // "-1"->no edge;
    std::vector<std::pair<float, float>> coords;
};
#endif //BSS_PROBLEM_H
