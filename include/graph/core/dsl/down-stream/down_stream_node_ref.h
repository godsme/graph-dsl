//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_DOWN_STREAM_NODE_REF_H
#define GRAPH_DOWN_STREAM_NODE_REF_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/graph_context.h>
#include <graph/core/node_index.h>
#include <graph/core/dsl/down-stream/down_stream_trait_decl.h>
#include <graph/core/actor_ports.h>
#include <boost/hana.hpp>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

struct node_signature {};

template <typename NODE>
struct down_stream_node_ref {
   constexpr static auto node_list = hana::tuple_t<NODE>;

   template<typename TUPLE>
   struct instance_type : node_index<NODE, TUPLE> {
      auto build(graph_context& context) -> status_t {
         if(!enabled_) {
            node_index<NODE, TUPLE>::get_node(context).add_ref();
            enabled_ = true;
         }

         return status_t::Ok;
      }

      auto release(graph_context& context) {
         if(enabled_) {
            node_index<NODE, TUPLE>::get_node(context).release();
            enabled_ = false;
         }
      }

      auto collect_actor_handle(graph_context& context, actor_handle_set& actor_handles) -> status_t {
         GRAPH_EXPECT_TRUE(enabled());
         auto handle = node_index<NODE, TUPLE>::get_node(context).actor_handle_;
         GRAPH_EXPECT_TRUE(handle.exists());
         actor_handles.emplace_back(std::move(handle));
         return status_t::Ok;
      }

      inline auto enabled() const -> bool { return enabled_; }

   private:
      bool enabled_{false};
   };
};

template<typename NODE>
struct down_stream_trait<NODE, std::enable_if_t<std::is_base_of_v<node_signature, NODE>>> {
   using type = down_stream_node_ref<NODE>;
};

GRAPH_DSL_NS_END

#endif //GRAPH_DOWN_STREAM_NODE_REF_H
