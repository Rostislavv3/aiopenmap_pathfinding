#pragma once
#include "GraphExtractor.h"
#include "common/Graph.h"



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
