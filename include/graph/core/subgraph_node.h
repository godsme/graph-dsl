//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_SUBGRAPH_NODE_H
#define GRAPH_SUBGRAPH_NODE_H

#include <graph/graph_ns.h>
#include <cstdint>

GRAPH_DSL_NS_BEGIN

template<typename NODE>
struct subgraph_node {
   inline auto add_ref() -> void {
      refs_++;
   }
   inline auto release() -> void {
      refs_--;
   }
private:
   uint8_t refs_{0};
   bool    running{false};
};

GRAPH_DSL_NS_END

#endif //GRAPH_SUBGRAPH_NODE_H
