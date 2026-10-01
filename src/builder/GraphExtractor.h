#pragma once
#include <osmium/handler.hpp>
#include <osmium/osm/way.hpp>
#include <osmium/osm/node.hpp>
#include <unordered_set>
#include <vector>

// this function extracts ways and keeps the order of the nodes in a struct called
// road sequence, then, it supposed to be used again in the reader to extract nodes'
// lontitude and latitude to thereafter use it in graph construction
struct ExtractedWay
{
    // add enum class for road directions
    // retrieve road directions in cpp file
    enum class Direction{
        Reverse,
        Forward,
        Bidirectional,
    };
    Direction direction;
    std::vector<uint64_t> road_sequence;
    size_t speed_limit_mph;
};

struct NodeLocation{
    double lat;
    double lon;
};


class GraphExtractor : public osmium::handler::Handler {
    public:
        const std::unordered_set<uint64_t>& getNodes() const;
        const std::vector<ExtractedWay>& getExtractedWays() const;
        const std::unordered_map<uint64_t, NodeLocation>& getNodeLocs() const;

        void way(const osmium::Way& way);
        void node(const osmium::Node& node);

    private:
        std::unordered_set<uint64_t> node_ids;
        std::unordered_map<uint64_t, NodeLocation> node_locs;
        std::vector<ExtractedWay> extracted_ways;
};