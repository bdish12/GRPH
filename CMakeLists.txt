cmake_minimum_required(VERSION 3.23)
project(grph)

set(CMAKE_CXX_STANDARD 20)

#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#conan_basic_setup()

find_package(GTest)
#find_package(nlohmann_json 3.11.2 REQUIRED)

# Project specification
add_executable(${PROJECT_NAME}
        main.cpp
        grph/graph/random/RandomGraphUtils.hpp
        grph/graph/common/AdjacencyMatrix.hpp
        grph/exceptions/graph/exceptions.h
        grph/graph/common/types.hpp
        grph/graph/common/AdjacencyList.hpp
        grph/graph/common/EdgesList.hpp
        grph/utils/parse/GMLParser.hpp
        grph/utils/parse/TGFParser.hpp
        grph/graph/common/DegreesVector.hpp grph/graph/random/UndirectedRandomGraph.hpp)

## Add nlohmann_json support to project
#target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

# Use the global target
#target_link_libraries(${PROJECT_NAME} gtest::gtest)
