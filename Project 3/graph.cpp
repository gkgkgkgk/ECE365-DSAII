#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <chrono>
#include "./graph.h"
#include "./hash.h"
#include "./heap.h"

using namespace std;

graph::graph(const string &inputFile)
{
    this->ht = hashTable(0);
    initialize(inputFile);
}

int graph::insertEdge(string &startVertex, string &endVertex, int weight)
{
    bool found = false;
    vertex *start = (vertex *)this->ht.getPointer(startVertex, &found);
    if (!found)
    {
        this->vertices.push_back(vertex(startVertex));
        start = &this->vertices.back();
        this->ht.insert(startVertex, start);
    }

    vertex *end = (vertex *)this->ht.getPointer(endVertex, &found);
    if (!found)
    {
        this->vertices.push_back(vertex(endVertex));
        end = &this->vertices.back();
        this->ht.insert(endVertex, end);
    }

    start->edges.push_back(vertex::edge(end, weight));

    return 0;
}

void graph::initialize(const string &inputFile)
{
    ifstream graphFile(inputFile);
    string line;

    while (getline(graphFile, line))
    {
        stringstream ss(line);
        string startVertex, endVertex;
        int weight;
        ss >> startVertex >> endVertex >> weight;
        this->insertEdge(startVertex, endVertex, weight);
    }
}

bool graph::checkValidStartVertex(string &startVertex)
{
    bool found = false;
    vertex *start = (vertex *)this->ht.getPointer(startVertex, &found);
    if (!found)
    {
        return false;
    }
    return true;
}

void graph::performDijkstra()
{
    string startVertex;

    do
    {
        cout << "Enter name of starting vertex: ";
        cin >> startVertex;
    } while (!checkValidStartVertex(startVertex));

    auto start = chrono::high_resolution_clock::now();

    dijkstra(startVertex);

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    double time = duration.count() / 1000000.0;
    cout << "Total time (in seconds) to apply Dijkstra's algorithm: " << time << endl;

    string outputFile;

    cout << "Enter name of output file: " << endl;
    cin >> outputFile;

    ofstream output(outputFile);
    for (auto &vertex : this->vertices)
    {
        if (vertex.dv >= 1000000000 || vertex.dv < 0)
        {
            output << vertex.name << ": NO PATH" << endl;
        }
        else
        {
            output << vertex.name << ": " << vertex.dv << " [" << getPathString(startVertex, vertex.name) << "]" << endl;
        }
    }
}

string graph::getPathString(const string &startVertex, const string &currentVertex)
{
    if (startVertex == currentVertex)
    {
        return startVertex;
    }
    else
    {
        vertex *v = (vertex *)this->ht.getPointer(currentVertex);
        string pv = v->pv;
        string path = getPathString(startVertex, pv);
        return path + ", " + currentVertex;
    }
}

void graph::dijkstra(string &startVertex)
{
    heap unknownNodes(this->vertices.size());
    tuple<vertex *, vertex *> pairs[vertices.size()];
    int tupleIndex = 0;

    for (auto &v : this->vertices)
    {
        if (v.name == startVertex)
        {
            v.dv = 0;
            v.pv = startVertex;
        }

        unknownNodes.insert(v.name, (v.name == startVertex ? 0 : INT_MAX), &v);
    }

    string vName;
    int dv;
    vertex *v;

    while (unknownNodes.deleteMin(&vName, &dv, &v) != 1)
    {
        for (auto &e : v->edges)
        {
            vertex *w = e.dest;
            bool found;
            int dw = unknownNodes.getKey(w->name, &found);

            if (found && dv + e.weight < dw)
            {
                dw = dv + e.weight;
                unknownNodes.setKey(w->name, dw);
                w->pv = v->name;
                w->dv = dw;
            }
        }
    }
}

graph::vertex::vertex(string name, list<graph::vertex::edge> edges)
{
    this->name = name;
    this->edges = edges;
}

graph::vertex::edge::edge(graph::vertex *dest, int weight)
{
    this->dest = dest;
    this->weight = weight;
}