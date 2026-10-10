#pragma once

#include <cstddef>
#include <cstdint>

struct Edge
{
    uint32_t target_node;
    float distance_km;
    unsigned short speed_limit;
    float time_seconds;
};
