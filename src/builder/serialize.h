#pragma once
#include "GraphAssembler.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>


inline bool serialize(const Graph& graph, const std::string& outfile){

    std::filesystem::path outfile_fs = outfile;

    std::string outfile_name = outfile_fs.parent_path() / (outfile_fs.stem().stem());
    outfile_name += "_graph.bin";

    std::ofstream ofile(outfile_name, std::ios::binary);

    if(!ofile.is_open()){
        return false;
    }
    
    //vertices vetex, edges Edge, real_ids uint64
    uint64_t size = graph.vertices.size();
    ofile.write(reinterpret_cast<const char*>(&size), sizeof(size));
    ofile.write(reinterpret_cast<const char*>(graph.vertices.data()), sizeof(Vertex) * graph.vertices.size());

    size = graph.edges.size();
    ofile.write(reinterpret_cast<const char*>(&size), sizeof(size));
    ofile.write(reinterpret_cast<const char*>(graph.edges.data()), sizeof(Edge) * graph.edges.size());

    size = graph.real_ids.size();
    ofile.write(reinterpret_cast<const char*>(&size), sizeof(size));
    ofile.write(reinterpret_cast<const char*>(graph.real_ids.data()), sizeof(uint64_t) * graph.real_ids.size());

    return ofile.good();
}

