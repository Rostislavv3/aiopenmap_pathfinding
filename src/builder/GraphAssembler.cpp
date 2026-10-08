#include "common/Edge.h"
#include "common/Vertex.h"
#include "GraphAssembler.h"

#include <cstdint>
#include <limits>
#include <vector>

// a place holder to differentiate where vertex block in edges end
constexpr uint32_t NO_EDGE = std::numeric_limits<uint32_t>::max();


namespace{
    size_t countEdges(const GraphExtractor& handler, Graph& graph){
        size_t sum = 0;
        const std::vector<ExtractedWay>& extractedWays = handler.getExtractedWays();
        const std::vector<uint32_t>& allWays = handler.getAllWays();

        for(size_t i = 0; i < extractedWays.size(); ++i){
            const ExtractedWay& currWay = extractedWays[i];
            size_t start = currWay.index_in_all_ways;
            size_t end = i + 1 < extractedWays.size() ? extractedWays[i + 1].index_in_all_ways : allWays.size();
            
            while (start < end){
                //probably need to refactor this condition
                //doesn't look for pairs in invalid single value ways
                //use all ways to look up the vertex and increment the count of the vertex on the same index
                if(currWay.direction == ExtractedWay::Direction::Forward){
                    if(start + 1 < end){
                        graph.vertices[allWays[start]].first_edge_ind += 1;
                        sum += 1;
                    }
                    start += 1;

                }
                else if(currWay.direction == ExtractedWay::Direction::Reverse){
                    if(end - 1 > start){
                        graph.vertices[allWays[end - 1]].first_edge_ind += 1;
                        sum += 1;
                    }
                    end -= 1;
                }
                else if(currWay.direction == ExtractedWay::Direction::Bidirectional){
                    if(start + 1 < end){
                        graph.vertices[allWays[start]].first_edge_ind += 1;
                        graph.vertices[allWays[start + 1]].first_edge_ind += 1;
                        sum += 2;
                    }
                    start += 1;
                }
            }
        }
        return sum;
    }


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
        size_t sum_edges = countEdges(handler, graph);
        
    }
}

//constructor
GraphAssembler::GraphAssembler(GraphExtractor& h) : handler(h) {};


//populates the graph struct
void GraphAssembler::BuildGraph(){
    buildVertices(handler, graph); //populates graph's vertices
    buildEdges(handler, graph); //populates edges
}
