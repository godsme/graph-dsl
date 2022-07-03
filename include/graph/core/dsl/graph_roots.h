//
// Created by Darwin Yuan on 2020/8/18.
//

#ifndef GRAPH_GRAPH_ROOTS_H
#define GRAPH_GRAPH_ROOTS_H

#include <graph/graph_ns.h>
#include <graph/core/cb/RootNodeCb.h>

GRAPH_DSL_NS_BEGIN

// NODES: RootNodeCb
template<typename ... NODES>
struct graph_roots final {
   using type = RootNodes<NODES...>;

   auto Build(GraphContext& context) -> Status {
      GRAPH_EXPECT_SUCC(tuple_foreach(roots_, [&](auto& root) { return root.Build(context); }));
      context.UpdateRootNodes(roots_);
      return Status::OK;
   }

   auto CleanUp() {
      tuple_foreach_void(roots_, [&](auto& root) { root.CleanUp(); });
   }

   auto Stop() { tuple_foreach_void(roots_, [](auto& root){ root.Stop(); }); }

   template<size_t I>
   auto Get() -> decltype(auto) { return (roots_.template get<I>()); }

public:
   type roots_;
};

GRAPH_DSL_NS_END

#define __g_ROOTS(...) __MACO_template_type(GRAPH_DSL_NS::graph_roots<__VA_ARGS__>)

#endif //GRAPH_GRAPH_ROOTS_H
