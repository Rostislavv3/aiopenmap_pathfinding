#include <iostream>
// 1. For reading the compressed .pbf file format
#include <osmium/io/pbf_input.hpp>
#include <osmium/handler.hpp>
#include <osmium/visitor.hpp>

#include <osmium/osm/node.hpp>
#include <osmium/osm/way.hpp>

class NodePrint : public osmium::handler::Handler
{
    int count = 0;

public:
    void node(osmium::Node &node)
    {
        if (count < 5)
        {
            std::cout << "Node ID: " << node.id() << "\n";
            std::cout << "  Lat: " << node.location().lat() << "\n";
            std::cout << "  Lon: " << node.location().lon() << "\n";

            // Check if this node has a "name" tag (like a restaurant or park name)
            const char *name = node.tags().get_value_by_key("name");
            if (name)
            {
                std::cout << "  Name: " << name << "\n";
            }
            std::cout << "-----------------------\n";

            count++;
        }
    }
};


int main(int argc, char *argv[])
{
    std::string filename = "us-midwest.osm.pbf";
    
    try {
        // Initialize the reader
        osmium::io::Reader reader{filename, osmium::osm_entity_bits::way};
        
        // Create an instance of your class
        NodePrint printer;
        
        std::cout << "Starting to read the map file...\n";
        
        // Blast the data from the reader into your printer
        osmium::apply(reader, printer);
        
        reader.close();
        std::cout << "Finished.\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    return 0;
}