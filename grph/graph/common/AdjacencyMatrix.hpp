#ifndef GRPH_ADJACENCYMATRIX_HPP
#define GRPH_ADJACENCYMATRIX_HPP

#include <memory>
#include <vector>
#include <set>

#include "types.hpp"

#define NOT_FOUND (-1)

namespace grph::graph {

    template<typename VertexesRatioType = bool>
    class AdjacencyMatrix {
    public:
        explicit AdjacencyMatrix(const int dimension) {
            this->INITIAL_DIMENSION = dimension;
            this->curDimension = dimension;
            this->matrix = std::shared_ptr<VertexesRatioType[]>(new VertexesRatioType[dimension * dimension],
                                                                std::default_delete<VertexesRatioType[]>());

            memset(matrix.get(),
                   0,
                   sizeof(VertexesRatioType) * INITIAL_DIMENSION * INITIAL_DIMENSION);
        }

        bool isConnected(int firstVertexNum, int secondVertexNum) const {
            return matrix[firstVertexNum * INITIAL_DIMENSION + secondVertexNum] != 0;
        }

        int getDimension() const {
            return curDimension;
        } // TODO: добавить ограничение на размер

        VertexesRatioType at(int firstVertexNum, int secondVertexNum) const {
            // TODO: добавить выбрасывание исключения
            return matrix[firstVertexNum * INITIAL_DIMENSION + secondVertexNum];
        }

        void setAt(VertexesRatioType value, int lineIndex, int columnIndex) {
            // TODO: добавить выбрасывание исключения
            matrix[lineIndex * INITIAL_DIMENSION + columnIndex] = value;
        }

        void renumberPairOfVertices(int firstVertexNum, int secondVertexNum) {
            if (firstVertexNum == secondVertexNum) {
                return;
            }

            this->swapLines(firstVertexNum, secondVertexNum);

            // TODO: add OpenMP
            // swap columns
            for (int lineIdx = 0; lineIdx < curDimension; ++lineIdx) {
                std::swap(matrix[lineIdx * INITIAL_DIMENSION + firstVertexNum],
                          matrix[lineIdx * INITIAL_DIMENSION + secondVertexNum]);
            }
        }

        int findNeighborWithLowestNumFor(int vertexNum,
                                         const std::set<int> &excludedVerticesSet = std::set < int > ()) const {
            for (int columnIndex = 0; columnIndex < curDimension; ++columnIndex) {
                if (matrix[vertexNum * INITIAL_DIMENSION + columnIndex] != 0
                    && !excludedVerticesSet.contains(columnIndex)) {
                    return columnIndex;
                }
            }
            return NOT_FOUND;
        }

        int findNeighborWithBiggestNumFor(int vertexNum,
                                             const std::set<int> &excludedVerticesSet = std::set < int > ()) const {
            for (int columnIndex = curDimension - 1; columnIndex >= 0; --columnIndex) {
                if (matrix[vertexNum * INITIAL_DIMENSION + columnIndex] != 0
                    && !excludedVerticesSet.contains(columnIndex)) {
                    return columnIndex;
                }
            }
            return NOT_FOUND;
        }

        std::pair<Edge, VertexesRatioType> removeHangingVertex(int vertexToRemove) {
            if (vertexToRemove >= curDimension) {
                throw std::runtime_error("");
            }

            auto vertexToRemoveNeighbor = findHangingVertexNeighbor(vertexToRemove);
            auto ratioValue = at(vertexToRemove, vertexToRemoveNeighbor);
            auto lastVertexNum = curDimension - 1;
            renumberPairOfVertices(vertexToRemove, lastVertexNum);
            decrementDimension();

            return std::make_pair(Edge{vertexToRemove, vertexToRemoveNeighbor}, ratioValue);
        }

        std::shared_ptr<AdjacencyMatrix<VertexesRatioType>> copy() const {
            return std::shared_ptr<AdjacencyMatrix<VertexesRatioType>>(new AdjacencyMatrix(this->matrix,
                                                                                           this->INITIAL_DIMENSION,
                                                                                           this->curDimension));
        }

    private:
        AdjacencyMatrix(const std::shared_ptr<VertexesRatioType[]> &sourceMatrix,
                        int INITIAL_DIMENSION, int curDimension)
                : INITIAL_DIMENSION(INITIAL_DIMENSION), curDimension(curDimension) {
            this->matrix = std::shared_ptr<VertexesRatioType[]>(
                    new VertexesRatioType[INITIAL_DIMENSION * INITIAL_DIMENSION],
                    std::default_delete<VertexesRatioType[]>());

            memcpy(matrix.get(),
                   sourceMatrix.get(),
                   INITIAL_DIMENSION * INITIAL_DIMENSION * sizeof(VertexesRatioType));
        }

        inline void decrementDimension() {
            if (curDimension > 0) {
                --curDimension;
            } // TODO: throw exception
        }

        inline int calculateFirstByteIndexInByteArray(int matrixLineIndex) {
            return matrixLineIndex * INITIAL_DIMENSION * sizeof(VertexesRatioType);
        }

        int findHangingVertexNeighbor(int vertexToRemove) {
            for (int columnIndex = 0; columnIndex < curDimension; ++columnIndex) {
                if (isConnected(vertexToRemove, columnIndex)) {
                    return columnIndex;
                }
            }
            return NOT_FOUND;
            // TODO: throw exception
        }

        // TODO: переписать на использование двумерного массива
        void swapLines(int firstLineIndex, int secondLineIndex) {
            const int lineLengthInBytes = sizeof(VertexesRatioType) * INITIAL_DIMENSION;

            auto buffer = new VertexesRatioType[INITIAL_DIMENSION];
            char *matrixBytesPtr = reinterpret_cast<char *>(matrix.get());

            memcpy(buffer,
                   matrixBytesPtr + calculateFirstByteIndexInByteArray(firstLineIndex),
                   lineLengthInBytes);

            memcpy(matrixBytesPtr + calculateFirstByteIndexInByteArray(firstLineIndex),
                   matrixBytesPtr + calculateFirstByteIndexInByteArray(secondLineIndex),
                   lineLengthInBytes);

            memcpy(matrixBytesPtr + calculateFirstByteIndexInByteArray(secondLineIndex),
                   buffer,
                   lineLengthInBytes);

            delete[]buffer;
        }

    private:
        std::shared_ptr<VertexesRatioType[]> matrix;
        int INITIAL_DIMENSION{};
        int curDimension{};
    };

}

#endif //GRPH_ADJACENCYMATRIX_HPP
