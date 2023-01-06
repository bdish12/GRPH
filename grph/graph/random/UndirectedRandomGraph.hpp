#ifndef GRPH_UNDIRECTEDRANDOMGRAPH_HPP
#define GRPH_UNDIRECTEDRANDOMGRAPH_HPP

#include <vector>
#include <set>
#include <list>
#include <memory>

#include "../common/AdjacencyMatrix.hpp"
#include "../../exceptions/graph/exceptions.h"
#include "../common/DegreesVector.hpp"

#define NOT_FOUND (-1)
#define TRIVIAL_CHAIN_LENGTH 3

namespace grph::graph::random {

    template<typename VertexesRatioType = EdgeProbability>
    class UndirectedRandomGraph {
    public:
        explicit UndirectedRandomGraph(const std::shared_ptr<AdjacencyMatrix<VertexesRatioType>> &adjacencyMatrix)
                : adjacencyMatrix(adjacencyMatrix), degreesVector(adjacencyMatrix) {
        };

        int getNumVertexes() const {
            return adjacencyMatrix->getDimension();
        }

        int getVertexDegree(int vertexNum) const {
            return degreesVector.getVertexDegree(vertexNum);
        }

        const DegreesVector& getDegreesVector() const {
            return degreesVector;
        }

        const AdjacencyMatrix<VertexesRatioType>& getAdjacencyMatrix() const {
            return *adjacencyMatrix;
        }

        VertexesRatioType at(int firstVertexNum, int secondVertexNum) const {
            return adjacencyMatrix->at(firstVertexNum, secondVertexNum);
        }

        bool containsHangingVertex() const {
            return degreesVector.containsHangingVertex();
        }

        VertexesRatioType removeHangingVertexWithLowestIndex() {
            auto hangingVertexWithLowestIndex = degreesVector.getHangingVertexWithLowestIndex();
            auto removedEdgeWithRatio = adjacencyMatrix->removeHangingVertex(hangingVertexWithLowestIndex);
            degreesVector.removeHangingVertex(removedEdgeWithRatio.first.to);
            return removedEdgeWithRatio.second;
        }

        int findFirstVertexThatDegreeEqualTwo() const {
            return degreesVector.findFirstVertexThatDegreeEqualTwo();
        }

        bool containsChain() const {
            return degreesVector.findFirstVertexThatDegreeEqualTwo() != NOT_FOUND;
        }

        double removeChain(const std::list<int> &chain) { // возвращает ФАКТОР
            if (chain.size() < TRIVIAL_CHAIN_LENGTH) {
                throw std::runtime_error("invalid chain");
            }
            if (chain.size() == TRIVIAL_CHAIN_LENGTH) {
                return removeTrivialChain(chain);
            }

            if (chain.front() == chain.back()) { // прикрепленный цикл
                return 0; // TODO накапливание суммы при поиске цепи
            } else {
                return 0; // то же самое, что и сверху
            }
        }

        double removeTrivialChain(const std::list<int> &trivialChain) {
            if (trivialChain.front() == trivialChain.back()) {
                throw std::runtime_error("invalid trivial chain: parallel edges");
            }

        }

    private:
        std::shared_ptr<AdjacencyMatrix<VertexesRatioType>> adjacencyMatrix;
        DegreesVector degreesVector;
    };

}


#endif //GRPH_UNDIRECTEDRANDOMGRAPH_HPP
