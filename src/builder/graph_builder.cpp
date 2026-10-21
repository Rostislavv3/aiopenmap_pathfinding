#include <iostream>
#include <fstream>
// 1. For reading the compressed .pbf file format
#include <osmium/io/pbf_input.hpp>
#include <osmium/visitor.hpp>

#include <osmium/osm/node.hpp>
#include <osmium/osm/way.hpp>

#include "GraphExtractor.h"
#include "GraphAssembler.h"
#include "serialize.h"
#include <string_view>

void extract_graph(const std::string &filename, GraphExtractor &handler)
{
    {
        osmium::io::Reader way_reader{filename, osmium::osm_entity_bits::way};
        osmium::apply(way_reader, handler);
        way_reader.close();
    }
    std::cout << "stage 1 pass" << std::endl;
    std::cout << "prepare for node extraction" << "\n";
    handler.prepare_for_node_extraction();
    std::cout << "prepare for node extraction - done" << "\n";
    {
        osmium::io::Reader node_reader{filename, osmium::osm_entity_bits::node};
        osmium::apply(node_reader, handler);
        node_reader.close();
    }
    std::cout << "stage 2 pass" << "\n";

    // to be implemented 
    handler.remove_missing_nodes();
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Use: ./build/GraphBuilder filename.osm.pbf [-d|--debug] [-g|--geojson]" << "\n";
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream ifile(filename);
    if (!ifile.is_open())
    {
        std::cerr << "Error: Could not open the file!\n";
        return 1;
    }
    ifile.close();

    GraphExtractor *handler = new GraphExtractor{};

    extract_graph(filename, *handler);
    // build the graph here
    std::cout << "starts graph assembly" << "\n";
    GraphAssembler assembler{*handler};
    assembler.BuildGraph();
    std::cout << "graph assembled" << "\n";


    delete handler;
    handler = nullptr;
    
    // optional flags can come in any order after the filename
    bool debug_json = false;
    bool debug_geojson = false;
    for (int i = 2; i < argc; i++)
    {
        std::string_view arg = argv[i];
        if (arg == "-d" or arg == "--debug")
        {
            debug_json = true;
        }
        else if (arg == "-g" or arg == "--geojson")
        {
            debug_geojson = true;
        }
        else
        {
            std::cerr << "Unknown option: " << arg << "\n";
        }
    }

    if (debug_json)
    {
        std::cout << "starts json graph serialization" << "\n";
        bool success = serialize_json(assembler.getGraph(), filename);
        std::cout << "done json graph serialization, ";
        if (success)
        {
            std::cout << "success." << "\n";
        }
        else
        {
            std::cout << "fail." << "\n";
        }
    }

    if (debug_geojson)
    {
        std::cout << "starts geojson graph serialization" << "\n";
        bool success = serialize_geojson(assembler.getGraph(), filename);
        std::cout << "done geojson graph serialization, ";
        if (success)
        {
            std::cout << "success." << "\n";
        }
        else
        {
            std::cout << "fail." << "\n";
        }
    }

    std::cout << "starts graph serialization" << "\n";
    bool success = serialize(assembler.getGraph(), filename);
    std::cout << "done graph serialization, ";
    if (success)
    {
        std::cout << "success." << "\n";
    }
    else
    {
        std::cout << "fail." << "\n";
    }
    return success ? 0 : 1;

}