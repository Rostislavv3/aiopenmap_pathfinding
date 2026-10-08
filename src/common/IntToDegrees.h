#pragma once
#include <cstdint>

constexpr double DEGREES_PER_UNIT = 1e-7;

inline double int_to_degrees(int32_t value){
    return static_cast<double>(value) * DEGREES_PER_UNIT;
}