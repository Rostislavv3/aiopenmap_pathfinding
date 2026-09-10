#include "WayHighwayExtractor.h"
#include <iostream>
#include <string_view>
#include <charconv>


void string_speed_extractor(std::string_view& speed_string, double& speed){
    const char *start = speed_string.data();
    const char *end = start + speed_string.size();

    while(start != end && (*start < '0' || *start > '9')){
        start++; 
    }

    while(end != start && (*end < '0' || *end > '9')){
        end--;
    }

    std::from_chars(start, end, speed);
}

size_t speed_extractor(const char *speed_tag)
{
    std::string_view ref_to_speed_tag{speed_tag};
    double speed = -1;

    if (ref_to_speed_tag.find("mph") == std::string::npos)
    {
        string_speed_extractor(ref_to_speed_tag, speed);
    }else{
        string_speed_extractor(ref_to_speed_tag, speed);
        speed = speed * 1.60934;
    }
    return static_cast<std::size_t>(speed);
}

// extracts nodes that are part of roads and saves their order
void WayHighwayExtractor::way(const osmium::Way &way)
{
    const char *highway_tag = way.tags().get_value_by_key("highway");
    if (!highway_tag)
    {
        return;
    }
    RoadSequence currSequence;

    const char *maxspeed_tag = way.tags().get_value_by_key("maxspeed");

    if (maxspeed_tag)
    {
        currSequence.speed_limit_mph = speed_extractor(maxspeed_tag);
    }


    for (const osmium::NodeRef &node : way.nodes())
    {
        node_ids.insert(node.ref());
    }
}

// getters
std::unordered_set<uint64_t> WayHighwayExtractor::getNodes()
{
    return node_ids;
}

std::vector<RoadSequence> WayHighwayExtractor::getRoadSequences()
{
    return road_sequences;
}
