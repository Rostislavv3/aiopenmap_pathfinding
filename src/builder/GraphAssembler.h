#pragma once
#include "common/Edge.h"
#include "common/Vertex.h"
#include "GraphExtractor.h"
#include <vector>


struct Graph
{
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
};


class GraphAssembler
{
private:
    const GraphExtractor& handler;
    Graph graph;
public:
    GraphAssembler(GraphExtractor &handler);
    void BuildGraph();
    
};

