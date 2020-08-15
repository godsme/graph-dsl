//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_GRAPH_DESC_H
#define GRAPH_GRAPH_DESC_H

#include <graph/graph_ns.h>
#include <graph/core/graph_trait.h>
#include <boost/hana.hpp>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

template<typename ... NODES>
struct graph_desc {
   constexpr static auto all_sorted_nodes = graph_trait<NODES...>::all_sorted_nodes;
};

GRAPH_DSL_NS_END

#define __graph(...) GRAPH_DSL_NS::graph_desc<__VA_ARGS__>

#endif //GRAPH_GRAPH_DESC_H
