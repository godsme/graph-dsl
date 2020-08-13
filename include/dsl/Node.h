//
// Created by Darwin Yuan on 2020/8/13.
//

#ifndef GRAPH_DSL_NODE_H
#define GRAPH_DSL_NODE_H

#include <type-list/Unique.h>
#include <type-list/Fold.h>

struct NodeSignature {};

template<typename NODE, typename ... LINK>
struct Node : NodeSignature {
private:
   template<typename ACC, typename R>
   struct concat {
      using type = typename ACC::template concat<R>;
   };

public:
   using NodeType = NODE;
   using Decendents = Unique_tl<FoldL_t<concat, TypeList<>, typename LINK::NodeList...>>;

private:
   static_assert(!Elem_v<NodeType, Decendents>, "graph should not be cyclic");
};

#endif //GRAPH_DSL_NODE_H
