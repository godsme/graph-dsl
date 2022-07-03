//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_SUBGRAPH_NODE_CB_H
#define GRAPH_SUBGRAPH_NODE_CB_H

#include <graph/graph_ns.h>
#include <nano-caf/actor/ActorHandle.h>
#include <graph/util/tuple_element_by_type.h>
#include <graph/core/GraphContext.h>
#include <graph/status.h>
#include <graph/core/NodeCategory.h>
#include <cstdint>
#include <graph/core/msgs/graph_msgs.h>

GRAPH_DSL_NS_BEGIN

//////////////////////////////////////////////////////////////////////////////////////////
template<typename NODE>
struct subgraph_node_base {
   using NodeType = NODE;

   inline auto AddRef() -> void { refs_++; }
   inline auto Release() -> void { refs_--; }
   inline auto Present() -> bool { return refs_ > 0; }

   auto Stop() -> Status {
      if(running_) {
         running_ = false;
         //actor_handle_.exit_and_release();
      }

      return Status::OK;
   }

   auto CleanUp() -> Status {
      if(refs_ == 0) {
         return Stop();
      }
      return Status::OK;
   }

   inline auto GetActorHandle() -> decltype(auto) {
      return (actor_handle_);
   }

protected:
   nano_caf::ActorHandle actor_handle_{};
   uint8_t refs_{0};
   bool    running_{false};
};

//////////////////////////////////////////////////////////////////////////////////////////
template<typename NODE, NodeCategory category>
struct subgraph_node_cb;

//////////////////////////////////////////////////////////////////////////////////////////
template<typename NODE>
struct subgraph_node_cb<NODE, NodeCategory::LEAF> : subgraph_node_base<NODE> {
private:
   using self = subgraph_node_base<NODE>;

public:
   template<typename NODE_DESC_TUPLE>
   auto Start(GraphContext& context, NODE_DESC_TUPLE&) -> Status {
      if(self::Present() && !self::running_) {
         self::actor_handle_ = NODE::spawn();
         GRAPH_EXPECT_TRUE(self::actor_handle_);
         self::running_ = true;
      }
      return Status::OK;
   }

};

//////////////////////////////////////////////////////////////////////////////////////////
template<typename NODE>
struct subgraph_node_cb<NODE, NodeCategory::INTERMEDIATE> : subgraph_node_base<NODE> {
private:
   using self = subgraph_node_base<NODE>;

   template<typename T>
   struct desc_node_type {
      using type = typename T::NodeType;
   };

public:
   template<typename NODE_DESC_TUPLE>
   auto Start(GraphContext& context, NODE_DESC_TUPLE& nodes_desc) -> Status {
      constexpr static auto Index = tuple_element_index_v<NODE, NODE_DESC_TUPLE, desc_node_type>;
      static_assert(Index >= 0, "");

      if(!self::Present()) return Status::OK;

      auto ports = std::make_unique<ActorPorts>();
      GRAPH_EXPECT_SUCC(std::get<Index>(nodes_desc).CollectActorPorts(context, *ports));
      if(!self::running_) {
         self::actor_handle_ = NODE::spawn(std::move(ports));
         GRAPH_EXPECT_TRUE(self::actor_handle_);
         self::running_ = true;
      } else {
         auto status = self::actor_handle_
            .template Send<PortsUpdateMsg, nano_caf::Message::URGENT>(std::move(ports));
         if(status != nano_caf::Status::OK) {
            return Status::FAILED;
         }
      }

      return Status::OK;
   }

private:
   ActorHandleSet set;
};

GRAPH_DSL_NS_END

#endif //GRAPH_SUBGRAPH_NODE_CB_H
