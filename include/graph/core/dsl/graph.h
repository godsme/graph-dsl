//
// Created by Darwin Yuan on 2020/8/18.
//

#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include <graph/graph_ns.h>
#include <graph/core/dsl/sub_graph_selector.h>
#include <graph/util/assertion.h>

GRAPH_DSL_NS_BEGIN

template<typename ROOTS, typename ... SUB_GRAPH_SELECTOR>
struct graph final {
private:
   constexpr static auto sequence = std::make_index_sequence<sizeof...(SUB_GRAPH_SELECTOR)>{};

public:
   inline auto build(graph_context& context) -> status_t {
      context.update_root_nodes(roots_.roots_);
      return build(context, sequence);
   }
   inline auto start(graph_context& context) -> status_t {
      GRAPH_EXPECT_SUCC(start(context, sequence));
      return connect(context, ROOTS::sequence);
   }

private:
   template<size_t ... I>
   auto build(graph_context& context, std::index_sequence<I...>) -> status_t {
      status_t status = status_t::Ok;
      return (((status = std::get<I>(sub_graphs_).build(context)) == status_t::Ok) && ...) ?
             status_t::Ok : status;
   }

   template<size_t ... I>
   auto start(graph_context& context, std::index_sequence<I...>) -> status_t {
      status_t status = status_t::Ok;
      return (((status = std::get<I>(sub_graphs_).start(context)) == status_t::Ok) && ...) ?
             status_t::Ok : status;
   }

   using roots = typename ROOTS::type;
   template<size_t ... I>
   auto connect(graph_context& context, std::index_sequence<I...>) -> status_t {
      status_t status = status_t::Ok;
      return (((status = connect_by_root(context, std::get<I>(roots_.roots_), sequence)) == status_t::Ok) && ...) ?
             status_t::Ok : status;
   }

   template<size_t ... I, typename ROOT>
   auto connect_by_root(graph_context& context, ROOT& root, std::index_sequence<I...>) -> status_t {
      status_t status = status_t::Ok;
      auto ports = std::make_unique<root_ports>();
      auto result = (((status = std::get<I>(sub_graphs_).connect_root(context, root, *ports)) == status_t::Ok) && ...);
      if(result) {
         GRAPH_EXPECT_SUCC(root.update_ports(context, std::move(ports)));
         return status_t::Ok;
      }

      return status;
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
