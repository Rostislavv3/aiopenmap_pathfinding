#include <unordered_map>
#include "common/Edge.h"
#include "common/Vertex.h"
#include "GraphExtractor.h"

class GraphAssembler
{
private:
    std::unordered_map<uint64_t, Vertex> graph;
    GraphExtractor *handler;
public:
    std::unordered_map<uint64_t, Vertex> getGraph();
    GraphAssembler(GraphExtractor &handler);
};

