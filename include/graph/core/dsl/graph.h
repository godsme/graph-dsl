//
// Created by Darwin Yuan on 2020/8/18.
//

#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include <graph/graph_ns.h>
#include <graph/core/dsl/sub_graph_selector.h>
#include <graph/util/assertion.h>
#include <graph/function/tuple_foreach.h>

GRAPH_DSL_NS_BEGIN

template<typename ROOTS, typename ... SUB_GRAPH_SELECTOR>
struct graph final {
private:
   constexpr static auto sequence = std::make_index_sequence<sizeof...(SUB_GRAPH_SELECTOR)>{};

public:
   inline auto build(graph_context& context) -> status_t {
      context.update_root_nodes(roots_.roots_);
      return tuple_foreach(sub_graphs_, [&](auto& sub) {
         return sub.build(context);
      });
   }

   auto start(graph_context& context) -> status_t {
      GRAPH_EXPECT_SUCC(tuple_foreach(sub_graphs_, [&](auto& sub) {
         return sub.start(context);
      }));
      GRAPH_EXPECT_SUCC(tuple_foreach(roots_.roots_, [&](auto& root) {
         auto ports = std::make_unique<root_ports>();
         auto status = tuple_foreach(sub_graphs_, [&](auto& sub) {
            return sub.connect_root(context, root, *ports);
         });
         if(status != status_t::Ok) return status;
         return root.update_ports(context, std::move(ports));
      }));
      cleanup_sub_graphs(sequence);
      return status_t::Ok;
   }

   auto stop() {
      stop_roots(ROOTS::sequence);
      stop_sub_graphs(sequence);
   }

   template<size_t I>
   inline auto get_root() -> decltype(auto) {
      return (std::get<I>(roots_.roots_).get_handle());
   }

private:

   template<size_t ... I>
   auto cleanup_sub_graphs(std::index_sequence<I...>) {
      (std::get<I>(sub_graphs_).cleanup(),  ...);
   }

   using roots = typename ROOTS::type;

   template<size_t ... I>
   auto stop_sub_graphs(std::index_sequence<I...>) {
      (std::get<I>(sub_graphs_).stop(),  ...);
   }

   template<size_t ... I>
   auto stop_roots(std::index_sequence<I...>) {
      (std::get<I>(roots_.roots_).stop(),  ...);
   }

private:
   template<typename T>
   using selector = typename sub_graph_selector<T>::template instance_type<roots>;

private:
   ROOTS roots_;
   std::tuple<selector<SUB_GRAPH_SELECTOR>...> sub_graphs_;
};

GRAPH_DSL_NS_END

#define __g_GRAPH(...) GRAPH_DSL_NS::graph<__VA_ARGS__>

#endif //GRAPH_GRAPH_H
