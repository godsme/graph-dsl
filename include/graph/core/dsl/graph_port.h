//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_GRAPH_PORT_H
#define GRAPH_GRAPH_PORT_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/GraphContext.h>
#include <graph/core/dsl/down-stream/down_stream_trait.h>
#include <graph/core/ActorPorts.h>

GRAPH_DSL_NS_BEGIN

template<typename T>
struct graph_port;

template<typename PORT, typename NODE_LIKE>
struct graph_port<auto (PORT) -> NODE_LIKE> final {
   using NodeLikeType = typename DownStreamTrait<NODE_LIKE>::Type;
   constexpr static auto NODE_LIST = NodeLikeType::NODE_LIST;

   template<typename TUPLE>
   struct InstanceType {
      inline auto Build(GraphContext& context) -> Status {
         return m_downStreamNode.Build(context);
      }

      inline auto Release(GraphContext& context) {
         m_downStreamNode.Release(context);
      }

      template<bool Is_Root>
      auto CollectActorPort(GraphContext& context, ActorPorts& ports) -> Status {
         if(m_downStreamNode.Enabled()) {
            if constexpr (Is_Root) {
               if(ports.find(PORT::port_id) == ports.end()) UpdatePortFormat(context, ports);
            } else {
                UpdatePortFormat(context, ports);
            }
            GRAPH_EXPECT_SUCC(m_downStreamNode.CollectActorHandle(context, ports[PORT::port_id].m_actorHandles));
         }
         return Status::OK;
      }

      inline auto Enabled() -> bool { return m_downStreamNode.Enabled(); }

   private:
      auto UpdatePortFormat(GraphContext& context, ActorPorts& ports) {
         ports[PORT::port_id].m_format = PORT::GetPortFormat(context);
      }
   private:
      typename NodeLikeType::template InstanceType<TUPLE>  m_downStreamNode;
   };
};

GRAPH_DSL_NS_END

#endif //GRAPH_GRAPH_PORT_H
