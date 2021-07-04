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
    Path(int vehicle, int destination, int label, float cost, float demand, float batteryCost, bool needsRecharge) :
    vehicle(vehicle), destination(destination), label(label), cost(cost), batteryCost(batteryCost), demand(demand), needsRecharge(needsRecharge){ };
    int vehicle;
    int destination;
    int label;
    float cost;
    float demand;
    float batteryCost;
    bool needsRecharge;

    bool operator< (const Path& that) const
    {
        if(this->cost == that.cost)
            return (this->vehicle < that.vehicle);
        return (this->cost < that.cost);
    }
};

Solution* greedyAlg(Problem* Prob){
    int getNearestBss(int currentNode, Problem* Prob, std::vector<bool>& wasVisitedByVehicle);
    bool canKVisitAnotherClient(Problem* Prob, std::set<int>& unvisitedClients, float vehicleCapacity);
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
            if(!canKVisitAnotherClient(Prob, unvisitedClients, remainingVehicleCapacities[k])){
                // continue;
            }
            for (auto &nodeIndex : possibleNextNodes) {
                float cost = 0;
                float demand = 0;
                float batteryCost = 0;
                bool needsRecharge = false;
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
                        if(wasVisitedByK[k][i]){ continue; }
                        if(Prob->getDistance(currentNode, nodeIndex) + Prob->getDistance(nodeIndex, i) <= remainingVehicleBattery[k]){
                            hasBssNearby = true;
                            break;
                        }
                    }
                    if(Prob->getDistance(currentNode, nodeIndex) + Prob->getDistance(nodeIndex, DEPOT) > remainingVehicleBattery[k]){
                        needsRecharge = true;
                    } // need recharge means that the vehicle cant go to the node and return to the depot without recharging in between
                    if ((!hasBssNearby && needsRecharge) || Prob->getDemand(nodeIndex) > remainingVehicleCapacities[k]) { continue; }
                    cost = Prob->getDistance(currentNode, nodeIndex);
                    label = nodeLabel::Client;
                    batteryCost += Prob->getDistance((currentNode), nodeIndex); // TODO(abreu): Redundante a linha 124;
                    demand = Prob->getDemand(nodeIndex);
                }
                auto path = new Path(k, nodeIndex, label, cost, demand, batteryCost, needsRecharge);
                pathList.insert(*path);
            }
        }
        if(pathList.size() == 0) { // if no viable next path, return
            for (int i = 0; i < Prob->getNNodes() - Prob->getNClients(); i++) {
                if (wasInstalledBss[i]) {
                    Sol->bssLocations.insert(i + Prob->getFirstBssIndex());
                }
            }
            return Sol;
        }
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
            for(int k = 0; k < Sol->routes.size(); k++){
                auto& route = Sol->routes[k];
                if(route.back() != 0) {
                    if(route.back() < Prob->getFirstBssIndex()) {
                        int nearBss = getNearestBss(route.back(), Prob, wasVisitedByK[k]);
                        Sol->cost += Prob->getDistance(route.back(), nearBss) + Prob->getDistance(nearBss, DEPOT);
                        route.push_back(nearBss);
                        route.push_back(0);
                        wasInstalledBss[nearBss - Prob->getFirstBssIndex()] = true;
                    }
                    else{
                        Sol->cost += Prob->getDistance(route.back(), DEPOT);
                        route.push_back(0);

                    }
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
bool isFeasibleSolution(Solution* Sol, Problem* Prob, float& cost){
    std::vector<float> vehicleCapacities(Prob->getNVehicles(), Prob->getVehicleCapacity());
    std::vector<float> vehicleBatteries(Prob->getNVehicles(), Prob->getVehicleMaxBattery());
    std::vector<bool> clientVisited(Prob->getNClients(), false);
    cost = 0;
    for(int k = 0; k < Sol->routes.size(); k++){
        int lastNode = 0;
        for(auto node : Sol->routes[k]){
            if(node < Prob->getFirstBssIndex()){ clientVisited[node] = true; }
            vehicleCapacities[k] -= Prob->getDemand(node);
            vehicleBatteries[k] -= Prob->getDistance(lastNode, node); // will be zero for (0,0)
            cost += Prob->getDistance(lastNode, node);
            if(vehicleBatteries[k] < 0 || vehicleCapacities[k] < 0){ return false; }
            if(Sol->bssLocations.find(node) != Sol->bssLocations.end()){
                vehicleBatteries[k] = Prob->getVehicleMaxBattery();
            }
            lastNode = node;
        }
    }
    for(auto node : Sol->bssLocations){
        cost += Prob->getBssCost();
    }
    for(bool visited : clientVisited){
        if(!visited) {
            return false;
        }
    }
    return true;
}

int getNearestBss(int currentNode, Problem* Prob, std::vector<bool>& wasVisitedByVehicle){
    float minDistance = 1e6;
    int bestBss = -1;
    for(int i = Prob->getFirstBssIndex(); i < Prob->getNNodes(); i++){
        float dist = Prob->getDistance(currentNode, i);
        if(dist < minDistance){
            minDistance = dist;
            bestBss = i;
        }
    }
    return bestBss;
}

bool canKVisitAnotherClient(Problem* Prob, std::set<int>& unvisitedClients, float vehicleCapacity){
    for(auto& client : unvisitedClients){
        if(Prob->getDemand(client) <= vehicleCapacity){
            return true;
        }
    }
    return false;
}
void localSearch(Solution* NewSol, Solution* Sol, Problem* Prob){
    void opt2(std::vector<int>& route, int a, int b);
    // 2 opt
    std::vector<float> bestRouteCosts(Prob->getNVehicles(), 1e7);
    Solution* BestSol = new Solution(Prob->getNVehicles());
    for(int k = 0; k < Sol->routes.size(); k++){
        NewSol->routes[k].clear();
        BestSol->routes[k].clear();
        for(auto const& node : Sol->routes[k]){
            NewSol->routes[k].push_back(node);
            BestSol->routes[k].push_back(node);
        }
    }
    NewSol->bssLocations = Sol->bssLocations;
    BestSol->bssLocations = Sol->bssLocations;
    NewSol->cost = Sol->cost;
    BestSol->cost = Sol->cost;



    for(int k = 0; k < Sol->routes.size(); k++){
        std::vector<int> route(Sol->routes[k]); // N
        float cost;
        float bestCost = 1e7;

        if(route.size() < 5){ continue; }
        for(int i = 1; i < route.size()-1; i++){
            for(int j = 2; j < route.size()-1; j++){
                opt2(route, i, j);
                // NewSol->routes[k] = route;
                //for(int l = 0; l < route.size(); l++){
                    //NewSol->routes[k][l] = route[l];
                //}
                bool isFeasible = isFeasibleRoute(route, Prob, Sol, cost);
                std::cout << "wait";
                if(isFeasible){
                    if(cost < bestRouteCosts[k]){
                        bestRouteCosts[k] = cost;

                    }
                }
            }
        }


    }
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

void swap(std::vector<int>& v, int i, int j){
    int aux;
    aux = v[i];
    v[i] = v[j];
    v[j] = aux;
}
bool isFeasibleRoute(std::vector<int>& route, Problem* Prob, Solution* Sol, float& cost) {
    float vehicleCapacity = Prob->getVehicleCapacity();
    float vehicleBattery = Prob->getVehicleMaxBattery();
    cost = 0;
    int lastNode = DEPOT;
    for (auto node : route) {
        vehicleCapacity -= Prob->getDemand(node);
        vehicleBattery -= Prob->getDistance(lastNode, node); // will be zero for (0,0)
        cost += Prob->getDistance(lastNode, node);
        if (vehicleCapacity < 0 || vehicleBattery < 0) { return false; }
        if (Sol->bssLocations.find(node) != Sol->bssLocations.end()) {
            vehicleBattery = Prob->getVehicleMaxBattery();
        }
        lastNode = node;
    }
    return true;
}
