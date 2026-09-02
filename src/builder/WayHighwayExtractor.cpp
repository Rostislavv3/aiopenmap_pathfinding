#include "WayHighwayExtractor.h"
#include <iostream>


void myWayHandler::way(const osmium::Way &way)
{   
    std::cout << "Way ID: " << way.id() << '\n';
    for (const osmium::Tag &t : way.tags())
    {
        std::cout << "  " << t.key() << "=" << t.value() << '\n';
    }
    std::cout << "-----------------------\n";

    way_count++;
}
