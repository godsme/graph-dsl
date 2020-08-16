//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_SUBGRAPH_NODE_H
#define GRAPH_SUBGRAPH_NODE_H

#include <graph/graph_ns.h>
#include <nano-caf/core/actor/actor_handle.h>
#include <graph/util/tuple_element_by_type.h>
#include <graph/core/graph_context.h>
#include <graph/status.h>
#include <graph/core/node_category.h>
#include <cstdint>
#include <iostream>

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

   nano_caf::actor_handle actor_handle_{};

protected:
   uint8_t refs_{0};
   bool    running_{false};
};

template<typename NODE, node_category category>
struct subgraph_node;

template<typename NODE>
struct subgraph_node<NODE, node_category::Root> {
   using node_type = NODE;

private:
   template<typename T>
   struct desc_node_type {
      using type = typename T::node_type;
   };

   template<typename NODE_DESC_TUPLE, typename F>
   auto op(graph_context& context, NODE_DESC_TUPLE& nodes_desc, F&& f) -> status_t {
      constexpr auto Index = tuple_element_index_v<NODE, NODE_DESC_TUPLE, desc_node_type>;
      static_assert(Index >= 0, "");

      auto root_node = context.get_root_node(NODE::root_id);
      GRAPH_EXPECT_TRUE(root_node != nullptr);

      if(!root_node->present()) {
         return status_t::Ok;
      }

      return f(root_node, std::get<Index>(nodes_desc));
   }

public:
   template<typename NODE_DESC_TUPLE>
   auto start(graph_context& context, NODE_DESC_TUPLE& nodes_desc) -> status_t {
      return op<NODE_DESC_TUPLE>(context, nodes_desc,
         [&](auto root_node, auto& node_desc) {
            auto ports = std::make_unique<root_actor_ports>();
            GRAPH_EXPECT_SUCC(node_desc.collect_actor_ports(context, *ports));

            GRAPH_EXPECT_SUCC(root_node->connect(std::move(ports)));

            return status_t::Ok;
      });
   }

   template<typename NODE_DESC_TUPLE>
   auto cleanup(graph_context& context, NODE_DESC_TUPLE& nodes_desc) -> status_t {
      return op<NODE_DESC_TUPLE>(context, nodes_desc,
         [&](auto root_node, auto& node_desc) {
            auto ports = std::make_unique<root_actor_ports>();
            GRAPH_EXPECT_SUCC(node_desc.collect_actor_ports(context, *ports));

            GRAPH_EXPECT_SUCC(root_node->disconnect(std::move(ports)));

            return status_t::Ok;
         });
   }

   auto dump() {
   }
};

template<typename NODE>
struct subgraph_node<NODE, node_category::Leaf> : subgraph_node_base<NODE> {
private:
   using parent = subgraph_node_base<NODE>;

public:
   template<typename NODE_DESC_TUPLE>
   auto start(graph_context& context, NODE_DESC_TUPLE&) -> status_t {
      if(parent::enabled() && !parent::running_) {
         parent::actor_handle_ = NODE::spawn(context);
         GRAPH_EXPECT_TRUE(parent::actor_handle_.exists());
         parent::running_ = true;
      }

      return status_t::Ok;
   }
};

template<typename NODE>
struct subgraph_node<NODE, node_category::Intermediate> : subgraph_node_base<NODE> {
private:
   using parent = subgraph_node_base<NODE>;

   template<typename T>
   struct desc_node_type {
      using type = typename T::node_type;
   };

public:
   template<typename NODE_DESC_TUPLE>
   auto start(graph_context& context, NODE_DESC_TUPLE& nodes_desc) -> status_t {
      constexpr auto Index = tuple_element_index_v<NODE, NODE_DESC_TUPLE, desc_node_type>;
      static_assert(Index >= 0, "");

      if(!parent::enabled()) return status_t::Ok;
      auto ports = std::make_unique<actor_ports>();
      GRAPH_EXPECT_SUCC(std::get<Index>(nodes_desc).collect_actor_ports(context, *ports));
      if(!parent::running_) {
         parent::actor_handle_ = NODE::spawn(context, std::move(ports));
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
