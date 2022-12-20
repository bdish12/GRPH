cmake_minimum_required(VERSION 3.23)
project(GRPH)

set(CMAKE_CXX_STANDARD 23)
#set(Boost_NO_WARN_NEW_VERSIONS 1)

#set(Boost_INCLUDE_DIR /Users/a.p.evsyukov/develop/boost/boost_1_80_0)
#
#find_package(Boost COMPONENTS graph)

#include_directories(${Boost_INCLUDE_DIR})

add_executable(grph main.cpp grph/graph/random/UndirectedRandomGraph.cpp grph/graph/random/UndirectedRandomGraph.hpp grph/graph/common/AdjacencyMatrix.hpp grph/exceptions/graph/exceptions.h grph/graph/common/types.hpp grph/graph/common/AdjacencyList.cpp grph/graph/common/AdjacencyList.hpp grph/graph/common/EdgesList.hpp)
#target_link_libraries(grph ${Boost_LIBRARIES})
