#include "GraphExtractor.h"
#include <iostream>
#include <string_view>
#include <charconv>
#include <iterator>

// helpers 
namespace{
    ExtractedWay::Direction extractDirection(const osmium::Way &way){
        const char* oneway_val = way.tags()["oneway"];
        const char* highway_val = way.tags()["highway"];
        const char* junction_val = way.tags()["junction"];

        if (oneway_val) {
            std::string_view val(oneway_val);
            if (val == "yes" || val == "true" || val == "1") {
                return ExtractedWay::Direction::Forward;
            }
            if (val == "-1" || val == "reverse") {
                return ExtractedWay::Direction::Reverse;
            }
            if (val == "no" || val == "false" || val == "0") {
                return ExtractedWay::Direction::Bidirectional;
            }
        }

        if (highway_val) {
            std::string_view hwy(highway_val);
            if (hwy == "motorway" || hwy == "motorway_link") {
                return ExtractedWay::Direction::Forward;
            }
        }

        if (junction_val) {
            std::string_view junc(junction_val);
            if (junc == "roundabout" || junc == "circular") {
                return ExtractedWay::Direction::Forward;
            }
        }
        return ExtractedWay::Direction::Bidirectional;
    }

    void string_speed_extractor(std::string_view &speed_string, double &speed)
    {
        const char *start = speed_string.data();
        const char *end = start + speed_string.size();

        while (start != end && (*start < '0' || *start > '9'))
        {
            start++;
        }

        while (end != start && (*end < '0' || *end > '9'))
        {
            end--;
        }

        std::from_chars(start, end, speed);
    }

    size_t speed_extractor(const char *speed_tag)
    {
        std::string_view ref_to_speed_tag{speed_tag};
        double speed = 0;
        string_speed_extractor(ref_to_speed_tag, speed);


        if (!(ref_to_speed_tag.find("mph") == std::string::npos))
        {
            speed = speed * 1.60934;
        }
        return static_cast<std::size_t>(speed);
    }
}

// extracts nodes that are part of roads and saves their order
void GraphExtractor::way(const osmium::Way &way)
{
    const char *highway_tag = way.tags().get_value_by_key("highway");
    if (!highway_tag)
    {
        return;
    }
    ExtractedWay currentWay;

    const char *maxspeed_tag = way.tags().get_value_by_key("maxspeed");

    if (maxspeed_tag)
    {
        currentWay.speed_limit_mph = speed_extractor(maxspeed_tag);
    }

    std::string_view ref_to_highway_tag(highway_tag);
    if (ref_to_highway_tag == "motorway")
        currentWay.speed_limit_mph = 65;
    else if (ref_to_highway_tag == "motorway_link")
        currentWay.speed_limit_mph = 40;
    else if (ref_to_highway_tag == "trunk")
        currentWay.speed_limit_mph = 55;
    else if (ref_to_highway_tag == "trunk_link")
        currentWay.speed_limit_mph = 35;
    else if (ref_to_highway_tag == "primary")
        currentWay.speed_limit_mph = 45;
    else if (ref_to_highway_tag == "primary_link")
        currentWay.speed_limit_mph = 30;
    else if (ref_to_highway_tag == "secondary")
        currentWay.speed_limit_mph = 35;
    else if (ref_to_highway_tag == "secondary_link")
        currentWay.speed_limit_mph = 25;
    else if (ref_to_highway_tag == "tertiary")
        currentWay.speed_limit_mph = 30;
    else if (ref_to_highway_tag == "tertiary_link")
        currentWay.speed_limit_mph = 20;
    else if (ref_to_highway_tag == "unclasselse ified")
        currentWay.speed_limit_mph = 35;
    else if (ref_to_highway_tag == "residential")
        currentWay.speed_limit_mph = 25;
    else if (ref_to_highway_tag == "living_street")
        currentWay.speed_limit_mph = 15;
    else if (ref_to_highway_tag == "service")
        currentWay.speed_limit_mph = 10;
    else if (ref_to_highway_tag == "track")
        currentWay.speed_limit_mph = 15;
    else
        currentWay.speed_limit_mph = 25;

    for (const osmium::NodeRef &node : way.nodes())
    {
        currentWay.road_sequence.emplace_back(node.ref());
        node_ids.push_back(node.ref());
    }

    // direction
    currentWay.direction = extractDirection(way);

    extracted_ways.push_back(std::move(currentWay)); //avoids unnecessary copy
}

void GraphExtractor::prepare_for_node_extraction(){
    std::sort(node_ids.begin(), node_ids.end()); //sort for binary search
     
    auto last = std::unique(node_ids.begin(), node_ids.end()); //remove dupes
    node_ids.erase(last, node_ids.end()); //erase extra

    node_ids.shrink_to_fit(); // deallocate extra memory
    node_locs.assign(node_ids.size(), NodeLocation{0.0, 0.0}); 
}



void GraphExtractor::node(const osmium::Node &node)
{
    prepare_for_node_extraction();
    if(!node.location().valid()){
        return;
    }

    auto it = std::lower_bound(node_ids.begin(), node_ids.end(), node.id());
    if(it != node_ids.end() && *it == node.id()){

        uint32_t index = static_cast<uint32_t>(std::distance(node_ids.begin(), it));
        node_locs[index] = NodeLocation{node.location().lat(), node.location().lon()};
    }
}

// getters
const std::vector<uint64_t>& GraphExtractor::getNodes() const 
{
    return node_ids;
}

const std::vector<ExtractedWay>& GraphExtractor::getExtractedWays() const
{
    return extracted_ways;
}

const std::vector<NodeLocation>& GraphExtractor::getNodeLocs() const
{
    return node_locs;
}
