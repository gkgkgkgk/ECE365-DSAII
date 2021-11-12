#include <climits>
#include <string>
#include <list>
#include "./hash.h"

// Should the Graph class be closed to modifications and only be able to run Dijkstra's algorithm?
// Or, should the Graph class be a fully functional graph than you can implement Dijkstra on?

class graph
{
public:
    int insertEdge(std::string &startVertex, std::string &endVertex, int weight);
    graph(const std::string &inputFile);
    void performDijkstra();
    int dv;

private:
    class vertex
    {
    public:
        class edge
        {
        public:
            vertex *dest = {nullptr};
            int weight = {INT_MAX};
            edge(vertex *dest, int weight);
        };
        std::string name = {""};
        std::list<edge> edges;
        int dv = 1000000000;
        std::string pv;
        vertex(std::string name = "", std::list<edge> edges = {});
    };

    void dijkstra(std::string &startVertex);
    bool checkValidStartVertex(std::string &startVertex);
    void initialize(const std::string &inputFile);
    std::string getPathString(const std::string &startVertex, const std::string &currentVertex);

    hashTable ht;
    std::list<vertex> vertices;
};