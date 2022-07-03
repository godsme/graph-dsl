//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_DOWN_STREAM_FORK_H
#define GRAPH_DOWN_STREAM_FORK_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/GraphContext.h>
#include <graph/util/result_t.h>
#include <graph/core/dsl/down-stream/down_stream_trait_decl.h>
#include <graph/function/tuple_foreach.h>
#include <holo/holo.h>

GRAPH_DSL_NS_BEGIN

template<typename ... NODEs_LIKE>
struct down_stream_fork {
   constexpr static auto node_list =
        holo::make_list(down_stream_trait<NODEs_LIKE>::type::node_list...)
      | holo::flatten();

   template<typename TUPLE>
   struct instance_type {
      auto Build(GraphContext& context) -> Status {
         return tuple_foreach(nodes_, [&](auto& node) { return node.Build(context); });
      }
      auto Release(GraphContext& context) {
         tuple_foreach_void(nodes_, [&](auto& node) { node.Release(context); });
      }
      auto Enabled() const -> bool {
         return tuple_exists(nodes_, [](auto& node) { return node.Enabled(); });
      }
      auto CollectActorHandle(GraphContext& context, ActorHandleSet& actor_handles) -> Status {
         return tuple_foreach(nodes_, [&](auto& node) {
            if (node.Enabled()) {
               return node.CollectActorHandle(context, actor_handles);
            }
            return Status::Ok;
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

#define __g_FORK(...) __MACO_template_type(GRAPH_DSL_NS::down_stream_fork<__VA_ARGS__>)

#endif //GRAPH_DOWN_STREAM_FORK_H
