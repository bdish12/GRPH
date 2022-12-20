#ifndef GRPH_EDGESLIST_HPP
#define GRPH_EDGESLIST_HPP

#include <vector>
#include <map>

#include "AdjacencyMatrix.hpp"
#include "types.hpp"

namespace grph::graph {

    template<typename MetaInfoType = random::empty>
    struct EdgeNode {
        Vertex from{}, to{};
        MetaInfoType metaInfo;

        EdgeNode(const Edge &edge, const MetaInfoType &metaInfo) {
            this->from = edge.from;
            this->to = edge.to;
            this->metaInfo = metaInfo;
        };

        EdgeNode(Vertex from, Vertex to, const MetaInfoType &metaInfo) {
            this->from = from;
            this->to = to;
            this->metaInfo = metaInfo;
        }
    };

    template<typename VertexesRatioType = bool>
    class EdgesList {
    public:
        explicit EdgesList(const AdjacencyMatrix<VertexesRatioType> &adjacencyMatrix) {
            this->numVertexes = adjacencyMatrix.getDimension();

            for (int lineIndex = 0; lineIndex < adjacencyMatrix.getDimension(); ++lineIndex) {
                for (int columnIndex = 0; columnIndex < adjacencyMatrix.getDimension(); ++columnIndex) {
                    if (adjacencyMatrix(lineIndex, columnIndex) != 0) {
                        Edge edge(lineIndex, columnIndex);
                        edges.push_back(EdgeNode(edge, adjacencyMatrix(lineIndex, columnIndex)));
                    }
                }
            }
        }

        void renumberVertexes(const std::map<Vertex, Vertex> &mapping) {
            for (auto &edge : edges) {
                edge.from = mapping.contains(edge.from)
                        ? mapping.at(edge.from)
                        : edge.from;
                edge.to = mapping.contains(edge.to)
                        ? mapping.at(edge.to)
                        : edge.to;
            }
        }

        AdjacencyMatrix<VertexesRatioType> toAdjacencyMatrix() const {
            AdjacencyMatrix<VertexesRatioType> adjacencyMatrix(this->numVertexes);
            for (auto &edge : edges) {
                adjacencyMatrix(edge.from, edge.to) = sizeof(edge.metaInfo) == 0 ? 1 : edge.metaInfo; // TODO: пересмотреть
            }
            return adjacencyMatrix;
        }

    private:
        std::vector<EdgeNode<VertexesRatioType>> edges;
        int numVertexes{};
    };

}

#endif //GRPH_EDGESLIST_HPP
