#include "WayHighwayExtractor.h"
#include <iostream>

// extracts nodes that are affiliated with roads, 
void WayHighwayExtractor::way(const osmium::Way &way)
{   
    const char* highway_tag = way.tags().get_value_by_key("highway");
    if(!highway_tag){
        return;
    }
    RoadSequence currSequences;

    // if(highway_tag)

    for(const osmium::NodeRef &node : way.nodes()){
        node_ids.insert(node.ref());
    }
}

std::unordered_set<uint64_t> WayHighwayExtractor::getNodes(){
    return node_ids;
}
