//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_SUBGRAPH_NODE_H
#define GRAPH_SUBGRAPH_NODE_H

#include <graph/graph_ns.h>
#include <cstdint>
#include <iostream>

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

   auto dump() {
      std::cout << node_type::id << ": refs = " << (int)refs_ << std::endl;
   }
private:
   uint8_t refs_{0};
   bool    running{false};
};

GRAPH_DSL_NS_END

#endif //GRAPH_SUBGRAPH_NODE_H
