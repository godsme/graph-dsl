//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_ROOT_NODE_CB_H
#define GRAPH_ROOT_NODE_CB_H

#include <graph/graph_ns.h>
#include <nano-caf/core/actor/actor_handle.h>
#include <graph/util/tuple_element_by_type.h>
#include <graph/core/graph_context.h>
#include <graph/status.h>
#include <cstdint>
#include <iostream>
#include <graph/util/assertion.h>
#include <graph/core/actor_ports.h>
#include <graph/core/msgs/graph_msgs.h>

GRAPH_DSL_NS_BEGIN

template<typename NODE>
struct root_node_cb  {
   using node_type = NODE;

   nano_caf::actor_handle actor_handle_;

   inline auto present() const -> bool { return present_; }

   auto start(graph_context& context, std::unique_ptr<root_ports> ports) -> status_t {
      if(!running_) {
         actor_handle_ = NODE::spawn(context, ports);
         GRAPH_EXPECT_TRUE(actor_handle_.exists());
         running_ = true;
      }

      return status_t::Ok;
   }

   auto stop() {
      if(!running_) return;
      actor_handle_.exit_and_wait();
      running_ = false;
   }

   auto get_handle() -> decltype(auto) {
      return (actor_handle_);
   }

   auto update_ports(graph_context& context, std::unique_ptr<root_ports> ports) -> status_t {
      if(!running_) {
         actor_handle_ = NODE::spawn(context, std::move(ports));
         GRAPH_EXPECT_TRUE(actor_handle_.exists());
         running_ = true;
      } else {
         GRAPH_EXPECT_SUCC(update_ports_(context.get_actor_context(), std::move(ports)));
      }

      return status_t::Ok;
   }

private:
   auto update_ports_(nano_caf::actor_context& context, std::unique_ptr<root_ports> ports) -> status_t {
      GRAPH_EXPECT_TRUE(present_ && running_);
      auto result = context.send<root_ports_update_msg, nano_caf::message::urgent>(actor_handle_, std::move(ports));
      return result != nano_caf::status_t::ok ? status_t::Failed : status_t::Ok;
   }

protected:
   bool    running_{false};
   bool    present_{true};
};

template<typename ... Ts>
using root_nodes = std::tuple<root_node_cb<Ts>...>;

GRAPH_DSL_NS_END

#endif //GRAPH_ROOT_NODE_CB_H
