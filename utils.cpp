//
// Created by abrolhus on 23/06/2021.
//
#include "Graph/Graph.h"
#include <iostream>
#include <sstream>
stringstream graphToGraphViz(Graph* graph){
    stringstream ss;
    ss << "graph weighted {\n";
    for(int i = 0; i < graph->getOrder(); i++){
        auto* node = graph->getNode(i);
        for(Edge* e = node->getFirstEdge(); e != nullptr; e = e->getNextEdge()){

        }
        ss << (int)graph->getNode(i)->getWeight();
        ss << " -> ";
        ss << (int)graph->getNode(j)->getWeight();
    }
    ss << "\n}\n"

}
