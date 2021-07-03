//
// Created by abrolhus on 26/06/2021.
//

#include "Problem.h"

float Problem::getDistance(int from, int to) {
    // TODO(Abreu): assert that both "from" and "to" are valid node indexes;
    return this->distances[from][to]; // returns -1 if not connected; Actually, to solve this problem Im assuming all vertices are connected;
}
Problem::Problem(int vehicleCapacity, int nVehicles, int maxBss, int maxBattery, int nClients, int bssCost,
                 std::vector<float> nodeDemands, std::vector<std::vector<float>> distances) {
    this->vehicleCapacity = vehicleCapacity;
    this->nVehicles = nVehicles;
    this->maxBss = maxBss;
    this->vehicleMaxBattery = maxBattery;
    this->nClients = nClients;
    this->bssCost = bssCost;
    this->nodeDemands = nodeDemands;
    this->distances = distances; // O(n2)
}
//Problem::Problem(int vehicleCapacity, int nVehicles, int maxBss, int maxBattery, int nClients, int bssCost){
    //this->vehicleCapacity = vehicleCapacity;
    //this->nVehicles = nVehicles;
    //this->maxBss = maxBss;
    //this->vehicleMaxBattery = maxBattery;
    //this->nClients = nClients;
    //this->bssCost = bssCost;
//}
