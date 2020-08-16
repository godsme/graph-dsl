cmake_minimum_required(VERSION 3.16)
project(graph)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/third-party"
        "${CMAKE_CURRENT_SOURCE_DIR}/third-party/hana/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../nano-caf/include"
)

set(graph_dsl
        include/graph/core/subgraph_desc.h
        include/graph/graph_ns.h
        include/graph/core/node_desc.h
        include/graph/status.h
        include/graph/core/graph_context.h
        include/graph/util/assertion.h
        include/graph/core/link_desc.h
        include/graph/util/tuple_element_by_type.h
        include/graph/core/down-stream/node_like_trait.h
        include/graph/function/unique.h
        include/graph/function/group.h
        include/graph/core/graph_trait.h
        include/graph/util/result_t.h
        include/graph/util/ptr_trait.h
        include/graph/util/either.h
        include/graph/core/cb/subgraph_node.h
        include/graph/core/node_index.h
        include/graph/core/down-stream/node_like_ref.h
        include/graph/core/down-stream/node_like_maybe.h
        include/graph/core/down-stream/node_like_either.h
        include/graph/core/down-stream/node_like_fork.h
        include/graph/core/down-stream/node_like_trait_decl.h
        include/graph/util/hana_tuple_trait.h include/graph/core/actor_ports.h include/graph/core/cb/root_node.h include/graph/core/node_category.h)

add_executable(graph_ut
        tests/ut/test_hana.cpp
        tests/ut/test_node.cpp
        tests/ut/catch.cpp
        ${graph_dsl})

target_link_libraries(graph_ut ${CMAKE_CURRENT_SOURCE_DIR}/../nano-caf/cmake-build-release/libnano_caf.a)

