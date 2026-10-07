#pragma once
#include <osmium/handler.hpp>
#include <osmium/osm/way.hpp>
#include <osmium/osm/node.hpp>
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
    // instead of keeping a vector for each way, we will keep every way sequence in a separate vector,
    // and only store the beginning index of a particular way
    uint32_t index_in_all_ways;
    size_t speed_limit_mph = 0;
};

struct NodeLocation{
    int32_t lat;
    int32_t lon;
};


class GraphExtractor : public osmium::handler::Handler {
    public:
        const std::vector<uint64_t>& getNodes() const;
        const std::vector<ExtractedWay>& getExtractedWays() const;
        const std::vector<NodeLocation>& getNodeLocs() const;
        const std::vector<uint32_t>& getAllWays() const;
        void prepare_for_node_extraction();

        void way(const osmium::Way& way);
        void node(const osmium::Node& node);

    private:
        std::vector<uint64_t> node_ids;
        std::vector<NodeLocation> node_locs;
        std::vector<ExtractedWay> extracted_ways;
        std::vector<uint64_t> raw_refs; //it is a temp strorage for ids before serialization
        std::vector<uint32_t> all_ways;
};