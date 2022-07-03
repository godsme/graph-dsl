//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_ROOT_NODE_CB_H
#define GRAPH_ROOT_NODE_CB_H

#include <graph/graph_ns.h>
#include <nano-caf/actor/ActorHandle.h>
#include <graph/util/tuple_element_by_type.h>
#include <graph/core/GraphContext.h>
#include <graph/status.h>
#include <cstdint>
#include <iostream>
#include <graph/util/assertion.h>
#include <graph/core/ActorPorts.h>
#include <graph/core/msgs/graph_msgs.h>

GRAPH_DSL_NS_BEGIN

template<typename NODE>
struct RootNodeCb  {
   using NodeType = NODE;

   inline auto Present() const -> bool { return m_present; }

   auto Build(GraphContext& context) -> Status {
      auto device_info = context.GetRootState().GetDeviceInfo(NodeType::id);
      m_present = (device_info != nullptr);
      return Status::Ok;
   }

   auto Start(GraphContext& context, std::unique_ptr<ActorPorts> ports) -> Status {
      if(m_present && !m_running) {
          actorHandle = NODE::spawn(std::move(ports));
          GRAPH_EXPECT_TRUE(actorHandle);
          m_running = true;
      }

      return Status::Ok;
   }

   auto CleanUp() {
      if(!m_present) {
         Stop();
      }
   }

   auto Stop() {
      if(!m_running) return;
      //actorHandle.exit_and_release();
      m_running = false;
   }

   auto GetHandle() -> decltype(auto) {
      return (actorHandle);
   }

   auto UpdatePorts(GraphContext& context, std::unique_ptr<ActorPorts> ports) -> Status {
      GRAPH_EXPECT_TRUE(m_present);
      if(!m_running) {
         GRAPH_EXPECT_SUCC(Start(context, std::move(ports)));
      } else {
         GRAPH_EXPECT_SUCC(DoUpdatePorts(std::move(ports)));
      }

      return Status::Ok;
   }

private:
   auto DoUpdatePorts(std::unique_ptr<ActorPorts> ports) -> Status {
      GRAPH_EXPECT_TRUE(m_present && m_running);
      auto result = actorHandle.Send<PortsUpdateMsg, nano_caf::Message::URGENT>(std::move(ports));
      return result != nano_caf::Status::OK ? Status::Failed : Status::Ok;
   }

protected:
   nano_caf::ActorHandle actorHandle{};
   bool    m_running{false};
   bool    m_present{true};
};

template<typename ... Ts>
using RootNodes = std::tuple<RootNodeCb<Ts>...>;

GRAPH_DSL_NS_END

#endif //GRAPH_ROOT_NODE_CB_H
