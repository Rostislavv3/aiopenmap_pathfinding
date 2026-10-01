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
#include "GraphAssembler.h"
#include "common/Edge.h"
#include "common/Vertex.h"
#include "haversine_km.h"


void extract_graph(const std::string& filename, GraphExtractor& handler)
{
    {
    osmium::io::Reader way_reader{filename, osmium::osm_entity_bits::way};
    osmium::apply(way_reader, handler);
    way_reader.close();
    }  
    std::cout<<"stage 1 pass" << std::endl;
    
    {
    osmium::io::Reader node_reader{filename, osmium::osm_entity_bits::node};
    osmium::apply(node_reader, handler);
    node_reader.close();
    }  
    std::cout<<"stage 2 pass" << "\n";
}



int main(int argc, char *argv[])
{
    if(argc < 2){
        std::cout << "Use: ./build/GraphBuilder filename.osm.pbf" << "\n";
        return 1;
    }

    std::string filename = argv[1];

    GraphExtractor handler {}; 

    extract_graph(filename, handler);
    //build the graph


    return 0;
}