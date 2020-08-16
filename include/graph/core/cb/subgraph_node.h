//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_SUBGRAPH_NODE_H
#define GRAPH_SUBGRAPH_NODE_H

#include <graph/graph_ns.h>
#include <nano-caf/core/actor/actor_handle.h>
#include <cstdint>
#include <iostream>
#include <graph/util/tuple_element_by_type.h>
#include <graph/core/graph_context.h>
#include <graph/status.h>

GRAPH_DSL_NS_BEGIN

template<typename NODE>
struct subgraph_node_base {
   using node_type = NODE;

   inline auto add_ref() -> void {
      refs_++;
   }
   inline auto release() -> void {
      refs_--;
   }
   inline auto enabled() -> bool {
      return refs_ > 0;
   }

public:

   auto dump() {
      std::cout << NODE::id << ": refs = " << (int)refs_ << std::endl;
   }

   nano_caf::actor_handle actor_handle_;

protected:
   uint8_t refs_{0};
   bool    running_{false};
};

template<typename NODE, bool IS_LEAF>
struct subgraph_node;

template<typename NODE>
struct subgraph_node<NODE, true> : subgraph_node_base<NODE> {
private:
   using parent = subgraph_node_base<NODE>;

public:
   template<typename NODE_DESC_TUPLE>
   auto start(graph_context& context, NODE_DESC_TUPLE& nodes_desc) -> status_t {
      if(parent::enabled() && !parent::running_) {
         parent::actor_handle_ = NODE::spawn(context);
         GRAPH_EXPECT_TRUE(parent::actor_handle_.exists());
         parent::running_ = true;
      }

      return status_t::Ok;
   }
};

template<typename NODE>
struct subgraph_node<NODE, false> : subgraph_node_base<NODE> {
private:
   using parent = subgraph_node_base<NODE>;

   template<typename T>
   struct desc_node_type {
      using type = typename T::node_type;
   };

public:
   template<typename NODE_DESC_TUPLE>
   auto start(graph_context& context, NODE_DESC_TUPLE& nodes_desc) -> status_t {
      if(!parent::enabled()) return status_t::Ok;
      constexpr auto Index = tuple_element_index_v<NODE, NODE_DESC_TUPLE, desc_node_type>;
      static_assert(Index >= 0, "");
      actor_ports ports;
      GRAPH_EXPECT_SUCC(std::get<Index>(nodes_desc).collect_actor_ports(context, ports));

      if(!parent::running_) {
         parent::actor_handle_ = NODE::spawn(context, ports);
         GRAPH_EXPECT_TRUE(parent::actor_handle_.exists());
         parent::running_ = true;
      } else {
         //parent::actor_handle_.request<>()
      }

      return status_t::Ok;
   }

private:
   actor_handle_set set;
};

GRAPH_DSL_NS_END

#endif //GRAPH_SUBGRAPH_NODE_H
