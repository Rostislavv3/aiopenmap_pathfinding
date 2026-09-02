#include <iostream>
// 1. For reading the compressed .pbf file format
#include <osmium/io/pbf_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>

#include <osmium/osm/node.hpp>
#include <osmium/osm/way.hpp>
#include "WayHighwayExtractor.h"
#include <unordered_set>





int main(int argc, char *argv[])
{   

    std::string filename = "data/us-midwest.osm.pbf";
    osmium::io::Reader reader{filename, osmium::osm_entity_bits::way};

    myWayHandler handler;

    
    osmium::apply(reader, handler);

    reader.close();

    return 0;
}