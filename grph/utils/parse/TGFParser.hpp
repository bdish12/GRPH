#ifndef GRPH_TGFPARSER_HPP
#define GRPH_TGFPARSER_HPP

#include <iostream>

#include "../../graph/common/AdjacencyMatrix.hpp"
#include "../../graph/common/types.hpp"

namespace grph::utils {

    class TGFParser {
    public:
        static graph::AdjacencyMatrix<graph::random::EdgeProbability> parseFile(std::istream &istream) {
            std::string dimension;
            std::getline(istream, dimension);

            graph::AdjacencyMatrix<graph::random::EdgeProbability> adjacencyMatrix(std::stoi(dimension));

            std::string nextLine;
            while (std::getline(istream, nextLine)) {
                std::stringstream ss(nextLine);
                int from, to;
                double probability;
                ss >> from >> to >> probability;

                adjacencyMatrix.setAt(probability, from - 1, to - 1);
                adjacencyMatrix.setAt(probability, to - 1, from - 1);
            }

            return adjacencyMatrix;
        }
    };

}

#endif //GRPH_TGFPARSER_HPP
