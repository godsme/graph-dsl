//
// Created by Darwin Yuan on 2020/8/18.
//

#ifndef GRAPH_GRAPH_ROOTS_H
#define GRAPH_GRAPH_ROOTS_H

#include <graph/graph_ns.h>
#include <graph/core/cb/root_node_cb.h>

GRAPH_DSL_NS_BEGIN

template<typename ... NODES>
struct graph_roots final {
   using type = root_nodes<NODES...>;

   auto start(graph_context& context) -> status_t {
      auto status = tuple_foreach(roots_, [&](auto& root) { return root.build(context); });
      if(status != status_t::Ok) { stop(); }
      return status;
   }

   auto stop() { tuple_foreach_void(roots_, [](auto& root){ root.stop(); }); }

   template<size_t I>
   auto get() -> decltype(auto) { return (roots_.template get<I>()); }

public:
   type roots_;
};

GRAPH_DSL_NS_END

#define __g_ROOTS(...) GRAPH_DSL_NS::graph_roots<__VA_ARGS__>

#endif //GRAPH_GRAPH_ROOTS_H
