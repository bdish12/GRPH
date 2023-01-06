#ifndef GRPH_UNDIRECTEDRANDOMGRAPH_H
#define GRPH_UNDIRECTEDRANDOMGRAPH_H

#include <vector>
#include <set>
#include <list>

#include "../common/AdjacencyMatrix.hpp"
#include "../common/EdgesList.hpp"
#include "../common/types.hpp"
#include "../../exceptions/graph/exceptions.h"
#include "../common/DegreesVector.hpp"
#include "UndirectedRandomGraph.hpp"

#define COMPUTABLE_DIMENSION 5

namespace grph::graph::random {

    class RandomGraphUtils {
    public:
        static double calculateATR(const AdjacencyMatrix<EdgeProbability> &adjacencyMatrix) {
            UndirectedRandomGraph<EdgeProbability> graph(adjacencyMatrix.copy());

            auto foundConnectivityComponent = findSomeConnectivityComponent(graph);
            if (!isConnectedGraph(graph, foundConnectivityComponent)) {
                return 0;
            }
            if (graph.getNumVertexes() <= COMPUTABLE_DIMENSION) {
                return caclulateComputableDimensionGraph(graph);
            }

            double R = 1;
            R *= prepareForReduction(graph);



            return R;
        }

    private:
        static double prepareForReduction(UndirectedRandomGraph<EdgeProbability> &graph) {
            double R = 1;
            R *= removeHangingVertexes(graph);

            std::list<int> resolvingChain;
            while (!(resolvingChain = findResolvingChain(graph)).empty()) {
                R *= graph.removeChain(resolvingChain);
                R *= removeHangingVertexes(graph);
            }
            return R;
        }

        static double removeHangingVertexes(UndirectedRandomGraph<EdgeProbability> &graph) {
            double R = 1;
            while (graph.containsHangingVertex()) {
                R *= graph.removeHangingVertexWithLowestIndex();
            }
            return R;
        }

        // TODO: добавить накомлпние суммы для редукции
        static std::list<int> findResolvingChain(const UndirectedRandomGraph<EdgeProbability> &graph) {
            // TODO: попробовать переписать на вектор и замерить разницу в производительности
            std::list<int> foundChain;
            auto begin = graph.getDegreesVector().findFirstVertexThatDegreeEqualTwo();
            if (begin == -1) {
                return foundChain;
            }

            auto prevVertex = begin;
            auto curVertex = begin;
            do {
                foundChain.push_front(graph.getAdjacencyMatrix().findNeighborWithLowestNumFor(curVertex,
                                                                                              {prevVertex}));
                prevVertex = curVertex;
                curVertex = foundChain.front();
            } while (graph.getVertexDegree(curVertex) == 2);

            prevVertex = begin;
            curVertex = begin;
            do {
                foundChain.push_back(graph.getAdjacencyMatrix().findNeighborWithBiggestNumFor(curVertex,
                                                                                              {prevVertex}));
                prevVertex = curVertex;
                curVertex = foundChain.back();
            } while (graph.getVertexDegree(curVertex) == 2);

            return foundChain;
        }

        static std::vector<int> findSomeConnectivityComponent(const UndirectedRandomGraph<EdgeProbability> &graph) {
            std::vector<int> connectivityComponent;
            std::vector<bool> visited(graph.getAdjacencyMatrix().getDimension(), false);

            dfs(graph.getAdjacencyMatrix(), 0, visited, connectivityComponent);
            return connectivityComponent;
        }

        static void dfs(const AdjacencyMatrix<EdgeProbability> &adjacencyMatrix,
                        int curVisitedVertexNum, std::vector<bool> &visited,
                        std::vector<int> &connectivityComponent) {
            visited[curVisitedVertexNum] = true;
            connectivityComponent.push_back(curVisitedVertexNum);
            for (int vertexNum = 0; vertexNum < adjacencyMatrix.getDimension(); ++vertexNum) {
                if (vertexNum != curVisitedVertexNum && adjacencyMatrix.isConnected(curVisitedVertexNum, vertexNum)
                    && !visited[vertexNum]) {
                    dfs(adjacencyMatrix, vertexNum, visited, connectivityComponent);
                }
            }
        }

        static bool isConnectedGraph(const UndirectedRandomGraph<EdgeProbability> &graph,
                                     const std::vector<int> &foundConnectivityComponent) {
            return graph.getAdjacencyMatrix().getDimension() == foundConnectivityComponent.size();
        }

        static void chainReduction(UndirectedRandomGraph<EdgeProbability> &graph, long double &R) {

        }

        static double caclulateComputableDimensionGraph(const UndirectedRandomGraph<EdgeProbability> &graph) {
            return 0;
        }

//        static AdjacencyMatrix<EdgeProbability> renumberVertexes(
//                const AdjacencyMatrix<EdgeProbability> &adjacencyMatrix,
//                const std::map<int, int> &vertexesMapping) {
//            EdgesList<EdgeProbability> edgesList(adjacencyMatrix);
//            edgesList.renumberVertexes(vertexesMapping);
//            return edgesList.toAdjacencyMatrix();
//        }
    };

}

#endif //GRPH_UNDIRECTEDRANDOMGRAPH_H
