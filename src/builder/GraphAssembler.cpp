#include "common/Edge.h"
#include "common/Vertex.h"
#include "common/IntToDegrees.h"
#include "GraphAssembler.h"

#include <cstdint>
#include <limits>
#include <vector>
#include "haversine_km.h"

// a place holder to differentiate where vertex block in edges end
constexpr uint32_t NO_EDGE = std::numeric_limits<uint32_t>::max();
constexpr double KMCONV = 1.609344;
constexpr double HRTOSEC = 3600;

namespace
{
    void placeEdge(Graph &graph, size_t currVertexInd, size_t nextVertexInd, unsigned short speed)
    {
        Vertex &currVertex = graph.vertices[currVertexInd];
        Vertex &nextVertex = graph.vertices[nextVertexInd];
        size_t positionInEdges = currVertex.first_edge_ind;

        while (graph.edges[positionInEdges].target_node != NO_EDGE)
        {
            positionInEdges += 1;
        }

        double distance = haversine_km(
            int_to_degrees(currVertex.lat),
            int_to_degrees(currVertex.lon),
            int_to_degrees(nextVertex.lat),
            int_to_degrees(nextVertex.lon));

        graph.edges[positionInEdges] = Edge{
            static_cast<uint32_t>(nextVertexInd),
            static_cast<float>(distance),
            speed,
            static_cast<float>(distance / speed / KMCONV * HRTOSEC)};
    }

    void placeEdges(const GraphExtractor &handler, Graph &graph)
    {
        const std::vector<ExtractedWay> &extractedWays = handler.getExtractedWays();
        const std::vector<uint32_t> &allWays = handler.getAllWays();

        for (size_t i = 0; i < extractedWays.size(); ++i)
        {
            const ExtractedWay &currWay = extractedWays[i];
            size_t start = currWay.index_in_all_ways;
            size_t end = i + 1 < extractedWays.size() ? extractedWays[i + 1].index_in_all_ways : allWays.size();

            while (start < end)
            {
                // doesn't look for pairs in invalid single value ways
                // use all ways to look up the vertex and increment the count of the vertex on the same index
                if (currWay.direction == ExtractedWay::Direction::Forward)
                {
                    if (start + 1 < end){
                        placeEdge(graph, allWays[start], allWays[start + 1], static_cast<unsigned short>(currWay.speed_limit_mph));
                    }
                    start += 1;
                }
                else if (currWay.direction == ExtractedWay::Direction::Reverse)
                {
                    if (end - 1 > start)
                    {
                         placeEdge(graph, allWays[end - 1], allWays[end - 2], static_cast<unsigned short>(currWay.speed_limit_mph));
                    }
                    end -= 1;
                }
                else if (currWay.direction == ExtractedWay::Direction::Bidirectional)
                {
                    if (start + 1 < end)
                    {
                         placeEdge(graph, allWays[start], allWays[start + 1], static_cast<unsigned short>(currWay.speed_limit_mph));
                          placeEdge(graph, allWays[start + 1], allWays[start], static_cast<unsigned short>(currWay.speed_limit_mph));
                    }
                    start += 1;
                }
            }
        }
    }

    void initializeEdges(Graph &graph, size_t count)
    {
        graph.edges.assign(count, Edge{NO_EDGE, 0.0, 0, 0.0});
        uint32_t running_sum = 0;
        for (Vertex &currVert : graph.vertices)
        {
            size_t num_edges = currVert.first_edge_ind;
            currVert.first_edge_ind = running_sum;
            running_sum += num_edges;
        }
    }

    size_t countEdges(const GraphExtractor &handler, Graph &graph)
    {
        size_t sum = 0;
        const std::vector<ExtractedWay> &extractedWays = handler.getExtractedWays();
        const std::vector<uint32_t> &allWays = handler.getAllWays();

        for (size_t i = 0; i < extractedWays.size(); ++i)
        {
            const ExtractedWay &currWay = extractedWays[i];
            size_t start = currWay.index_in_all_ways;
            size_t end = i + 1 < extractedWays.size() ? extractedWays[i + 1].index_in_all_ways : allWays.size();

            while (start < end)
            {
                // doesn't look for pairs in invalid single value ways
                // use all ways to look up the vertex and increment the count of the vertex on the same index
                if (currWay.direction == ExtractedWay::Direction::Forward)
                {
                    if (start + 1 < end)
                    {
                        graph.vertices[allWays[start]].first_edge_ind += 1;
                        sum += 1;
                    }
                    start += 1;
                }
                else if (currWay.direction == ExtractedWay::Direction::Reverse)
                {
                    if (end - 1 > start)
                    {
                        graph.vertices[allWays[end - 1]].first_edge_ind += 1;
                        sum += 1;
                    }
                    end -= 1;
                }
                else if (currWay.direction == ExtractedWay::Direction::Bidirectional)
                {
                    if (start + 1 < end)
                    {
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

    void buildVertices(const GraphExtractor &handler, Graph &graph)
    {
        size_t num_vertices = handler.getNodeLocs().size();
        const std::vector<NodeLocation> &node_locs = handler.getNodeLocs();
        graph.vertices.reserve(num_vertices);

        for (size_t i = 0; i < num_vertices; ++i)
        {
            const NodeLocation &currLoc = node_locs[i];
            graph.vertices.push_back(Vertex{currLoc.lat, currLoc.lon, 0});
        }
    }

    void buildEdges(const GraphExtractor &handler, Graph &graph)
    {
        size_t sum_edges = countEdges(handler, graph);
        initializeEdges(graph, sum_edges); // initialize edges' array and set the respective vertex index using runnung sum
        placeEdges(handler, graph);        // places edges and calculates distances between them
    }
}

// constructor
GraphAssembler::GraphAssembler(GraphExtractor &h) : handler(h) {};

// populates the graph struct
void GraphAssembler::BuildGraph()
{
    buildVertices(handler, graph); // populates graph's vertices
    buildEdges(handler, graph);    // populates edges
}
