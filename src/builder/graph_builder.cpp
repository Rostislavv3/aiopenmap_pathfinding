#include <iostream>
// 1. For reading the compressed .pbf file format
#include <osmium/io/pbf_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>

#include <osmium/osm/node.hpp>
#include <osmium/osm/way.hpp>
#include <unordered_set>
#include <unordered_map>

#include "WayHighwayExtractor.h"


std::unordered_set<uint64_t> highwayNodeExtractor(std::string filename)
{
    osmium::io::Reader reader{filename, osmium::osm_entity_bits::way};
    std::unordered_set<uint64_t> nodes {};

    WayHighwayExtractor handler {};

    osmium::apply(reader, handler);


    reader.close();
    return handler.getNodes();
}



int main(int argc, char *argv[])
{
    std::string filename = "data/us-midwest.osm.pbf";

    std::unordered_set<uint64_t> setOfNodes = highwayNodeExtractor(filename);
    std::cout << setOfNodes.size() << std::endl;
    return 0;
}