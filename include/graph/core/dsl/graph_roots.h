//
// Created by Darwin Yuan on 2020/8/18.
//

#ifndef GRAPH_GRAPH_ROOTS_H
#define GRAPH_GRAPH_ROOTS_H

#include <graph/graph_ns.h>
#include <graph/core/cb/root_node_cb.h>

GRAPH_DSL_NS_BEGIN

// NODES: root_node_cb
template<typename ... NODES>
struct graph_roots final {
   using type = root_nodes<NODES...>;

   auto build(graph_context& context) -> status_t {
      GRAPH_EXPECT_SUCC(tuple_foreach(roots_, [&](auto& root) { return root.build(context); }));
      context.update_root_nodes(roots_);
      return status_t::Ok;
   }

   auto cleanup() {
      tuple_foreach_void(roots_, [&](auto& root) { root.cleanup(); });
   }

   auto stop() { tuple_foreach_void(roots_, [](auto& root){ root.stop(); }); }

   template<size_t I>
   auto get() -> decltype(auto) { return (roots_.template get<I>()); }

public:
   type roots_;
};

GRAPH_DSL_NS_END

#define __g_ROOTS(...) std::decay_t<decltype(std::declval<GRAPH_DSL_NS::graph_roots<__VA_ARGS__>>())>

#endif //GRAPH_GRAPH_ROOTS_H
