//
// Created by Darwin Yuan on 2020/8/18.
//

#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include <graph/graph_ns.h>
#include <graph/core/graph_analizer.h>
#include <graph/util/hana_tuple_trait.h>
#include <boost/hana.hpp>
#include <graph/core/dsl/sub_graph_selector.h>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

template<typename ROOTS, typename ... SUB_GRAPH_SELECTOR>
struct graph final {
private:
   constexpr static auto sequence = std::make_index_sequence<sizeof...(SUB_GRAPH_SELECTOR)>{};

public:
   inline auto build(graph_context& context) -> status_t { return build(context, sequence); }
   inline auto start(graph_context& context) -> status_t {
      return build(context, sequence);
   }

private:
   template<size_t ... I>
   auto build(graph_context& context, std::index_sequence<I...>) -> status_t {
      status_t status = status_t::Ok;
      return (((status = std::get<I>(sub_graphs_).build(context)) == status_t::Ok) && ...) ?
             status_t::Ok : status;
   }

private:
   template<typename T>
   using selector = typename sub_graph_selector<T>::template instance_type<typename ROOTS::type>;

private:
   ROOTS roots_;
   std::tuple<selector<SUB_GRAPH_SELECTOR>...> sub_graphs_;
};

GRAPH_DSL_NS_END

#define __g_GRAPH(...) GRAPH_DSL_NS::graph<__VA_ARGS__>

#endif //GRAPH_GRAPH_H
