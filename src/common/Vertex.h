#pragma once

#include <vector>

#include "Edge.h"

struct Vertex
{
    double lat;
    double lon;
    std::vector<Edge*> outgoing_edges{};
};

