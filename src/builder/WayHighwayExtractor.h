#pragma once
#include <osmium/handler.hpp>
#include <osmium/osm/way.hpp>
#include <exception>
#include <unordered_set>


struct EarlyExitException : public std::exception {};

class WayHighwayExtractor : public osmium::handler::Handler {
    public:
        std::unordered_set<uint64_t> getNodes();
        void way(const osmium::Way& node);
    private:
        std::unordered_set<uint64_t> node_ids;
};