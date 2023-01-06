#ifndef GRPH_DEGREESVECTOR_HPP
#define GRPH_DEGREESVECTOR_HPP

#include <vector>
#include <map>
#include <algorithm>

#include "types.hpp"
#include "AdjacencyMatrix.hpp"

#define NOT_FOUND (-1)

namespace grph::graph {

    class DegreesVector {
    public:
        explicit DegreesVector(const AdjacencyMatrix<random::EdgeProbability> &adjacencyMatrix)
                : beginIndex(0),
                  endIndex(adjacencyMatrix.getDimension() - 1),
                  mappingVector(std::vector<int>(adjacencyMatrix.getDimension())) {

            for (int vertexIndex = 0; vertexIndex < adjacencyMatrix.getDimension(); ++vertexIndex) {
                degreesVector.emplace_back(vertexIndex);
                for (int lineIndex = 0; lineIndex < adjacencyMatrix.getDimension(); ++lineIndex) {
                    if (adjacencyMatrix.isConnected(vertexIndex, lineIndex)) {
                        degreesVector[vertexIndex].degree++;
                    }
                }
            }

            std::sort(degreesVector.begin(), degreesVector.end(),
                      [](const DegreeNode &n1, const DegreeNode &n2) {
                          if (n1.degree < n2.degree) {
                              return true;
                          } else if (n1.degree == n2.degree) {
                              return n1.vertexNum < n2.vertexNum;
                          } else {
                              return false;
                          }
                      });

            // TODO: OpenMP?
            for (int i = 0; i < degreesVector.size(); ++i) {
                mappingVector[degreesVector[i].vertexNum] = i;
            }
        }

        explicit DegreesVector(const std::shared_ptr<AdjacencyMatrix<random::EdgeProbability>> &adjacencyMatrix)
                : beginIndex(0),
                  endIndex(adjacencyMatrix->getDimension() - 1),
                  mappingVector(std::vector<int>(adjacencyMatrix->getDimension())) {

            for (int vertexIndex = 0; vertexIndex < adjacencyMatrix->getDimension(); ++vertexIndex) {
                degreesVector.emplace_back(vertexIndex);
                for (int lineIndex = 0; lineIndex < adjacencyMatrix->getDimension(); ++lineIndex) {
                    if (adjacencyMatrix->isConnected(vertexIndex, lineIndex)) {
                        degreesVector[vertexIndex].degree++;
                    }
                }
            }

            std::sort(degreesVector.begin(), degreesVector.end(),
                      [](const DegreeNode &n1, const DegreeNode &n2) {
                          if (n1.degree < n2.degree) {
                              return true;
                          } else if (n1.degree == n2.degree) {
                              return n1.vertexNum < n2.vertexNum;
                          } else {
                              return false;
                          }
                      });

            // TODO: OpenMP?
            for (int i = 0; i < degreesVector.size(); ++i) {
                mappingVector[degreesVector[i].vertexNum] = i;
            }
        }

        int findFirstVertexThatDegreeEqualTwo() const {
            for (int i = beginIndex; i <= endIndex; ++i) {
                if (degreesVector[i].degree == 2) {
                    return degreesVector[i].vertexNum;
                } else if (degreesVector[i].degree > 2) {
                    return NOT_FOUND;
                }
            }
            return NOT_FOUND;
        };

        bool containsHangingVertex() const {
            return degreesVector[beginIndex].degree == 1;
        }

        int getHangingVertexWithLowestIndex() const {
            if (degreesVector[beginIndex].degree != 1) {
                throw std::runtime_error("getHangingVertexWithLowestIndex");
            }
            return beginIndex;
        }

        // Происходит при перенумерации последней вершины и висячей, и последующее удаление обновленной последней
        void removeHangingVertex(int removedVertexNeighborNum) {
            // ставим последнюю вершину на место удаленной висячей в векторе отображения
            std::swap(mappingVector[degreesVector[beginIndex].vertexNum],
                      mappingVector[endIndex - beginIndex]); // всего в графе (endIndex - beginIndex + 1) вершин
            beginIndex++; // update degreesVector

            degreesVector[mappingVector[removedVertexNeighborNum]].degree--;
            siftLeft(removedVertexNeighborNum);
        }

        int getVertexDegree(int vertexNum) const {
            return degreesVector[mappingVector[vertexNum]].degree;
        }

//        void recalculate(const AdjacencyMatrix<random::EdgeProbability> &adjacencyMatrix) {
//            for (int degreeNodeIndex = 0; degreeNodeIndex < adjacencyMatrix.getDimension(); ++degreeNodeIndex) {
//                degreesVector[degreeNodeIndex].degree = 0;
//                for (int lineIndex = 0; lineIndex < adjacencyMatrix.getDimension(); ++lineIndex) {
//                    if (adjacencyMatrix.isConnected(degreeNodeIndex, lineIndex)) {
//                        degreesVector[degreeNodeIndex].degree++;
//                    }
//                }
//            }
//
//            std::sort(degreesVector.begin(), degreesVector.end(),
//                      [](const DegreeNode &n1, const DegreeNode &n2) {
//                          if (n1.degree < n2.degree) {
//                              return true;
//                          } else if (n1.degree == n2.degree) {
//                              return n1.vertexNum < n2.vertexNum;
//                          } else {
//                              return false;
//                          }
//                      });
//
//            beginIndex = 0;
//            endIndex = degreesVector.size();
//        }

    private:
        struct DegreeNode {
            int vertexNum;
            int degree;

            explicit DegreeNode(int vertexNum) : vertexNum(vertexNum), degree(0) {}
        };

        void siftLeft(int toSiftLeftIndex) { /// good
            if (toSiftLeftIndex == beginIndex) {
                return;
            }

            int indexToInsert = toSiftLeftIndex - 1; // предполагаем, что можно сместить влево на 1
            while (indexToInsert >= beginIndex
                   && degreesVector[toSiftLeftIndex].degree < degreesVector[indexToInsert].degree) {
                --indexToInsert;
            }
            ++indexToInsert; // чтобы вернуться после ухода за границу
            if (toSiftLeftIndex == indexToInsert) {
                return;
            }

            std::swap(degreesVector[toSiftLeftIndex],
                      degreesVector[indexToInsert]);
            std::swap(mappingVector[degreesVector[toSiftLeftIndex].vertexNum],
                      mappingVector[degreesVector[indexToInsert].vertexNum]);
        }

//        void siftRight(int degreeNodeIndex) {
//            if (degreeNodeIndex == endIndex - 1) {
//                return;
//            }
//
//            int indexToInsert = degreeNodeIndex + 1;
//            while (indexToInsert < endIndex && degreesVector[indexToInsert].degree < degreesVector[degreeNodeIndex].degree) {
//                ++indexToInsert;
//            }
//            --indexToInsert;
//
//            std::swap(degreesVector[degreeNodeIndex],
//                      degreesVector[indexToInsert]);
//            std::swap(mappingVector[degreesVector[degreeNodeIndex].vertexNum],
//                      mappingVector[degreesVector[indexToInsert].vertexNum]);
//        }

        int beginIndex; // cur begin at degreesVector
        int endIndex; // [beginIndex, endIndex]
        std::vector<DegreeNode> degreesVector; // вершины, отсортированные по возрастанию своих степеней
        std::vector<int> mappingVector; // map: (cur-graph-state) vertexNum --> index in degreesVector
    };

}

#endif //GRPH_DEGREESVECTOR_HPP
