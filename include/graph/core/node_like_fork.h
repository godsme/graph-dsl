//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_NODE_LIKE_FORK_H
#define GRAPH_NODE_LIKE_FORK_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/graph_context.h>
#include <graph/util/result_t.h>
#include <graph/core/node_like_trait_decl.h>
#include <boost/hana/fwd/tuple.hpp>
#include <boost/hana/flatten.hpp>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

template<typename ... NODEs_LIKE>
struct node_like_fork {
   constexpr static auto node_list =
      hana::flatten(hana::make_tuple(node_like_trait<NODEs_LIKE>::type::node_list...));

   constexpr static auto sequence = std::make_index_sequence<sizeof...(NODEs_LIKE)>{};
   template<typename TUPLE>
   struct instance_type {
      auto build(graph_context& context) -> status_t {
         return build(context, sequence);
      }

      auto release(graph_context& context) {
         release(context, sequence);
      }

      auto enabled() const -> bool {
         return enabled(sequence);
      }
   private:
      template<size_t ... I>
      auto build(graph_context& context, std::index_sequence<I...>) -> status_t {
         status_t status = status_t::Ok;
         (((status = std::get<I>(nodes_).build(context)) == status_t::Ok) && ...);
         return status;
      }

      template<size_t ... I>
      auto release(graph_context& context, std::index_sequence<I...>) {
         (std::get<I>(nodes_).release(context), ...);
      }

      template<size_t ... I>
      auto enabled(std::index_sequence<I...>) {
         return (std::get<I>(nodes_).enabled() || ...);
      }

   private:
      std::tuple<typename node_like_trait<NODEs_LIKE>::type::template instance_type<TUPLE>...> nodes_;
   };
};

template<typename ... NODEs_LIKE>
struct node_like_trait<node_like_fork<NODEs_LIKE...>, void> {
   using type = node_like_fork<NODEs_LIKE...>;
};

GRAPH_DSL_NS_END

#define __fork(...) GRAPH_DSL_NS::node_like_fork<__VA_ARGS__>

#endif //GRAPH_NODE_LIKE_FORK_H
