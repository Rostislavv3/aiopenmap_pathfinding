#pragma once

#include <cstdint>
#include "Edge.h"

constexpr int32_t NO_LOCATION = INT32_MAX;


struct Vertex
{
    int32_t lat;
    int32_t lon;
    uint32_t first_edge_ind;
};

inline bool hasLocation(const Vertex& v){
    return  v.lat != NO_LOCATION && v.lon != NO_LOCATION;
}
