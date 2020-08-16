//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_SUBGRAPH_NODE_H
#define GRAPH_SUBGRAPH_NODE_H

#include <graph/graph_ns.h>
#include <nano-caf/core/actor/actor_handle.h>
#include <cstdint>
#include <iostream>
#include <graph/util/tuple_element_by_type.h>
#include <graph/core/graph_context.h>
#include <graph/status.h>

GRAPH_DSL_NS_BEGIN

template<typename NODE>
struct subgraph_node {
   using node_type = NODE;

   inline auto add_ref() -> void {
      refs_++;
   }
   inline auto release() -> void {
      refs_--;
   }
   inline auto enabled() -> bool {
      return refs_ > 0;
   }

private:

   template<typename T>
   struct desc_node_type {
      using type = typename T::node_type;
   };
public:

   template<typename NODE_DESC_TUPLE>
   auto start(graph_context& context, NODE_DESC_TUPLE& nodes_desc) -> status_t {
      if(refs_ == 0) return status_t::Ok;
//      constexpr auto Index = tuple_element_index_v<node_type, NODE_DESC_TUPLE, desc_node_type>;
//      std::get<Index>(nodes_desc);

      return status_t::Ok;
   }


   auto dump() {
      std::cout << node_type::id << ": refs = " << (int)refs_ << std::endl;
   }

   nano_caf::actor_handle actor_handle_;

private:
   uint8_t refs_{0};
   bool    running{false};
};

GRAPH_DSL_NS_END

#endif //GRAPH_SUBGRAPH_NODE_H
