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
    
    std::string_view ref_to_highway_tag(highway_tag);
    if (ref_to_highway_tag == "motorway")       currSequence.speed_limit_mph = 65;
    else if (ref_to_highway_tag == "motorway_link")  currSequence.speed_limit_mph = 40;
    else if (ref_to_highway_tag == "trunk")          currSequence.speed_limit_mph = 55;
    else if (ref_to_highway_tag == "trunk_link")     currSequence.speed_limit_mph = 35;
    else if (ref_to_highway_tag == "primary")        currSequence.speed_limit_mph = 45;
    else if (ref_to_highway_tag == "primary_link")   currSequence.speed_limit_mph = 30;
    else if (ref_to_highway_tag == "secondary")      currSequence.speed_limit_mph = 35;
    else if (ref_to_highway_tag == "secondary_link") currSequence.speed_limit_mph = 25;
    else if (ref_to_highway_tag == "tertiary")       currSequence.speed_limit_mph = 30;
    else if (ref_to_highway_tag == "tertiary_link")  currSequence.speed_limit_mph = 20;
    else if (ref_to_highway_tag == "unclasselse ified")   currSequence.speed_limit_mph = 35;
    else if (ref_to_highway_tag == "residential")    currSequence.speed_limit_mph = 25;
    else if (ref_to_highway_tag == "living_street")  currSequence.speed_limit_mph = 15;
    else if (ref_to_highway_tag == "service")        currSequence.speed_limit_mph = 10;
    else if (ref_to_highway_tag == "track")          currSequence.speed_limit_mph = 15;
    else currSequence.speed_limit_mph = 25;

    for (const osmium::NodeRef &node : way.nodes())
    {   
        currSequence.road_sequence.emplace_back(node.ref());
        node_ids.insert(node.ref());
    }
    road_sequences.emplace_back(currSequence);
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
