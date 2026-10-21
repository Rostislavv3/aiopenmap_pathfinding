#include "GraphExtractor.h"
#include <iostream>
#include <string_view>
#include <charconv>
#include <iterator>
#include <algorithm>
#include "common/Vertex.h"
#include <cmath>

namespace
{
    bool check_access(const osmium::Way &way)
    {
        constexpr const char *keys[] = {"motorcar", "motor_vehicle", "vehicle", "access"};

        for (const char *key : keys)
        {
            std::string_view value(way.tags().get_value_by_key(key, ""));
            if (value.empty())
            {
                continue;
            }
            return value != "no" && value != "private";
        }
        return true; 
    }

    unsigned short default_speed_kmh(const char *highway_tag)
    {
        std::string_view ref_to_highway_tag(highway_tag);
        if (ref_to_highway_tag == "motorway")
            return 105;
        else if (ref_to_highway_tag == "motorway_link")
            return 64;
        else if (ref_to_highway_tag == "trunk")
            return 89;
        else if (ref_to_highway_tag == "trunk_link")
            return 56;
        else if (ref_to_highway_tag == "primary")
            return 72;
        else if (ref_to_highway_tag == "primary_link")
            return 48;
        else if (ref_to_highway_tag == "secondary")
            return 56;
        else if (ref_to_highway_tag == "secondary_link")
            return 40;
        else if (ref_to_highway_tag == "tertiary")
            return 48;
        else if (ref_to_highway_tag == "tertiary_link")
            return 32;
        else if (ref_to_highway_tag == "unclassified")
            return 56;
        else if (ref_to_highway_tag == "residential")
            return 40;
        else if (ref_to_highway_tag == "living_street")
            return 24;
        else if (ref_to_highway_tag == "service")
            return 16;
        else if (ref_to_highway_tag == "track")
            return 24;
        else
            return 0;
    }

    ExtractedWay::Direction extractDirection(const osmium::Way &way)
    {
        const char *oneway_val = way.tags()["oneway"];
        const char *highway_val = way.tags()["highway"];
        const char *junction_val = way.tags()["junction"];

        if (oneway_val)
        {
            std::string_view val(oneway_val);
            if (val == "yes" || val == "true" || val == "1")
            {
                return ExtractedWay::Direction::Forward;
            }
            if (val == "-1" || val == "reverse")
            {
                return ExtractedWay::Direction::Reverse;
            }
            if (val == "no" || val == "false" || val == "0")
            {
                return ExtractedWay::Direction::Bidirectional;
            }
        }

        if (highway_val)
        {
            std::string_view hwy(highway_val);
            if (hwy == "motorway" || hwy == "motorway_link")
            {
                return ExtractedWay::Direction::Forward;
            }
        }

        if (junction_val)
        {
            std::string_view junc(junction_val);
            if (junc == "roundabout" || junc == "circular")
            {
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

        while (end != start && (*(end - 1) < '0' || *(end - 1) > '9'))
        {
            end--;
        }

        std::from_chars(start, end, speed);
    }

    unsigned short speed_extractor(const char *speed_tag)
    {
        std::string_view ref_to_speed_tag{speed_tag};
        double speed = 0;
        string_speed_extractor(ref_to_speed_tag, speed);

        if (ref_to_speed_tag.find("mph") != std::string::npos)
        {
            speed = speed * 1.60934;
        }
        return static_cast<unsigned short>(std::lround(speed));
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

    if (!check_access(way))
    {
        return;
    }

    const char *maxspeed_tag = way.tags().get_value_by_key("maxspeed");

    if (currentWay.speed_limit_kmh == 0)
    {
        currentWay.speed_limit_kmh = default_speed_kmh(highway_tag);
        if (currentWay.speed_limit_kmh == 0)
        {
            return;
        }
    }

    if (maxspeed_tag)
    {
        unsigned short new_speed = speed_extractor(maxspeed_tag);
        if (new_speed > 0)
        {
            currentWay.speed_limit_kmh = new_speed;
        }
    }


    currentWay.index_in_all_ways = raw_refs.size();
    for (const osmium::NodeRef &node : way.nodes())
    {
        raw_refs.emplace_back(node.ref());
    }

    // direction
    currentWay.direction = extractDirection(way);
    extracted_ways.push_back(std::move(currentWay)); // avoids unnecessary copy
}

void GraphExtractor::prepare_for_node_extraction()
{
    node_ids = raw_refs;
    std::sort(node_ids.begin(), node_ids.end()); // sort for binary search

    auto last = std::unique(node_ids.begin(), node_ids.end()); // remove dupes
    node_ids.erase(last, node_ids.end());                      // erase extra

    node_ids.shrink_to_fit(); // deallocate extra memory

    all_ways.reserve(raw_refs.size());
    for (size_t i = 0; i < raw_refs.size(); ++i)
    {
        auto it = std::lower_bound(node_ids.begin(), node_ids.end(), raw_refs[i]);
        uint32_t index = static_cast<uint32_t>(std::distance(node_ids.begin(), it));
        all_ways.push_back(index);
    }

    raw_refs.clear();
    raw_refs.shrink_to_fit();

    node_locs.assign(node_ids.size(), NodeLocation{NO_LOCATION, NO_LOCATION});
}

void GraphExtractor::node(const osmium::Node &node)
{
    if (!node.location().valid())
    {
        return;
    }

    auto it = std::lower_bound(node_ids.begin(), node_ids.end(), node.id());
    if (it != node_ids.end() && *it == node.id())
    {
        uint32_t index = static_cast<uint32_t>(std::distance(node_ids.begin(), it));
        node_locs[index] = NodeLocation{node.location().y(), node.location().x()};
    }
}

// getters
const std::vector<uint64_t> &GraphExtractor::getNodes() const
{
    return node_ids;
}

const std::vector<ExtractedWay> &GraphExtractor::getExtractedWays() const
{
    return extracted_ways;
}

const std::vector<NodeLocation> &GraphExtractor::getNodeLocs() const
{
    return node_locs;
}

const std::vector<uint32_t> &GraphExtractor::getAllWays() const
{
    return all_ways;
}
