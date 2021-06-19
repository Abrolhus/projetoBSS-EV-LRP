#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <utility>
#include <tuple>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include "Graph.h"
#include "Node.h"
#include <vector>
#include <sstream>

using namespace std;

int main(int argc, char const *argv[]) {
    Graph* leitura(ifstream& input_file);
    ifstream input_file;
    input_file.open(argv[1], ios::in);
    leitura(input_file);
    cout << "....";
    return 0;
}
Graph* leitura(ifstream& input_file){
    Graph* graph;
    string line;
    string value;

    int nodeCount, vehicleCapacity, nodeBase, batteryCapacity, bssCost;
    int bssCount;
    std::vector<std::pair<int, int>> nodeCoords;
    std::vector<std::pair<int, int>> bssCoords;
    std::vector<int> nodeDemands;

    // auxiliar variables:
    stringstream ss;
    int aux;

    while(getline(input_file,line) && line != "END_FILE\r"){
        if(line == "NODE_COUNT_SECTION\r") {
            cout << endl;
            while (getline(input_file, value) && value != "END_SECTION\r"){
                if ( !value.empty() && value[value.size()-1] == '\r' ) {
                    value = value.substr(0, value.size() - 1);
                }
                cout << value << endl;
                nodeCount = std::stoi(value);
                cout << nodeCount << "node coutn << " << endl;
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
            std::pair<int, int> coord;
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
                cout << value << endl;
            }
        }
        else if(line == "CANDIDATE_BSS_COORD_SECTION\r") {
            cout << endl;
            std::pair<int, int> coord;
            int bssIndex;
            while (getline(input_file, value) && value != "END_SECTION\r"){
                if ( !value.empty() && value[value.size()-1] == '\r' ) {
                    value = value.substr(0, value.size() - 1);
                }
                ss.clear();
                ss << value;
                ss >> bssIndex;
                ss >> aux;
                coord.first = aux;
                ss >> aux;
                coord.second = aux;
                bssCoords.push_back(coord);
                cout << value << endl;
            }
        }
    }


    return graph;
}


