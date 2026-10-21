#pragma once
#include <cmath>

inline double haversine_km(double lat1, double lon1, double lat2, double lon2) {
    constexpr double R = 6371.0;
    constexpr double PI = 3.14159265358979323846;
    constexpr double TO_RAD = PI / 180.0;

    double dlat = (lat2 - lat1) * TO_RAD;
    double dlon = (lon2 - lon1) * TO_RAD;

    double a = std::sin(dlat / 2.0) * std::sin(dlat / 2.0) +
               std::cos(lat1 * TO_RAD) * std::cos(lat2 * TO_RAD) *
               std::sin(dlon / 2.0) * std::sin(dlon / 2.0);

    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));
    return R * c;
}