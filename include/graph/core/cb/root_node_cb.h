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

struct graph_context;

template<typename NODE>
struct root_node_cb  {
   using node_type = NODE;

   nano_caf::actor_handle actor_handle_;

   inline auto present() const -> bool { return running_; }

   auto start(graph_context& context) -> status_t {
      if(!running_) {
         actor_handle_ = NODE::spawn(context);
         GRAPH_EXPECT_TRUE(actor_handle_.exists());
         running_ = true;
      }

      return status_t::Ok;
   }

   auto stop() {
      if(running_) {
         actor_handle_.exit(nano_caf::exit_reason::normal);
         running_ = false;
         actor_handle_.release();
      }
   }

   auto wait_for_exit() {
      if(running_) {
         actor_handle_.wait_for_exit();
      }
   }

   auto get_handle() {
      return actor_handle_;
   }

   auto connect(nano_caf::actor_context& context, std::unique_ptr<root_actor_ports> ports) -> status_t {
      GRAPH_EXPECT_TRUE(present());
      auto result = context.send<subgraph_connect_msg, nano_caf::message::urgent>(actor_handle_, std::move(ports));
      return result != nano_caf::status_t::ok ? status_t::Failed : status_t::Ok;
   }

   auto disconnect(nano_caf::actor_context& context, std::unique_ptr<root_actor_ports> ports) -> status_t {
      GRAPH_EXPECT_TRUE(present());
      auto result = context.send<subgraph_disconnect_msg, nano_caf::message::urgent>(actor_handle_, std::move(ports));
      return result != nano_caf::status_t::ok ? status_t::Failed : status_t::Ok;
   }

protected:
   bool    running_{false};
};


template<typename ... Ts>
using root_nodes = std::tuple<root_node_cb<Ts>...>;

GRAPH_DSL_NS_END

#endif //GRAPH_ROOT_NODE_CB_H
