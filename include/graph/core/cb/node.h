//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/graph_context.h>
#include <graph/util/tuple_element_by_type.h>
#include <tuple>
#include <vector>

GRAPH_DSL_NS_BEGIN

struct link;
struct node_cb {};

template<typename T>
struct generic_node_cb : node_cb {};


template<typename NODE, typename NODES>
struct node_impl {
   constexpr static int Index = tuple_element_by_type<generic_node_cb<NODE>, NODES>::Index;
   static_assert(Index >= 0, "cannot get the node control block");

   auto build(graph_context& context) -> status_t {
      return context.get_nodes<NODES, Index>().build(context);
   }

   auto build(graph_context& context, std::vector<link*>& links) -> status_t {
      return context.get_nodes<NODES, Index>().build(context, links);
   }
};

GRAPH_DSL_NS_END

#endif //GRAPH_NODE_H
