#include <iostream>
#include <fstream>

#include "grph/graph/common/AdjacencyMatrix.hpp"
#include "grph/utils/parse/TGFParser.hpp"
#include "grph/graph/random/RandomGraphUtils.hpp"

int main() {
    std::ifstream ifs("not_connected.tgf");
    auto matrix = grph::utils::TGFParser::parseFile(ifs);

    std::cout << grph::graph::random::RandomGraphUtils::calculateATR(matrix);
}
