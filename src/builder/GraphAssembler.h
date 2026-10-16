#pragma once
#include "common/Edge.h"
#include "common/Vertex.h"
#include "GraphExtractor.h"
#include <vector>

struct Graph
{
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::vector<uint64_t> real_ids;
};

class GraphAssembler
{
private:
    const GraphExtractor &handler;
    Graph graph;

public:
    GraphAssembler(GraphExtractor &handler);
    void BuildGraph();
    const Graph &getGraph() const;
};
