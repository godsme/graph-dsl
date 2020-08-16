//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_ROOT_NODE_H
#define GRAPH_ROOT_NODE_H

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

struct root_node {
   virtual auto present() const -> bool = 0;
   virtual auto connect(std::unique_ptr<root_actor_ports> handles) -> status_t = 0;
   virtual ~root_node() = default;
};

struct root_node_set {
   virtual auto get_root_node(size_t index) -> root_node* = 0;
   virtual ~root_node_set() = default;
};

struct graph_context;

template<typename NODE>
struct root_node_cb : root_node {
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

private:
   auto connect(std::unique_ptr<root_actor_ports> ports) -> status_t override {
      GRAPH_EXPECT_TRUE(present());
      auto result = actor_handle_.send<subgraph_connect_msg, nano_caf::message::urgent>(std::move(ports));
      if(result != nano_caf::status_t::ok) {
         return status_t::Failed;
      }
      return status_t ::Ok;
   }

protected:
   bool    running_{false};
};

template<typename ... Ts>
struct root_nodes : root_node_set {
   root_nodes() {
      push_to_container(std::make_index_sequence<sizeof...(Ts)>{});
   }

   auto get_root_node(size_t index) -> root_node* {
      if(index >= sizeof...(Ts)) {
         return nullptr;
      }

      return containers[index];
   }

   template <size_t I>
   auto get() -> decltype(auto) {
      static_assert(I <= sizeof...(Ts), "");
      return (std::get<I>(nodes));
   }

private:
   template <size_t ... I>
   auto push_to_container(std::index_sequence<I...>) {
      ((containers.push_back(static_cast<root_node*>(&std::get<I>(nodes)))), ...);
   }

private:
   std::vector<root_node*> containers;
   std::tuple<root_node_cb<Ts>...> nodes;
};

GRAPH_DSL_NS_END

#endif //GRAPH_ROOT_NODE_H
