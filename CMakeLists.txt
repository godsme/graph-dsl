cmake_minimum_required(VERSION 3.16)
project(graph)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/third-party"
        "${CMAKE_CURRENT_SOURCE_DIR}/third-party/hana/include"
)

add_executable(graph include/graph/core/graph_desc.h include/graph/graph_ns.h include/graph/core/node_desc.h include/graph/status.h include/graph/core/graph_context.h include/graph/util/assertion.h include/graph/core/link_desc.h include/graph/core/cb/node.h include/graph/util/tuple_element_by_type.h include/graph/core/node_like_trait.h tests/ut/test_hana.cpp tests/ut/catch.cpp include/graph/function/unique.h tests/ut/test_node.cpp include/graph/function/group.h include/graph/core/graph_trait.h include/graph/core/cb/node_control_block.h include/graph/core/port_desc.h include/graph/core/cb/actor_handle.h include/graph/util/result_t.h include/graph/util/ptr_trait.h include/graph/util/either.h include/graph/core/subgraph_node.h include/graph/core/node_index.h include/graph/core/node_ref.h)