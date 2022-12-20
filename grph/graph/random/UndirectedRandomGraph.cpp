#include "UndirectedRandomGraph.hpp"

#include <sstream>

namespace grph::graph::random {

//    bool UndirectedRandomGraph::isConnected() {
//        return false;
//    }

//    int UndirectedRandomGraph::getDimension() const {
//        return adjacencyMatrix.getDimension();
//    }

//    bool UndirectedRandomGraph::isNotValidEdge(const Edge &edge) {
//        bool invalidFormat = edge.from < 0 || edge.to < 0
//                             || edge.from > adjacencyMatrix.getDimension() || edge.to > adjacencyMatrix.getDimension();
//        bool verticesNotConnected = adjacencyMatrix(edge.from, edge.to) == 0;
//        return invalidFormat || verticesNotConnected;
//    }

//    void UndirectedRandomGraph::edgeContraction(const Edge &edge) {
//        if (isNotValidEdge(edge)) {
//            std::stringstream ss;
//            ss << "Invalid edge: " << edge.toString() << " for graph with curDimension: "
//               << adjacencyMatrix.getDimension();
//            throw exceptions::invalid_edge(ss.str());
//        }
//
//
//    }

//    void UndirectedRandomGraph::renumberVertexes(const std::map<Vertex, Vertex> &mapping) {
//        EdgesList<EdgeProbability> edgesList(this->adjacencyMatrix);
//        edgesList.renumberVertexes(mapping);
//        this->adjacencyMatrix = edgesList.toAdjacencyMatrix();
//    }
}