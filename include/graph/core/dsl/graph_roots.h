//
// Created by Darwin Yuan on 2020/8/18.
//

#ifndef GRAPH_GRAPH_ROOTS_H
#define GRAPH_GRAPH_ROOTS_H

#include <graph/graph_ns.h>
#include <graph/core/graph_analizer.h>
#include <graph/util/hana_tuple_trait.h>
#include <boost/hana.hpp>

GRAPH_DSL_NS_BEGIN

template<typename ... NODES>
struct graph_roots final {
   using type = root_nodes<NODES...>;

private:
   constexpr static auto sequence = std::make_index_sequence<sizeof...(NODES)>{};

public:
   auto start(graph_context& context) -> status_t {
      return start(context, sequence);
   }

private:
   template<size_t ... I>
   auto start(graph_context& context, std::index_sequence<I...>) -> status_t {
      status_t status = status_t::Ok;
      return (((status = std::get<I>(roots_).build(context)) == status_t::Ok) && ...) ?
             status_t::Ok : status;
   }

private:
   type roots_;
};

GRAPH_DSL_NS_END

#define __g_ROOTS(...) GRAPH_DSL_NS::graph_roots<__VA_ARGS__>

#endif //GRAPH_GRAPH_ROOTS_H
