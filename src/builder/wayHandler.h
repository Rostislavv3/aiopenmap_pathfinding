#pragma once
#include <osmium/handler.hpp>
#include <osmium/osm/way.hpp>
#include <exception>


struct EarlyExitException : public std::exception {};

class myWayHandler : public osmium::handler::Handler {
    public:
        size_t way_count = 0;
        void way(const osmium::Way& node);
};