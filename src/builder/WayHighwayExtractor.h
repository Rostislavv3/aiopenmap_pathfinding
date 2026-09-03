#pragma once
#include <osmium/handler.hpp>
#include <osmium/osm/way.hpp>
#include <exception>
#include <unordered_set>
#include <vector>


struct RoadSequence
{
    std::vector<uint64_t> road_sequence;
    size_t speed_limit_kmh;
};


class WayHighwayExtractor : public osmium::handler::Handler {
    public:
        std::unordered_set<uint64_t> getNodes();
        void way(const osmium::Way& node);
    private:
        std::unordered_set<uint64_t> node_ids;
        std::vector<RoadSequence> road_sequences;
};