//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_DOWN_STREAM_FORK_H
#define GRAPH_DOWN_STREAM_FORK_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/util/result_t.h>
#include <graph/core/dsl/down-stream/down_stream_trait_decl.h>
#include <graph/function/tuple_foreach.h>
#include <holo/holo.h>

GRAPH_DSL_NS_BEGIN

struct GraphContext;

template<typename ... NODEs_LIKE>
struct DownStreamFork {
   constexpr static auto NODE_LIST =
        holo::make_list(DownStreamTrait<NODEs_LIKE>::Type::NODE_LIST...)
      | holo::flatten();

   template<typename TUPLE>
   struct InstanceType {
      auto Build(GraphContext& context) -> Status {
         return tuple_foreach(m_nodes, [&](auto& node) { return node.Build(context); });
      }

      auto Release(GraphContext& context) -> void {
         tuple_foreach_void(m_nodes, [&](auto& node) { node.Release(context); });
      }

      auto Enabled() const -> bool {
         return tuple_exists(m_nodes, [](auto& node) { return node.Enabled(); });
      }

      auto CollectActorHandle(GraphContext& context, ActorHandleSet& actor_handles) -> Status {
         return tuple_foreach(m_nodes, [&](auto& node) {
            if (node.Enabled()) {
               return node.CollectActorHandle(context, actor_handles);
            }
            return Status::OK;
         });
      }

   private:
      std::tuple<typename DownStreamTrait<NODEs_LIKE>::Type::template InstanceType<TUPLE>...> m_nodes;
   };
};

template<typename ... NODEs_LIKE>
struct DownStreamTrait<DownStreamFork<NODEs_LIKE...>, void> {
   using Type = DownStreamFork<NODEs_LIKE...>;
};

GRAPH_DSL_NS_END

#define __g_FORK(...) __MACO_template_type(GRAPH_DSL_NS::DownStreamFork<__VA_ARGS__>)

#endif //GRAPH_DOWN_STREAM_FORK_H
