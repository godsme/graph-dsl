//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_LINK_DESC_H
#define GRAPH_LINK_DESC_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/graph_context.h>
#include <graph/core/node_like_trait.h>
#include <tuple>
#include <vector>

GRAPH_DSL_NS_BEGIN

template<typename T>
struct link_desc;

template<typename PORT, typename NODE_LIKE>
struct link_desc<auto (PORT) -> NODE_LIKE> {
   constexpr static auto node_list = node_like_trait<NODE_LIKE>::node_list;
};

GRAPH_DSL_NS_END

#define __port(name) auto (name)

#endif //GRAPH_LINK_DESC_H
