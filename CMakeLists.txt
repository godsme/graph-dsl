cmake_minimum_required(VERSION 3.16)
project(graph)

set(CMAKE_CXX_STANDARD 17)

include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/third-party"
        "${CMAKE_CURRENT_SOURCE_DIR}/../nano-caf-2/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../maco/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../holo/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../nano-caf/third-party"
)

set(graph_dsl
        include/graph/core/dsl/sub_graph.h
        include/graph/graph_ns.h
        include/graph/core/dsl/graph_node.h
        include/graph/status.h
        include/graph/core/GraphContext.h
        include/graph/util/assertion.h
        include/graph/core/dsl/graph_port.h
        include/graph/util/tuple_element_by_type.h
        include/graph/core/dsl/down-stream/down_stream_trait.h
        include/graph/core/SubGraphAnalyzer.h
        include/graph/util/result_t.h
        include/graph/util/ptr_trait.h
        include/graph/util/either.h
        include/graph/core/cb/subgraph_node_cb.h
        include/graph/core/NodeIndex.h
        include/graph/core/dsl/down-stream/down_stream_node_ref.h
        include/graph/core/dsl/down-stream/down_stream_maybe.h
        include/graph/core/dsl/down-stream/down_stream_either.h
        include/graph/core/dsl/down-stream/down_stream_fork.h
        include/graph/core/dsl/down-stream/down_stream_trait_decl.h
        include/graph/core/ActorPorts.h
        include/graph/core/cb/RootNodeCb.h
        include/graph/core/NodeCategory.h
        include/graph/core/msgs/graph_msgs.h)

add_executable(graph_ut
        tests/ut/test_node.cpp
        tests/ut/catch.cpp
        ${graph_dsl}
        tests/ut/test_number_string.cpp tests/ut/test_interval.cpp tests/ut/test_device.cpp tests/ut/test_state_transitions.cpp tests/ut/test_multi_device.cpp)

add_executable(graph_ft
        tests/ft/test_graph.cpp
        ${graph_dsl} )

#add_executable(graph_multi_device
#        ${graph_dsl}
#        tests/ft/test_multi_device.cpp)

set(nano-caf-lib ${CMAKE_CURRENT_SOURCE_DIR}/../nano-caf-2/cmake-build-release/libnano_caf.a)

target_link_libraries(graph_ut ${nano-caf-lib})
target_link_libraries(graph_ft ${nano-caf-lib} pthread)
#target_link_libraries(graph_multi_device  pthread ${nano-caf-lib})

