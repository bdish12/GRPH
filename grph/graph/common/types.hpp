#ifndef GRPH_TYPES_HPP
#define GRPH_TYPES_HPP

#include <sstream>

namespace grph::graph {

    struct Edge {
        int from, to;

        Edge(int from, int to) : from(from), to(to) {};

        std::string toString() const {
            std::stringstream ss;
            ss << "(" << from << ", " << to << ')';
            return ss.str();
        }
    };

    namespace random {

        struct empty {
        };

        typedef double EdgeProbability;

    }

}
#endif //GRPH_TYPES_HPP
