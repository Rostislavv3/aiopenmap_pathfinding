#include "WayHighwayExtractor.h"
#include <iostream>
#include <string_view>

// extracts nodes that are affiliated with roads, 
void WayHighwayExtractor::way(const osmium::Way &way)
{   
    const char* highway_tag = way.tags().get_value_by_key("highway");
    if(!highway_tag){
        return;
    }
    RoadSequence currSequence;

    const char* maxspeed_tag = way.tags().get_value_by_key("maxspeed");

    std::string_view ref_maxspeed_tag = (maxspeed_tag != nullptr) ? maxspeed_tag : "";

    if(!ref_maxspeed_tag.empty()){
        if(ref_maxspeed_tag.find("mph") != std::string::npos){
            currSequence.speed_limit_mph = std::string::stoi(ref_maxspeed_tag.substr(0, ref_maxspeed_tag.find(" ")));
        }
        currSequence.speed_limit_mph = int(maxspeed_tag);
    } 


    for(const osmium::NodeRef &node : way.nodes()){
        node_ids.insert(node.ref());
    }
}

std::unordered_set<uint64_t> WayHighwayExtractor::getNodes(){
    return node_ids;
}
