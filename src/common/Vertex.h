#ifndef VERTEX_H
#define VERTEX_H

#include <vector>

#include "Edge.h"

struct Vertex
{
    double lat;
    double lon;
    std::vector<Edge*> outgoing_edges{};
};

#endif // VERTEX_H
