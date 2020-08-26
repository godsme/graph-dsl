//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_DOWN_STREAM_FORK_H
#define GRAPH_DOWN_STREAM_FORK_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/graph_context.h>
#include <graph/util/result_t.h>
#include <graph/core/dsl/down-stream/down_stream_trait_decl.h>
#include <graph/function/tuple_foreach.h>
#include <holo/algo/flatten.h>

GRAPH_DSL_NS_BEGIN

template<typename ... NODEs_LIKE>
struct down_stream_fork {
   constexpr static auto node_list =
      holo::flatten(holo::make_tuple(down_stream_trait<NODEs_LIKE>::type::node_list...));

   template<typename TUPLE>
   struct instance_type {
      auto build(graph_context& context) -> status_t {
         return tuple_foreach(nodes_, [&](auto& node) { return node.build(context); });
      }
      auto release(graph_context& context) {
         tuple_foreach_void(nodes_, [&](auto& node) { node.release(context); });
      }
      auto enabled() const -> bool {
         return tuple_exists(nodes_, [](auto& node) { return node.enabled(); });
      }
      auto collect_actor_handle(graph_context& context, actor_handle_set& actor_handles) -> status_t {
         return tuple_foreach(nodes_, [&](auto& node) {
            if (node.enabled()) {
               return node.collect_actor_handle(context, actor_handles);
            }
            return status_t::Ok;
         });
      }

   private:
      std::tuple<typename down_stream_trait<NODEs_LIKE>::type::template instance_type<TUPLE>...> nodes_;
   };
};

template<typename ... NODEs_LIKE>
struct down_stream_trait<down_stream_fork<NODEs_LIKE...>, void> {
   using type = down_stream_fork<NODEs_LIKE...>;
};

GRAPH_DSL_NS_END

#define __g_FORK(...) std::decay_t<decltype(std::declval<GRAPH_DSL_NS::down_stream_fork<__VA_ARGS__>>())>

#endif //GRAPH_DOWN_STREAM_FORK_H
