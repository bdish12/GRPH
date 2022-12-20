#ifndef GRPH_UNDIRECTEDRANDOMGRAPH_H
#define GRPH_UNDIRECTEDRANDOMGRAPH_H

#include <vector>

#include "../common/AdjacencyMatrix.hpp"
#include "../common/EdgesList.hpp"
#include "../common/types.hpp"
#include "../../exceptions/graph/exceptions.h"

namespace grph::graph::random {

    class UndirectedRandomGraph {
    public:
        explicit UndirectedRandomGraph(const AdjacencyMatrix<EdgeProbability> &adjacencyMatrix)
            : adjacencyMatrix(adjacencyMatrix) {};

        static UndirectedRandomGraph &&readFromFile(const std::string &fileName);

        static double calculateConnectivityProbability(const UndirectedRandomGraph &undirectedRandomGraph) {
            auto graph = undirectedRandomGraph; // TODO перегрузить оператор =
            return 0;
        }

    private:
        const std::vector<Vertex> findResolvingChain() {

        }

        std::vector<AdjacencyMatrix<EdgeProbability>> isConnected() {
            std::vector<AdjacencyMatrix<EdgeProbability>> res;
            return res;
        } // TODO: возвращать компоненты связности, использовать метод из BGL

        void renumberPairOfVertices(int firstVertexNum, int secondVertexNum) {
            adjacencyMatrix.swapLines(firstVertexNum, secondVertexNum);

            for (int lineIdx = 0; lineIdx < adjacencyMatrix.getDimension(); ++lineIdx) {
                std::swap(adjacencyMatrix(lineIdx, firstVertexNum),
                          adjacencyMatrix(lineIdx, secondVertexNum));
            }
            // TODO: добавить обновление вектора степеней?
        }

        void renumberVertexes(const std::map<Vertex, Vertex> &mapping) {
            EdgesList<EdgeProbability> edgesList(this->adjacencyMatrix);
            edgesList.renumberVertexes(mapping);
            this->adjacencyMatrix = edgesList.toAdjacencyMatrix();
        }

        void edgeContraction(const Edge &edge) {
            if (isNotValidEdge(edge)) {
                std::stringstream ss;
                ss << "Invalid edge: " << edge.toString() << " for graph with curDimension: "
                   << curNumVertexes();
                throw exceptions::invalid_edge(ss.str());
            }

            auto updatedEdge = moveEdgeTailToVertexesEnd(edge);
            auto remainingVertex = updatedEdge.from;
            auto vertexToRemove = updatedEdge.to;

            adjacencyMatrix(remainingVertex, vertexToRemove) = 0;
            // TODO: добавить OpenMP
            for (int lineIndex = 0; lineIndex < curNumVertexes(); ++lineIndex) {
                if (lineIndex != remainingVertex) {
                    adjacencyMatrix(lineIndex, remainingVertex) = 1 -
                            (1 - (adjacencyMatrix(remainingVertex, lineIndex))
                            * (1 - adjacencyMatrix(vertexToRemove, lineIndex)));

                    adjacencyMatrix(remainingVertex, lineIndex) = adjacencyMatrix(lineIndex, remainingVertex);
                }
            }
            removeLastVertex();
        }

        inline bool isNotValidEdge(const Edge &edge) {
            bool invalidFormat = edge.from < 0 || edge.to < 0
                    || edge.from > curNumVertexes() || edge.to > curNumVertexes();
            bool verticesNotConnected = adjacencyMatrix(edge.from, edge.to) == 0;
            return invalidFormat || verticesNotConnected;
        }

        // возвращает ребро, to часть которого становится вершиной с наибольшим номером в графе
        Edge moveEdgeTailToVertexesEnd(const Edge &edge) {
            if (edge.from == curNumVertexes() - 1) {
                return {edge.to, edge.from};
            } else if (edge.to == curNumVertexes() - 1) {
                return edge;
            } else {
                renumberPairOfVertices(edge.to, curNumVertexes() - 1);
                return {edge.from, curNumVertexes() - 1};
            }
        }

        inline void removeLastVertex() {
            auto lastVertex = curNumVertexes() - 1;
            if (lastVertex == -1) {
                return;
            }

            for (int lineIndex = 0; lineIndex < curNumVertexes(); ++lineIndex) {
                adjacencyMatrix(lineIndex, lastVertex) = 0;
            }
            adjacencyMatrix.decrementDimension();
            verticesDegrees.pop_back();
        }

        inline void removeVertex(Vertex vertexToRemove) {
            if (curNumVertexes() < 2) {
                return;
            }

            if (vertexToRemove != curNumVertexes() - 1) {
                renumberPairOfVertices(vertexToRemove, curNumVertexes() - 1);
            }
            removeLastVertex();
        }

        inline int curNumVertexes() const {
            return adjacencyMatrix.getDimension();
        }

        int calcVertexDegree(Vertex vertex) {
            int degree = 0;
            for (int columnIndex = 0; columnIndex < curNumVertexes(); ++columnIndex) {
                if (adjacencyMatrix(vertex, columnIndex) != 0) {
                    degree++;
                }
            }
            return degree;
        }

        void updateVerticesDegrees() {
            verticesDegrees.clear();
            for (int lineIdx = 0; lineIdx < curNumVertexes(); ++lineIdx) {
                int curVertexDegree = 0;
                for (int columnIdx = 0; columnIdx < curNumVertexes(); ++columnIdx) {
                    if (adjacencyMatrix(lineIdx, columnIdx) != 0) {
                        curVertexDegree++;
                    }
                }
                verticesDegrees.push_back(curVertexDegree);
            }
            std::sort(verticesDegrees.begin(), verticesDegrees.end());
        }

    private:
        AdjacencyMatrix<EdgeProbability> adjacencyMatrix;
        std::vector<int> verticesDegrees; // храним всегда отсортированным // не отображается в вершины
    };

}

#endif //GRPH_UNDIRECTEDRANDOMGRAPH_H
