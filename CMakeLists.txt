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
        include/graph/core/dsl/sub_graph.h
        include/graph/graph_ns.h
        include/graph/core/dsl/graph_node.h
        include/graph/status.h
        include/graph/core/graph_context.h
        include/graph/util/assertion.h
        include/graph/core/dsl/graph_port.h
        include/graph/util/tuple_element_by_type.h
        include/graph/core/dsl/down-stream/down_stream_trait.h
        include/graph/function/unique.h
        include/graph/function/group.h
        include/graph/core/sub_graph_analizer.h
        include/graph/util/result_t.h
        include/graph/util/ptr_trait.h
        include/graph/util/either.h
        include/graph/core/cb/subgraph_node_cb.h
        include/graph/core/node_index.h
        include/graph/core/dsl/down-stream/down_stream_node_ref.h
        include/graph/core/dsl/down-stream/down_stream_maybe.h
        include/graph/core/dsl/down-stream/down_stream_either.h
        include/graph/core/dsl/down-stream/down_stream_fork.h
        include/graph/core/dsl/down-stream/down_stream_trait_decl.h
        include/graph/util/hana_tuple_trait.h
        include/graph/core/actor_ports.h
        include/graph/core/cb/root_node_cb.h
        include/graph/core/node_category.h
        include/graph/core/msgs/graph_msgs.h)

add_executable(graph_ut
        tests/ut/test_hana.cpp
        tests/ut/test_node.cpp
        tests/ut/catch.cpp
        ${graph_dsl} tests/ut/test_holo.cpp tests/ut/test_number_string.cpp tests/ut/test_interval.cpp)

add_executable(graph_ft
        tests/ft/test_graph.cpp
        ${graph_dsl} include/holo/holo_ns.h include/holo/types/type_c.h include/holo/types/tuple_t.h include/holo/algo/fold_left.h include/holo/algo/find_if.h include/holo/algo/transform.h include/holo/algo/filter.h include/graph/core/dsl/graph.h include/graph/core/dsl/sub_graph_selector.h include/graph/core/dsl/graph_roots.h include/graph/function/tuple_foreach.h include/graph/util/number_string.h include/graph/core/dsl/target_state_selector.h include/graph/util/interval.h)

target_link_libraries(graph_ut
        ${CMAKE_CURRENT_SOURCE_DIR}/../nano-caf/cmake-build-release/libnano_caf.a)

target_link_libraries(graph_ft
        ${CMAKE_CURRENT_SOURCE_DIR}/../nano-caf/cmake-build-release/libnano_caf.a)

