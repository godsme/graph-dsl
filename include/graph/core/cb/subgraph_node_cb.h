//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_SUBGRAPH_NODE_CB_H
#define GRAPH_SUBGRAPH_NODE_CB_H

#include <graph/graph_ns.h>
#include <nano-caf/core/actor/actor_handle.h>
#include <graph/util/tuple_element_by_type.h>
#include <graph/core/graph_context.h>
#include <graph/status.h>
#include <graph/core/node_category.h>
#include <cstdint>
#include <iostream>
#include <graph/core/msgs/graph_msgs.h>

GRAPH_DSL_NS_BEGIN

//////////////////////////////////////////////////////////////////////////////////////////
template<typename NODE>
struct subgraph_node_base {
   using node_type = NODE;

   inline auto add_ref() -> void { refs_++; }
   inline auto release() -> void { refs_--; }
   inline auto present() -> bool { return refs_ > 0; }

   auto stop() -> status_t {
      if(running_) {
         running_ = false;
         auto result = actor_handle_.exit_and_wait();
         GRAPH_EXPECT_TRUE(result.is_ok());
      }

      return status_t::Ok;
   }

   auto cleanup() -> status_t {
      if(refs_ == 0) { return stop(); }
      return status_t::Ok;
   }

   inline auto actor_handle() -> decltype(auto) {
      return (actor_handle_);
   }
protected:
   nano_caf::actor_handle actor_handle_{};
   uint8_t refs_{0};
   bool    running_{false};
};

//////////////////////////////////////////////////////////////////////////////////////////
template<typename NODE, node_category category>
struct subgraph_node_cb;

//////////////////////////////////////////////////////////////////////////////////////////
template<typename NODE>
struct subgraph_node_cb<NODE, node_category::Leaf> : subgraph_node_base<NODE> {
private:
   using self = subgraph_node_base<NODE>;

public:
   template<typename NODE_DESC_TUPLE>
   auto start(graph_context& context, NODE_DESC_TUPLE&) -> status_t {
      if(self::present() && !self::running_) {
         self::actor_handle_ = NODE::spawn(context);
         GRAPH_EXPECT_TRUE(self::actor_handle_.exists());
         self::running_ = true;
      }
      return status_t::Ok;
   }

};

//////////////////////////////////////////////////////////////////////////////////////////
template<typename NODE>
struct subgraph_node_cb<NODE, node_category::Intermediate> : subgraph_node_base<NODE> {
private:
   using self = subgraph_node_base<NODE>;

   template<typename T>
   struct desc_node_type {
      using type = typename T::node_type;
   };

public:
   template<typename NODE_DESC_TUPLE>
   auto start(graph_context& context, NODE_DESC_TUPLE& nodes_desc) -> status_t {
      constexpr static auto Index = tuple_element_index_v<NODE, NODE_DESC_TUPLE, desc_node_type>;
      static_assert(Index >= 0, "");

      if(!self::present()) return status_t::Ok;
      auto ports = std::make_unique<actor_ports>();
      GRAPH_EXPECT_SUCC(std::get<Index>(nodes_desc).collect_actor_ports(context, *ports));
      if(!self::running_) {
         self::actor_handle_ = NODE::spawn(context, std::move(ports));
         GRAPH_EXPECT_TRUE(self::actor_handle_.exists());
         self::running_ = true;
      } else {
         auto status = context.get_actor_context()
            .send<subgraph_ports_update_msg, nano_caf::message::urgent>(self::actor_handle_, std::move(ports));
         if(status != nano_caf::status_t::ok) {
            return status_t::Failed;
         }
      }

      return status_t::Ok;
   }

private:
   actor_handle_set set;
};

GRAPH_DSL_NS_END

#endif //GRAPH_SUBGRAPH_NODE_CB_H
