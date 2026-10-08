#include "common/Edge.h"
#include "common/Vertex.h"
#include "GraphAssembler.h"

#include <cstdint>
#include <limits>
#include <vector>

// a place holder to differentiate where vertex block in edges end
constexpr uint32_t NO_EDGE = std::numeric_limits<uint32_t>::max();


namespace{
    void buildVertices(const GraphExtractor& handler, Graph& graph){
        size_t num_vertices = handler.getNodeLocs().size();
        const std::vector<NodeLocation>& node_locs = handler.getNodeLocs();
        graph.vertices.reserve(num_vertices);

        for(size_t i = 0; i < num_vertices; ++i){
            const NodeLocation& currLoc = node_locs[i];
            graph.vertices.push_back(Vertex{currLoc.lat, currLoc.lon, 0});
        }
    }

    void buildEdges(const GraphExtractor& handler, Graph& graph){
        
    }
}

//constructor
GraphAssembler::GraphAssembler(GraphExtractor& h) : handler(h) {};


//populates the graph struct
void GraphAssembler::BuildGraph(){
    buildVertices(handler, graph); //populates graph's vertices
    buildEdges(handler, graph); //populates edges

}
