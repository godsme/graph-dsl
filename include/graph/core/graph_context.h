//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_GRAPH_CONTEXT_H
#define GRAPH_GRAPH_CONTEXT_H

#include <graph/graph_ns.h>
#include <nano-caf/core/actor_system.h>
#include <tuple>

GRAPH_DSL_NS_BEGIN

struct graph_context {
   graph_context(nano_caf::actor_context& context) : actor_context_(context) {}

   template<typename NODES, int Index>
   inline auto get_node() -> decltype(auto) {
      return (std::get<Index>(*reinterpret_cast<NODES*>(subgraph_nodes_)));
   }

   template<typename NODES>
   auto switch_subgraph_context(NODES& nodes) {
      subgraph_nodes_ = reinterpret_cast<void*>(&nodes);
   }

   auto get_actor_context() -> nano_caf::actor_context& {
      return actor_context_;
   }

private:
   void* subgraph_nodes_{};
   nano_caf::actor_context& actor_context_;
};

GRAPH_DSL_NS_END

#endif //GRAPH_GRAPH_CONTEXT_H
