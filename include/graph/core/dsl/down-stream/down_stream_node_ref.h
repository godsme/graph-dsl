//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_DOWN_STREAM_NODE_REF_H
#define GRAPH_DOWN_STREAM_NODE_REF_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/GraphContext.h>
#include <graph/core/NodeIndex.h>
#include <graph/core/dsl/down-stream/down_stream_trait_decl.h>
#include <graph/core/ActorPorts.h>
#include <holo/holo.h>

GRAPH_DSL_NS_BEGIN

struct node_signature {};

template <typename NODE>
struct down_stream_node_ref {
   constexpr static auto node_list = holo::list_t<NODE>;

   template<typename TUPLE>
   struct instance_type : NodeIndex<NODE, TUPLE> {
      auto Build(GraphContext& context) -> Status {
         if(!enabled_) {
            NodeIndex<NODE, TUPLE>::GetNode(context).AddRef();
            enabled_ = true;
         }
         return Status::Ok;
      }

      auto Release(GraphContext& context) {
         if(enabled_) {
            NodeIndex<NODE, TUPLE>::GetNode(context).Release();
            enabled_ = false;
         }
      }

      auto CollectActorHandle(GraphContext& context, ActorHandleSet& actor_handles) -> Status {
         GRAPH_EXPECT_TRUE(Enabled());
         auto&& handle = NodeIndex<NODE, TUPLE>::GetNode(context).GetActorHandle();
         GRAPH_EXPECT_TRUE(handle);
         actor_handles.emplace_back(std::move(handle));
         return Status::Ok;
      }

      inline auto Enabled() const -> bool { return enabled_; }

   private:
      bool enabled_{false};
   };
};

template<typename NODE>
struct down_stream_trait<NODE, std::enable_if_t<std::is_base_of_v<node_signature, NODE>>> {
   using type = down_stream_node_ref<NODE>;
};

GRAPH_DSL_NS_END

#endif //GRAPH_DOWN_STREAM_NODE_REF_H
