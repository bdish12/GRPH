#ifndef GRPH_ADJACENCYMATRIX_HPP
#define GRPH_ADJACENCYMATRIX_HPP

#include <memory>
#include <vector>

namespace grph::graph {

    // Иммутабельный: в том смысле, что память неизменяема
    template<typename VertexesRatioType = bool>
    class AdjacencyMatrix {
    public:
        explicit AdjacencyMatrix(int dimension) {
            this->initialDimension = dimension;
            this->curDimension = dimension;
            this->matrix = std::shared_ptr<VertexesRatioType[]>(new VertexesRatioType[dimension * dimension],
                                                                std::default_delete<VertexesRatioType[]>());

            memset(matrix.get(), 0, sizeof(VertexesRatioType) * dimension * dimension);
        }

        AdjacencyMatrix(const AdjacencyMatrix<VertexesRatioType> &other) {
            this->initialDimension = other.initialDimension;
            this->curDimension = other.curDimension;
            this->matrix = std::shared_ptr<VertexesRatioType[]>(new VertexesRatioType[initialDimension * initialDimension],
                                                                std::default_delete<VertexesRatioType[]>());
            memcpy(matrix.get(),
                   other.matrix.get(),
                   sizeof(VertexesRatioType) * initialDimension * initialDimension);
        }

        AdjacencyMatrix(AdjacencyMatrix<VertexesRatioType> &&other) noexcept {
            this->initialDimension = other.initialDimension;
            this->curDimension = other.curDimension;
            this->matrix = other.matrix;
        }

        AdjacencyMatrix<VertexesRatioType>& operator=(const AdjacencyMatrix<VertexesRatioType> &source) {
            if (this != &source) {
                this->initialDimension = source.initialDimension;
                this->curDimension = source.curDimension;
                this->matrix = std::shared_ptr<VertexesRatioType[]>(new VertexesRatioType[initialDimension * initialDimension],
                                                                    std::default_delete<VertexesRatioType[]>());
                memcpy(matrix.get(),
                       source.matrix.get(),
                       sizeof(VertexesRatioType) * initialDimension * initialDimension);
            }
            return *this;
        }

        int getDimension() const {
            return curDimension;
        } // TODO: добавить ограничение на размер

        VertexesRatioType& operator()(int lineIndex, int columnIndex) {
            return matrix.get()[lineIndex * curDimension + columnIndex];
        }

        VertexesRatioType operator()(int lineIndex, int columnIndex) const {
            return matrix.get()[lineIndex * curDimension + columnIndex];
        }

        void swapLines(int firstLineIndex, int secondLineIndex) {
            const int lineLengthInBytes = sizeof(VertexesRatioType) * initialDimension;

            auto buffer = new VertexesRatioType[initialDimension];
            char* matrixBytesPtr = reinterpret_cast<char*>(matrix.get());

            memcpy(buffer, matrixBytesPtr + calcLineBeginIdx(firstLineIndex), lineLengthInBytes);

            memcpy(matrixBytesPtr + calcLineBeginIdx(firstLineIndex),
                   matrixBytesPtr + calcLineBeginIdx(secondLineIndex),
                   lineLengthInBytes);

            memcpy(matrixBytesPtr + calcLineBeginIdx(secondLineIndex), buffer, lineLengthInBytes);

            delete []buffer;
        }

        void decrementDimension() {
            if (curDimension > 0) {
                curDimension--;
            }
        }

//        AdjacencyMatrix<VertexesRatioType>& operator=(const AdjacencyMatrix<VertexesRatioType> &source);

    private:
        inline int calcLineBeginIdx(int lineIndex) {
            return lineIndex * initialDimension;
        }

    private:
        std::shared_ptr<VertexesRatioType[]> matrix;
        std::vector<int> vertexesDegrees;
        int initialDimension{};
        int curDimension{};
    };

}

#endif //GRPH_ADJACENCYMATRIX_HPP
