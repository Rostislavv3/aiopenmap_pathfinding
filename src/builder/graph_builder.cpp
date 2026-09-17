#include <iostream>
// 1. For reading the compressed .pbf file format
#include <osmium/io/pbf_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>

#include <osmium/osm/node.hpp>
#include <osmium/osm/way.hpp>
#include <unordered_set>
#include <unordered_map>

#include "GraphExtractor.h"
#include "common/Edge.h"
#include "common/Vertex.h"

//needs to be refactored
void extract_graph(const std::string& filename, GraphExtractor& handler)
{
    {
    osmium::io::Reader way_reader{filename, osmium::osm_entity_bits::way};
    osmium::apply(way_reader, handler);
    way_reader.close();
    }  
    
    {
    osmium::io::Reader node_reader{filename, osmium::osm_entity_bits::node};
    osmium::apply(node_reader, handler);
    node_reader.close();
    }  
    
}



int main(int argc, char *argv[])
{
    std::string filename = "data/us-midwest.osm.pbf";
    GraphExtractor handler {}; 

    extract_graph(filename, handler);
    //build the graph


    return 0;
}