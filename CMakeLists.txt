cmake_minimum_required(VERSION 3.16)
project(graph)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/third-party"
)

add_executable(graph main.cpp include/graph/core/graph.h include/graph/graph_ns.h include/graph/core/node.h include/graph/status.h include/graph/core/graph_context.h include/graph/util/assertion.h)