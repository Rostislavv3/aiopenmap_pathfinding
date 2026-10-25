#pragma once
#include <vector>
#include <cstdint>
#include "Edge.h"
#include "Vertex.h"


struct Graph
{
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::vector<uint64_t> real_ids;
};