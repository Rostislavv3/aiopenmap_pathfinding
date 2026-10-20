#pragma once
#include "GraphAssembler.h"
#include <iostream>
#include <fstream>
#include <string>
#include "common/IntToDegrees.h"
#include <filesystem>
#include <iomanip>

inline bool serialize(const Graph &graph, const std::string &outfile)
{

    std::filesystem::path outfile_fs = outfile;

    std::string outfile_name = outfile_fs.parent_path() / (outfile_fs.stem().stem());
    outfile_name += "_graph.bin";

    std::ofstream ofile(outfile_name, std::ios::binary);

    if (!ofile.is_open())
    {
        return false;
    }

    // vertices vertex, edges Edge, real_ids uint64
    uint64_t size = graph.vertices.size();
    ofile.write(reinterpret_cast<const char *>(&size), sizeof(size));
    ofile.write(reinterpret_cast<const char *>(graph.vertices.data()), sizeof(Vertex) * graph.vertices.size());

    size = graph.edges.size();
    ofile.write(reinterpret_cast<const char *>(&size), sizeof(size));
    ofile.write(reinterpret_cast<const char *>(graph.edges.data()), sizeof(Edge) * graph.edges.size());

    size = graph.real_ids.size();
    ofile.write(reinterpret_cast<const char *>(&size), sizeof(size));
    ofile.write(reinterpret_cast<const char *>(graph.real_ids.data()), sizeof(uint64_t) * graph.real_ids.size());

    return ofile.good();
}

inline bool serialize_json(const Graph &graph, const std::string &outfile)
{

    std::filesystem::path outfile_fs = outfile;

    std::string outfile_name = outfile_fs.parent_path() / (outfile_fs.stem().stem());
    outfile_name += "_graph.json";

    std::ofstream ofile(outfile_name, std::ios::trunc);

    if (!ofile.is_open())
    {
        return false;
    }

    ofile << std::setprecision(10);

    ofile << "{";
    ofile << "\"vertex_count\":" << graph.vertices.size() << ", \"edge_count\": " << graph.edges.size() << "," << "\n";

    ofile << "\"vertices\": [" << "\n";
    for (size_t i = 0; i < graph.vertices.size(); i++)
    {
        ofile << "{\"i\": " << i
              << ", \"osm_id\": " << graph.real_ids[i]
              << ", \"lat\": " << int_to_degrees(graph.vertices[i].lat)
              << ", \"lon\": " << int_to_degrees(graph.vertices[i].lon) << ","
              << "\n"
              << "\"edges\": [";

        uint32_t bound = i + 1 == graph.vertices.size() ? graph.edges.size() : graph.vertices[i + 1].first_edge_ind;
        for (uint32_t j = graph.vertices[i].first_edge_ind; j < bound; j++)
        {
            ofile << "{\"to\": " << graph.edges[j].target_node
                  << ", \"km\": " << graph.edges[j].distance_km
                  << ", \"mph\": " << graph.edges[j].speed_limit
                  << "}";
            if (j + 1 < bound)
            {
                ofile << ",";
            }
            ofile << "\n";
        }
        ofile << "]";
        ofile << "}";
        if (i + 1 < graph.vertices.size())
        {
            ofile << ",";
        }
    }
    ofile << "]";
    ofile << "}" << "\n";

    return ofile.good();
}

// writes every edge as a GeoJSON LineString, open the file in geojson.io or QGIS to see the graph on a map
inline bool serialize_geojson(const Graph &graph, const std::string &outfile)
{

    std::filesystem::path outfile_fs = outfile;

    std::string outfile_name = outfile_fs.parent_path() / (outfile_fs.stem().stem());
    outfile_name += "_graph.geojson";

    std::ofstream ofile(outfile_name);

    if (!ofile.is_open())
    {
        return false;
    }

    ofile << std::setprecision(10);

    ofile << "{\"type\": \"FeatureCollection\", \"features\": [" << "\n";

    bool first = true; // features can be skipped, so the comma can't be decided by the index
    size_t skipped = 0;
    for (size_t i = 0; i < graph.vertices.size(); i++)
    {
        const Vertex &from = graph.vertices[i];
        uint32_t bound = i + 1 == graph.vertices.size() ? graph.edges.size() : graph.vertices[i + 1].first_edge_ind;
        for (uint32_t j = from.first_edge_ind; j < bound; j++)
        {
            const Edge &edge = graph.edges[j];
            // skip empty slots and nodes whose location was never found, they would break the map
            if (edge.target_node >= graph.vertices.size() || from.lat == INT32_MAX ||
                graph.vertices[edge.target_node].lat == INT32_MAX)
            {
                skipped++;
                continue;
            }
            const Vertex &to = graph.vertices[edge.target_node];

            if (!first)
            {
                ofile << "," << "\n";
            }
            first = false;

            // GeoJSON coordinates are [lon, lat]
            ofile << "{\"type\": \"Feature\", \"geometry\": {\"type\": \"LineString\", \"coordinates\": ["
                  << "[" << int_to_degrees(from.lon) << ", " << int_to_degrees(from.lat) << "], "
                  << "[" << int_to_degrees(to.lon) << ", " << int_to_degrees(to.lat) << "]]}, "
                  << "\"properties\": {\"from\": " << i
                  << ", \"to\": " << edge.target_node
                  << ", \"from_osm\": " << graph.real_ids[i]
                  << ", \"to_osm\": " << graph.real_ids[edge.target_node]
                  << ", \"km\": " << edge.distance_km
                  << ", \"mph\": " << edge.speed_limit
                  << "}}";
        }
    }
    ofile << "\n" << "]}" << "\n";

    if (skipped > 0)
    {
        std::cout << "geojson: skipped " << skipped << " edges with missing node locations" << "\n";
    }

    return ofile.good();
}