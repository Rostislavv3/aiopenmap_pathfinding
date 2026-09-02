#include "WayHighwayExtractor.h"
#include <iostream>


void WayHighwayExtractor::way(const osmium::Way &way)
{   
    if(!way.tags().has_key("highway")){
        return;
    }

    for(const osmium::NodeRef &node : way.nodes()){
        node_ids.insert(node.ref());
    }
}

std::unordered_set<uint64_t> WayHighwayExtractor::getNodes(){
    return node_ids;
}
