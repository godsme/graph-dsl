cmake_minimum_required(VERSION 3.16)
project(graph_dsl)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/third-party"
)

add_library(graph_dsl library.cpp library.h include/type-list/TypeList.h include/type-list/Elem.h include/type-list/Unique.h include/type-list/Fold.h include/dsl/NodeTrait.h include/dsl/Link.h include/dsl/Node.h include/dsl/Graph.h ut/TestSort.cpp)