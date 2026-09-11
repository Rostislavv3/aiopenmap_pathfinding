#ifndef EDGE_H
#define EDGE_H

#include <cstddef>
#include <cstdint>

struct Edge
{
    uint64_t target_node;
    double distance_km;
    size_t speed_limit;
    double time_seconds;
};

#endif // EDGE_H
