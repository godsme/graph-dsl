//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_NODE_INDEX_H
#define GRAPH_NODE_INDEX_H

#include <graph/graph_ns.h>
#include <graph/util/tuple_element_by_type.h>
#include <graph/core/graph_context.h>

GRAPH_DSL_NS_BEGIN

template <typename NODE, typename TUPLE>
struct node_index {
private:
   template<typename T> struct trait { using type = typename T::node_type; };
   constexpr static int Index = tuple_element_index_v<NODE, TUPLE, trait>;
   static_assert(Index >= 0, "no type in tuple");

public:
   inline static auto get_node(graph_context& context) -> decltype(auto) {
      return (context.get_node<TUPLE, Index>());
   }

   inline static auto get_root_node(graph_context& context) -> decltype(auto) {
      return (context.get_root_node<TUPLE, Index>());
   }
};

GRAPH_DSL_NS_END

#endif //GRAPH_NODE_INDEX_H
