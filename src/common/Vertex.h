#pragma once

#include <cstdint>
#include "Edge.h"

struct Vertex
{
    int32_t lat;
    int32_t lon;
    uint32_t first_edge_ind;
};

