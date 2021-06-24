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

float distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}


int main(int argc, char const *argv[]) {
    Graph* leitura(ifstream& input_file);
    Graph* leituraParaGraph(ifstream& input_file);
    ifstream input_file;
    input_file.open(argv[1], ios::in);
    leitura(input_file);
    cout << "...." << endl;
    input_file.open(argv[1], ios::in);
    leituraParaGraph(input_file);

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

    cout << endl << endl;
    
    std::vector<std::vector<float>> matrix;

    for(int i = 0; i < nodeCount; i++){
        std::vector<float> aux;
        for(int j = 0 ; j <nodeCount;j++){
            aux.push_back(distance(nodeCoords[i].first,nodeCoords[i].second,nodeCoords[j].first,nodeCoords[j].second));
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

    batteryCapacity = batteryCapacity / 2;
    cout << endl << "Bateria Max veículo: " << batteryCapacity << endl;
    input_file.close();

    return graph;
}

Graph* leituraParaGraph(ifstream& input_file){
    //TODO(abreu): separar ou juntar parte de leitura de arquivo com criacao do grafo.
    Graph* graph;

    int nodeCount, vehicleCapacity, nodeBase, batteryCapacity, bssCost;
    int bssCount;
    std::vector<std::pair<int, int>> nodeCoords;
    std::vector<std::pair<int, int>> bssCoords;
    std::vector<int> nodeDemands;

    // auxiliar variables:
    stringstream ss;
    int aux;
    string line;
    string value;

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

    graph = new Graph(nodeCount, false, true, false);
    for(int i = 0; i < nodeCoords.size(); i++){
        graph->insertNode(i);
        graph->getNode(i)->setWeight((float)nodeDemands[i]);
    } // add N nodes to the graph with their respective demand stored as their weight;
    for(int i = 0; i < nodeCoords.size(); i++){
        for(int j = i+1; j < nodeCoords.size(); j++){
            graph->insertEdge(i, j, distance(nodeCoords[i].first, nodeCoords[i].second, nodeCoords[j].first, nodeCoords[j].second));
        }
    }
    // for(int i = 0; i < bssCoords.size(); i++){
        // for(int j = 0; j < bssCoords.size(); j++){
            // // graph->insertEdge(i, j, distance(bssCoords[i].first, bssCoords[i].second, bssCoords[j].first, bssCoords[j].second));
            // graph->insertNode(id);
            // id++;
        // }
    // }


    return graph;
}
