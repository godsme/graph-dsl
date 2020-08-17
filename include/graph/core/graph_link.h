//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_GRAPH_LINK_H
#define GRAPH_GRAPH_LINK_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/graph_context.h>
#include <graph/core/down-stream/down_stream_trait.h>
#include <graph/core/actor_ports.h>

GRAPH_DSL_NS_BEGIN

template<typename T>
struct graph_link;

template<typename PORT, typename NODE_LIKE>
struct graph_link<auto (PORT) -> NODE_LIKE> {
   using node_like_type = typename down_stream_trait<NODE_LIKE>::type;
   constexpr static auto node_list = node_like_type::node_list;

   template<typename TUPLE>
   struct instance_type {
      auto build(graph_context& context) -> status_t {
         return down_stream_node_.build(context);
      }

      auto collect_actor_port(graph_context& context, actor_ports& ports) -> status_t {
         if(down_stream_node_.enabled()) {
            actor_handle_set handles;
            GRAPH_EXPECT_SUCC(down_stream_node_.collect_actor_handle(context, handles));
            ports.push_back({PORT::get_port_format(context), handles});
         }
         return status_t::Ok;
      }

      auto collect_actor_port(graph_context& context, root_actor_ports& ports) -> status_t {
         if(down_stream_node_.enabled()) {
            actor_handle_set handles;
            GRAPH_EXPECT_SUCC(down_stream_node_.collect_actor_handle(context, handles));
            ports.push_back({PORT::root_port_id, handles});
         }
         return status_t::Ok;
      }

   private:
      typename node_like_type::template instance_type<TUPLE>  down_stream_node_;
   };
};

GRAPH_DSL_NS_END

#define __g_PORT(name) auto (name)

#endif //GRAPH_GRAPH_LINK_H
