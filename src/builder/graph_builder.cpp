#include <iostream>
#include <fstream>
// 1. For reading the compressed .pbf file format
#include <osmium/io/pbf_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>

#include <osmium/osm/node.hpp>
#include <osmium/osm/way.hpp>

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
    std::cout << "prepare for node extraction" << "\n";
    handler.prepare_for_node_extraction();
    std::cout << "prepare for node extraction - done" << "\n";
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
    std::ifstream ifile(filename);
    if (!ifile.is_open()) {
        std::cerr << "Error: Could not open the file!\n";
        return 1;
    }
    ifile.close();

    GraphExtractor handler {}; 
    
    extract_graph(filename, handler);
    //build the graph here
    std::cout << "starts graph assembly" << "\n";
    GraphAssembler assembler {handler};
    assembler.BuildGraph();
    std::cout << "graph assembled" << "\n";
    return 0;
}