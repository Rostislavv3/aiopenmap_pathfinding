#include "WayHighwayExtractor.h"
#include <iostream>
#include <string_view>

int speed_extractor(const char* speed_tag){
    
}

// extracts nodes that are affiliated with roads, 
void WayHighwayExtractor::way(const osmium::Way &way)
{   
    const char* highway_tag = way.tags().get_value_by_key("highway");
    if(!highway_tag){
        return;
    }
    RoadSequence currSequence;

    const char* maxspeed_tag = way.tags().get_value_by_key("maxspeed");
    
    if(maxspeed_tag){
        currSequence.speed_limit_mph = speed_extractor(maxspeed_tag);
    }


    for(const osmium::NodeRef &node : way.nodes()){
        node_ids.insert(node.ref());
    }
}

std::unordered_set<uint64_t> WayHighwayExtractor::getNodes(){
    return node_ids;
}
