#include <iostream>
#include <string>
#include "./graph.h"

using namespace std;

int main()
{
    string inputFile;
    string startVertex;
    string outputFile;

    cout << "Enter name of graph file: ";
    cin >> inputFile;

    graph g(inputFile);
    g.performDijkstra();

    return 0;
}