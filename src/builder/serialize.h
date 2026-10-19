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
                  << ", \"sec\": " << graph.edges[j].time_seconds
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