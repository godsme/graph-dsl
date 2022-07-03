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
   using node_like_type = typename down_stream_trait<NODE_LIKE>::type;
   constexpr static auto node_list = node_like_type::node_list;

   template<typename TUPLE>
   struct instance_type {
      inline auto Build(GraphContext& context) -> Status {
         return down_stream_node_.Build(context);
      }

      inline auto Release(GraphContext& context) {
         down_stream_node_.Release(context);
      }

      template<bool Is_Root>
      auto CollectActorPort(GraphContext& context, ActorPorts& ports) -> Status {
         if(down_stream_node_.Enabled()) {
            if constexpr (Is_Root) {
               if(ports.find(PORT::port_id) == ports.end()) UpdatePortFormat(context, ports);
            } else {
                UpdatePortFormat(context, ports);
            }
            GRAPH_EXPECT_SUCC(down_stream_node_.CollectActorHandle(context, ports[PORT::port_id].m_actorHandles));
         }
         return Status::Ok;
      }

      inline auto Enabled() -> bool { return down_stream_node_.Enabled(); }

   private:
      auto UpdatePortFormat(GraphContext& context, ActorPorts& ports) {
         ports[PORT::port_id].m_format = PORT::GetPortFormat(context);
      }
   private:
      typename node_like_type::template instance_type<TUPLE>  down_stream_node_;
   };
};

GRAPH_DSL_NS_END

#endif //GRAPH_GRAPH_PORT_H
