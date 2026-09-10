#pragma once 
#include <osmium/handler.hpp>
#include <osmium/osm/node.hpp>
#include <unordered_set>


class NodeLocationExtractor : public osmium::handler::Handler {
    public:
        void node(osmium::Node& node);
};