#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include "Graph/Graph.h"
#include "Graph/Node.h"
#include "Problem.h"
#include "algorithm.h"
#include "utils.h"
#include <vector>
#include <sstream>

using namespace std;

float distance(float x1, float y1, float x2, float y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}


int main(int argc, char const *argv[]) {
    Problem* leitura(ifstream& input_file);
    // Graph* leituraParaGraph(ifstream& input_file);
    ifstream input_file;
    input_file.open(argv[1], ios::in);
    Problem* problem = leitura(input_file);
    // greedyAlgorithm(problem);
    Solution* Sol = greedyAlg(problem);
    solutionToCsv(Sol, problem);
    float solCost;
    Solution* LocalSearchSolution = new Solution(problem->getNVehicles());
    bool isFeasible = isFeasibleSolution(Sol, problem, solCost);
    localSearch(LocalSearchSolution, Sol, problem);
    cout << "...." << endl;
    return 0;
}

Problem* leitura(ifstream& input_file){

    Problem* problem;
    string line;
    string value;

    int nodeCount, vehicleCapacity, nodeBase, batteryCapacity, bssCost;
    int nVehicles = 2; // TODO: get number of vehicles from filename;
    int bssCount;
    std::vector<std::pair<float, float>> nodeCoords;
    std::vector<std::pair<float, float>> bssCoords;
    std::vector<std::pair<float, float>> allCoords; // 0 -> deposit; 1 to N -> clients; N+1 to N+B -> bss`s
    std::vector<float> nodeDemands;

    // auxiliar variables:
    stringstream ss;
    int aux;

    //allCoords.push_back(std::pair<float, float>(1.f, -1.f));
    while(getline(input_file,line) && line != "END_FILE\r"){
        if(line == "NODE_COUNT_SECTION\r") {
            cout << endl;
            while (getline(input_file, value) && value != "END_SECTION\r"){
                if ( !value.empty() && value[value.size()-1] == '\r' ) {
                    value = value.substr(0, value.size() - 1);
                }
                cout << value << endl;
                nodeCount = std::stoi(value);
                //cout << nodeCount << "node coutn << " << endl;
            }
        }
        else if(line == "VEHICLE_CAP_SECTION\r") {
            cout << endl;
            while (getline(input_file, value) && value != "END_SECTION\r"){
                if ( !value.empty() && value[value.size()-1] == '\r' ) {
                    value = value.substr(0, value.size() - 1);
                }
                vehicleCapacity = std::stoi(value);
                cout << value << endl;
            }
        }
        else if(line == "NODE_COORD_SECTION\r") {
            cout << endl;
            std::pair<float, float> coord;
            int nodeIndex;
            while (getline(input_file, value) && value != "END_SECTION\r"){
                if ( !value.empty() && value[value.size()-1] == '\r' ) {
                    value = value.substr(0, value.size() - 1);
                }
                ss.clear();
                ss << value;
                ss >> nodeIndex;
                ss >> aux;
                coord.first = aux;
                ss >> aux;
                coord.second = aux;
                cout << value << endl;
                nodeCoords.push_back(coord);
                allCoords.push_back(coord);
            }
        }
        else if(line == "NODE_DEMAND_SECTION\r") {
            cout << endl;
            int nodeIndex;
            while (getline(input_file, value) && value != "END_SECTION\r"){
                if ( !value.empty() && value[value.size()-1] == '\r' ) {
                    value = value.substr(0, value.size() - 1);
                }
                ss.clear();
                ss << value;
                ss >> nodeIndex;
                ss >> aux;
                nodeDemands.push_back(aux);
                cout << value << endl;
            }
        }
        else if(line == "NODE_BASE_SECTION\r") {
            cout << endl;
            while (getline(input_file, value) && value != "END_SECTION\r"){
                if ( !value.empty() && value[value.size()-1] == '\r' ) {
                    value = value.substr(0, value.size() - 1);
                }
                ss.clear();
                ss << value;
                ss >> nodeBase;
                cout << value << endl;
            }
        }
        else if(line == "BATTERY_CAPACITY_SECTION\r") {
            cout << endl;
            while (getline(input_file, value) && value != "END_SECTION\r"){
                if ( !value.empty() && value[value.size()-1] == '\r' ) {
                    value = value.substr(0, value.size() - 1);
                }
                ss.clear();
                ss << value;
                ss >> batteryCapacity;
                cout << value << endl;
            }
        }
        else if(line == "CONSTRUCTION_COST_BSS_SECTION\r") {
            cout << endl;
            while (getline(input_file, value) && value != "END_SECTION\r"){
                if ( !value.empty() && value[value.size()-1] == '\r' ) {
                    value = value.substr(0, value.size() - 1);
                }
                ss.clear();
                ss << value;
                ss >> bssCost;
                cout << value << endl;
            }
        }
        else if(line == "CANDIDATE_BSS_COUNT_SECTION\r") {
            cout << endl;
            while (getline(input_file, value) && value != "END_SECTION\r"){
                if ( !value.empty() && value[value.size()-1] == '\r' ) {
                    value = value.substr(0, value.size() - 1);
                }
                if(value.empty()) continue;
                ss.clear();
                ss << value;
                ss >> bssCount;
                bssCount -= 1; // DONT want to count depot as a valid BSS location;
                cout << value << endl;
            }
        }
        else if(line == "CANDIDATE_BSS_COORD_SECTION\r") {
            cout << endl;
            std::pair<float, float> coord;
            int bssIndex;
            while (getline(input_file, value) && value != "END_SECTION\r"){
                if ( !value.empty() && value[value.size()-1] == '\r' ) {
                    value = value.substr(0, value.size() - 1);
                }
                ss.clear();
                ss << value;
                ss >> bssIndex;
                ss >> aux;
                coord.first = (float)aux;
                ss >> aux;
                coord.second = (float)aux;
                if(coord == std::pair<float,float>{1, -1}) { continue; }
                bssCoords.push_back(coord);
                allCoords.push_back(coord);
                cout << value << endl;
            }
        }
    }

    cout << endl << endl;

    std::vector<std::vector<float>> matrix;

    for(int i = 0; i < allCoords.size(); i++){
        std::vector<float> aux;
        aux.reserve(allCoords.size());
    for(int j = 0 ; j <allCoords.size();j++){
            aux.push_back(distance(allCoords[i].first,allCoords[i].second,allCoords[j].first,allCoords[j].second));
        }
        matrix.push_back(aux);
    }

    for(int i = 0; i < nodeCount; i++){
        for(int j = 0 ; j <nodeCount;j++){
         cout << matrix[i][j] << " ";
         if (matrix[i][j] > batteryCapacity){
                batteryCapacity = matrix[i][j];
            }
        }
        cout << endl;
    }
    batteryCapacity = ceil(1.2*batteryCapacity);
    bssCost = ceil(0.5*batteryCapacity);
    problem = new Problem(vehicleCapacity, nVehicles, bssCount, batteryCapacity, nodeCount-1, bssCost, nodeDemands, matrix, allCoords);
    batteryCapacity = batteryCapacity / 2;
    cout << endl << "Bateria Max veículo: " << batteryCapacity << endl;
    input_file.close();

    return problem;
}
