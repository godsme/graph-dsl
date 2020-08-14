//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include <graph/graph_ns.h>

GRAPH_DSL_NS_BEGIN

template<typename ... NODES>
struct graph_desc {

};

GRAPH_DSL_NS_END

#define __graph(...) GRAPH_DSL_NS::graph_desc<__VA_ARGS__>

#endif //GRAPH_GRAPH_H
