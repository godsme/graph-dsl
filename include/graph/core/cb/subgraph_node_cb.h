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

GRAPH_DSL_NS_BEGIN

//////////////////////////////////////////////////////////////////////////////////////////
template<typename NODE>
struct subgraph_node_base {
   using node_type = NODE;

   inline auto add_ref() -> void { refs_++; }
   inline auto release() -> void { refs_--; }
   inline auto present() -> bool { return refs_ > 0; }

   template<typename NODE_DESC_TUPLE>
   auto stop(graph_context& context, NODE_DESC_TUPLE&) -> status_t {
      if(running_) {
         actor_handle_.send<nano_caf::exit_msg, nano_caf::message::urgent>(nano_caf::exit_reason::normal);
         actor_handle_.wait_for_exit();
         actor_handle_.release();
         running_ = false;
      }

      return status_t::Ok;
   }

   template<typename NODE_DESC_TUPLE>
   auto cleanup(graph_context& context, NODE_DESC_TUPLE& desc) -> status_t {
      if(!present()) {
         return stop(context, desc);
      }

      return status_t::Ok;
   }

public:
   nano_caf::actor_handle actor_handle_{};

protected:
   uint8_t refs_{0};
   bool    running_{false};
};

//////////////////////////////////////////////////////////////////////////////////////////
template<typename ROOTS, typename NODE, node_category category>
struct subgraph_node_cb;

//////////////////////////////////////////////////////////////////////////////////////////
template<typename ROOTS, typename NODE>
struct subgraph_node_cb<ROOTS, NODE, node_category::Root> {
   using node_type = NODE;

private:
   template<typename T> struct desc_node_type { using type = typename T::node_type; };
   constexpr static auto Root_Node_Index = tuple_element_index_v<NODE, ROOTS, desc_node_type>;
   static_assert(Root_Node_Index >= 0, "it's not a root node");

   template<typename NODE_DESC_TUPLE, typename F>
   auto op(graph_context& context, NODE_DESC_TUPLE& nodes_desc, F&& f) -> status_t {
      constexpr auto Index = tuple_element_index_v<NODE, NODE_DESC_TUPLE, desc_node_type>;
      static_assert(Index >= 0, "");

      auto& root_node = context.get_root_node<ROOTS, Root_Node_Index>();
      if(!root_node.present()) {
         connected = false;
         return status_t::Ok;
      }

      return f(root_node, std::get<Index>(nodes_desc));
   }

   template<typename ROOT_NODE, typename NODE_DESC>
   auto stop_(graph_context& context, ROOT_NODE& root_node, NODE_DESC& desc) {
      if(connected) {
         auto ports = std::make_unique<root_actor_ports>();
         GRAPH_EXPECT_SUCC(desc.collect_actor_ports(context, *ports));
         GRAPH_EXPECT_SUCC(root_node.disconnect(context.get_actor_context(), std::move(ports)));
         connected = false;
      }

      return status_t::Ok;
   }

public:
   template<typename NODE_DESC_TUPLE>
   auto start(graph_context& context, NODE_DESC_TUPLE& nodes_desc) -> status_t {
      return op<NODE_DESC_TUPLE>(context, nodes_desc, [&](auto& root_node, auto& node_desc) {
         if(!enabled) { return status_t::Ok; }

         if(!connected) {
            auto ports = std::make_unique<root_actor_ports>();
            GRAPH_EXPECT_SUCC(node_desc.collect_actor_ports(context, *ports));
            GRAPH_EXPECT_SUCC(root_node.connect(context.get_actor_context(), std::move(ports)));
            connected = true;
         } else {
            // TODO: compare then update if changed
         }

         return status_t::Ok;
      });
   }

   template<typename NODE_DESC_TUPLE>
   auto cleanup(graph_context& context, NODE_DESC_TUPLE& nodes_desc) -> status_t {
      return op<NODE_DESC_TUPLE>(context, nodes_desc, [&](auto& root_node, auto& node_desc) {
         return enabled ? status_t::Ok : stop_(context, root_node, node_desc);
      });
   }

   template<typename NODE_DESC_TUPLE>
   auto stop(graph_context& context, NODE_DESC_TUPLE& nodes_desc) -> status_t {
      return op<NODE_DESC_TUPLE>(context, nodes_desc, [&](auto& root_node, auto& node_desc) {
         return stop_(context, root_node, node_desc);
      });
   }

   inline auto enable() {
      enabled = true;
   }

   inline auto disable() {
      enabled = false;
   }

private:
   bool enabled{false};
   bool connected{false};
};

//////////////////////////////////////////////////////////////////////////////////////////
template<typename ROOTS, typename NODE>
struct subgraph_node_cb<ROOTS, NODE, node_category::Leaf> : subgraph_node_base<NODE> {
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
template<typename ROOTS, typename NODE>
struct subgraph_node_cb<ROOTS, NODE, node_category::Intermediate> : subgraph_node_base<NODE> {
private:
   using self = subgraph_node_base<NODE>;

   template<typename T>
   struct desc_node_type {
      using type = typename T::node_type;
   };

public:
   template<typename NODE_DESC_TUPLE>
   auto start(graph_context& context, NODE_DESC_TUPLE& nodes_desc) -> status_t {
      constexpr auto Index = tuple_element_index_v<NODE, NODE_DESC_TUPLE, desc_node_type>;
      static_assert(Index >= 0, "");

      if(!self::present()) return status_t::Ok;
      auto ports = std::make_unique<actor_ports>();
      GRAPH_EXPECT_SUCC(std::get<Index>(nodes_desc).collect_actor_ports(context, *ports));
      if(!self::running_) {
         self::actor_handle_ = NODE::spawn(context, std::move(ports));
         GRAPH_EXPECT_TRUE(self::actor_handle_.exists());
         self::running_ = true;
      } else {
         // TODO: compare then update if changed
         //parent::actor_handle_.request<>()
      }

      return status_t::Ok;
   }

private:
   actor_handle_set set;
};

GRAPH_DSL_NS_END

#endif //GRAPH_SUBGRAPH_NODE_CB_H
