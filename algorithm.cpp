#include "algorithm.h"
#include <set>
#include <iostream>
#define MAXITER 1e4
#define DEPOT 0

enum nodeLabel { Client, Bss, ToBss};
bool isValidSolution(Solution solution, Problem problem){
    return false;
}
class Path{
public:
    Path(int vehicle, int destination, int label, float cost, float demand, float batteryCost) :
    vehicle(vehicle), destination(destination), label(label), cost(cost), batteryCost(batteryCost), demand(demand){ };
    int vehicle;
    int destination;
    int label;
    float cost;
    float demand;
    float batteryCost;

    bool operator< (const Path& that) const
    {
        if(this->cost == that.cost)
            return (this->vehicle > that.vehicle);
        return (this->cost < that.cost);
    }
};

class comparator_class {
public:
    // Comparator function
    bool operator()(Path v1, Path v2)
    {
        return (v1.cost < v2.cost);
    }
};

Solution* greedyAlg(Problem* Prob){
    Solution* Sol = new Solution(Prob->getNVehicles());
    //std::set<std::vector<float>, comparator_class> pathList;
    std::set<Path> pathList;
    std::set<int> possibleNextNodes;
    std::set<int> unvisitedClients;
    float constructionCosts = 0;
    float totalCost = 0;
    std::vector<float> remainingVehicleCapacities(Prob->getNVehicles(), Prob->getVehicleCapacity());
    std::vector<float> remainingVehicleBattery(Prob->getNVehicles(), Prob->getVehicleMaxBattery());
    std::vector<bool> wasInstalledBss((Prob->getNNodes() - Prob->getNClients()), false); // if the bss was built
    //std::vector<std::vector<int>> routes(Prob->getNVehicles());
    std::vector<std::vector<bool>> wasVisitedByK(Prob->getNVehicles(), {true});
    for(auto& rout : Sol->routes){
        rout.push_back(DEPOT);
    } // every rout starts on depot (0)

    for(int i = 1; i < Prob->getNNodes(); i++){
        possibleNextNodes.insert(i); // #1 to #Size-1; #0 -> depot
        if(i <= Prob->getNClients()) {
            unvisitedClients.insert(i);
        }
        for(auto& vehicle : wasVisitedByK){
            vehicle.push_back(false);
        }
    }
    bool stop = false;
    while(!stop) {
        pathList.clear();
        for (int k = 0; k < Prob->getNVehicles(); k++) {
            int currentNode = Sol->routes[k].back();
            int label;
            for (auto &nodeIndex : possibleNextNodes) {
                float cost = 0;
                float demand = 0;
                float batteryCost = 0;
                if(wasVisitedByK[k][nodeIndex]){ continue; }
                if (Prob->isPossibleBssLocation(nodeIndex)) {
                    if (wasInstalledBss[nodeIndex - Prob->getFirstBssIndex()]) { // if is BSS
                        if (Prob->getDistance(currentNode, nodeIndex) > remainingVehicleBattery[k]) { continue; }
                        cost = Prob->getDistance(currentNode, nodeIndex);
                        label = nodeLabel::Bss;
                    } else { // TODO(abreu): check if youre adding more BSS than you can; (if you have a limit)
                        if (Prob->getDistance(currentNode, nodeIndex) > remainingVehicleBattery[k]) { continue; }
                        cost = Prob->getDistance(currentNode, nodeIndex) + Prob->getBssCost();
                        label = nodeLabel::ToBss;
                    }
                } else { // if is CLIENT
                    bool hasBssNearby = false;
                    for(int i = Prob->getFirstBssIndex(); i < Prob->getNNodes(); i++){
                        if(Prob->getDistance(currentNode, nodeIndex) + Prob->getDistance(nodeIndex, i) <= remainingVehicleBattery[k]){
                            hasBssNearby = true;
                            break;
                        }
                    }
                    if ((!hasBssNearby && Prob->getDistance(currentNode, nodeIndex) + Prob->getDistance(nodeIndex, DEPOT) >
                        remainingVehicleBattery[k]) || Prob->getDemand(nodeIndex) > remainingVehicleCapacities[k]) { continue; }
                    cost = Prob->getDistance(currentNode, nodeIndex);
                    label = nodeLabel::Client;
                    batteryCost += Prob->getDistance((currentNode), nodeIndex); // TODO(abreu): Redundante a linha 124;
                    demand = Prob->getDemand(nodeIndex);
                }
                auto path = new Path(k, nodeIndex, label, cost, demand, batteryCost);
                int flag = 0;
                if(path->cost < 90) {
                    flag = 1;
                }
                pathList.insert(*path);
            }
        }
        int teste = pathList.size();
        if(pathList.size() == 0) // if no viable next path, return
            return Sol;
        // ---------- SELECTION ---------- //
        auto chosenPath = *(pathList.begin()); // get best (min local cost) path;

        // ---------- UPDATE ---------- //
        wasVisitedByK[chosenPath.vehicle][chosenPath.destination] = true;
        if (chosenPath.label == nodeLabel::Client) { // reduce battery, capacity. Add node to vehicle route and remove the client from the unvisited list;
            unvisitedClients.erase(chosenPath.destination);
            possibleNextNodes.erase((chosenPath.destination));
            remainingVehicleBattery[chosenPath.vehicle] -= chosenPath.batteryCost;
            Sol->routes[chosenPath.vehicle].push_back(chosenPath.destination);
            remainingVehicleCapacities[chosenPath.vehicle] -= chosenPath.demand;
        } else if (chosenPath.label == nodeLabel::Bss) {
            remainingVehicleBattery[chosenPath.vehicle] = Prob->getVehicleMaxBattery();
            Sol->routes[chosenPath.vehicle].push_back(chosenPath.destination);
        } else if (chosenPath.label == nodeLabel::ToBss) {
            remainingVehicleBattery[chosenPath.vehicle] = Prob->getVehicleMaxBattery();
            Sol->routes[chosenPath.vehicle].push_back(chosenPath.destination);
            wasInstalledBss[chosenPath.destination - Prob->getFirstBssIndex()] = true;
            constructionCosts += Prob->getBssCost();
            // TODO(abreu): add shipping cost to Cost.
        } else {
            std::cout << "error";
            return Sol;
        }
        Sol->cost += chosenPath.cost; // update solution cost
        if(unvisitedClients.size() == 0) {
            for(auto& route : Sol->routes){
                if(route.back() != 0) {
                    Sol->cost += Prob->getDistance(route.back(), DEPOT);
                    route.push_back(0);
                }
            }
            for(int i = 0; i < Prob->getNNodes() - Prob->getNClients(); i++){
                if(wasInstalledBss[i]){
                    Sol->bssLocations.insert(i + Prob->getFirstBssIndex());
                }
            }
            return Sol;
        }
    }
    return Sol;
}
bool isFeasibleSolution(Solution* Sol, Problem* Prob){
    std::vector<float> vehicleCapacities(Prob->getNVehicles(), Prob->getVehicleCapacity());
    std::vector<float> vehicleBatteries(Prob->getNVehicles(), Prob->getVehicleMaxBattery());
    for(int k = 0; k < Sol->routes.size(); k++){
        int lastNode = 0;
        for(auto node : Sol->routes[k]){
            vehicleCapacities[k] -= Prob->getDemand(node);
            vehicleBatteries[k] -= Prob->getDistance(lastNode, node); // will be zero for (0,0)
            if(vehicleBatteries[k] < 0 || vehicleCapacities[k] < 0){ return false; }
            if(Sol->bssLocations.find(node) != Sol->bssLocations.end()){
                vehicleBatteries[k] = Prob->getVehicleMaxBattery();
            }
            lastNode = node;
        }
    }
    return true;
}
