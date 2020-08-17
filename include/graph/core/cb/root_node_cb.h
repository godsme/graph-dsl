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

struct root_node_cb {
   virtual auto present() const -> bool = 0;
   virtual auto connect(std::unique_ptr<root_actor_ports> handles) -> status_t = 0;
   virtual auto disconnect(std::unique_ptr<root_actor_ports> handles) -> status_t = 0;
   virtual ~root_node_cb() = default;
};

struct graph_context;

template<typename NODE>
struct generic_root_node_cb : root_node_cb {
   using node_type = NODE;

   nano_caf::actor_handle actor_handle_;

   auto present() const -> bool override {
      return running_;
   }

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

private:
   auto connect(std::unique_ptr<root_actor_ports> ports) -> status_t override {
      GRAPH_EXPECT_TRUE(present());
      auto result = actor_handle_.send<subgraph_connect_msg, nano_caf::message::urgent>(std::move(ports));
      return result != nano_caf::status_t::ok ? status_t::Failed : status_t::Ok;
   }

   auto disconnect(std::unique_ptr<root_actor_ports> ports) -> status_t override {
      auto result = actor_handle_.send<subgraph_disconnect_msg, nano_caf::message::urgent>(std::move(ports));
      return result != nano_caf::status_t::ok ? status_t::Failed : status_t::Ok;
   }

protected:
   bool    running_{false};
};

GRAPH_DSL_NS_END

#endif //GRAPH_ROOT_NODE_CB_H
