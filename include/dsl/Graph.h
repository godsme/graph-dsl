//
// Created by Darwin Yuan on 2020/8/13.
//

#ifndef GRAPH_DSL_GRAPH_H
#define GRAPH_DSL_GRAPH_H

#include <dsl/Node.h>
#include <type_traits>

template <typename = void, typename ... NODES>
struct Graph;

template<typename ... NODES>
struct Graph<std::enable_if_t<(std::is_base_of_v<NodeSignature, NODES> && ...)>, NODES...> {

};

#endif //GRAPH_DSL_GRAPH_H
