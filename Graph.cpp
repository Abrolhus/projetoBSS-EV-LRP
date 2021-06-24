#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <list>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <float.h>
#include <iomanip>

using namespace std;

/**************************************************************************************************
 * Defining the Graph's methods
**************************************************************************************************/

// Constructor
Graph::Graph(int order, bool directed, bool weighted_edge, bool weighted_node)
{

    this->order = order;
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
}

// Destructor
Graph::~Graph()
{

    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {

        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters
int Graph::getOrder()
{

    return this->order;
}
int Graph::getNumberEdges()
{

    return this->number_edges;
}
//Function that verifies if the graph is directed
bool Graph::getDirected()
{

    return this->directed;
}
//Function that verifies if the graph is weighted at the edges
bool Graph::getWeightedEdge()
{

    return this->weighted_edge;
}

//Function that verifies if the graph is weighted at the nodes
bool Graph::getWeightedNode()
{

    return this->weighted_node;
}


Node *Graph::getFirstNode()
{

    return this->first_node;
}

Node *Graph::getLastNode()
{

    return this->last_node;
}

// Other methods
/*
    The outdegree attribute of nodes is used as a counter for the number of edges in the graph.
    This allows the correct updating of the numbers of edges in the graph being directed or not.
*/
void Graph::insertNode(int id)
{
    if (first_node != nullptr)
    {
        //caso tenha, cria um novo node, aponta o ultimo pro novo e o novo de torna o ultimo
        Node* novo_node = new Node(id);
        last_node->setNextNode(novo_node);
        last_node = novo_node;
    }
    else
    {
        //caso nao tenha, cria um novo node e ele se torna o ultimo e o primeiro
        Node* novo_node = new Node(id);
        first_node = novo_node;
        last_node = novo_node;
    }
}

void Graph::insertEdge(int id, int target_id, float weight)
{
    //cria um ponteiro para o node desejado e o um auxiliar para o node alvo da aresta
    Node* p = getNode(id);
    Node* aux = getNode(target_id);
    //confere se os nodes existem
    if (p != nullptr && aux != nullptr){
        //confere se a aresta já existe
        if (!p->searchEdge(target_id)){
            //caso o node exista mas a aresta nao, insere a aresta
            p->insertEdge(target_id, weight);
            this->number_edges++;
            // se o grafo for nao-direcionado e nao houver aresta de
            if (this->directed == 0 && !aux->searchEdge(id))
            {
                //insere a aresta de volta
                aux->insertEdge(id, weight);
                this->number_edges++;
            }
        }
    }
}

void Graph::removeNode(int id){
    //cria um ponteiro para o node desejado
    Node* p = getNode(id);

    //retorna caso nao exista o node desejado
    if (p == nullptr)
        return;
    else if (p == first_node) //se o node eh o primeiro, apenas faz o proximo ser o primeiro
        first_node = p->getNextNode();
    else
    {
        //caso seja um node do meio ou o ultimo, cria um ponteiro auxiliar
        Node* aux = first_node;

        //encontra o node anterior ao node desejado
        while (aux->getNextNode() != p)
            aux = aux->getNextNode();

        //se o no desejado for o ultimo, atualiza o ultimo para o anterior
        if (p == last_node)
            last_node = aux;

        //seta o proximo de anterior para o proximo do desejado
        aux->setNextNode(p->getNextNode());
    }

    //deleta o node desejado
    delete p;

}

bool Graph::searchNode(int id)
{
    //cria um ponteiro para o node desejado
    Node* p = getNode(id);

    //retorna falso caso nao encontre e verdadeiro caso encontre
    if (p == nullptr)
        return false;
    else
        return true;
}

Node *Graph::getNode(int id)
{
    //cria ponteiro para percorrer a lista de nodes
    Node* p = first_node;
    //encontra o node com o id desejado
    while (p != nullptr)
    {
        if (p->getId() == id)
            break;
        p = p->getNextNode();
    }
    //retorna o node ou null caso nao encontre
    return p;
}
