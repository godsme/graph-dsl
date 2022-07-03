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

struct NodeSignature {};

template <typename NODE>
struct DownStreamNodeRef {
   constexpr static auto NODE_LIST = holo::list_t<NODE>;

   template<typename TUPLE>
   struct InstanceType : NodeIndex<NODE, TUPLE> {
      auto Build(GraphContext& context) -> Status {
         if(!m_enabled) {
            NodeIndex<NODE, TUPLE>::GetNode(context).AddRef();
             m_enabled = true;
         }
         return Status::OK;
      }

      auto Release(GraphContext& context) {
         if(m_enabled) {
            NodeIndex<NODE, TUPLE>::GetNode(context).Release();
             m_enabled = false;
         }
      }

      auto CollectActorHandle(GraphContext& context, ActorHandleSet& actors) -> Status {
         GRAPH_EXPECT_TRUE(Enabled());
         auto&& actor = NodeIndex<NODE, TUPLE>::GetNode(context).GetActorHandle();
         GRAPH_EXPECT_TRUE(actor);
         actors.emplace_back(std::move(actor));
         return Status::OK;
      }

      inline auto Enabled() const -> bool { return m_enabled; }

   private:
      bool m_enabled{false};
   };
};

template<typename NODE>
struct DownStreamTrait<NODE, std::enable_if_t<std::is_base_of_v<NodeSignature, NODE>>> {
   using Type = DownStreamNodeRef<NODE>;
};

GRAPH_DSL_NS_END

#endif //GRAPH_DOWN_STREAM_NODE_REF_H
