#include <iostream>

#include "grph/graph/common/AdjacencyMatrix.hpp"

int main() {
    grph::graph::AdjacencyMatrix<int> matrix(12);
    std::cout << matrix(1, 1);
}
