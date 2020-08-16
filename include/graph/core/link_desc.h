//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_LINK_DESC_H
#define GRAPH_LINK_DESC_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/graph_context.h>
#include <graph/core/down-stream/node_like_trait.h>
#include <graph/core/actor_ports.h>

GRAPH_DSL_NS_BEGIN

template<typename T>
struct link_desc;

template<typename PORT, typename NODE_LIKE>
struct link_desc<auto (PORT) -> NODE_LIKE> {
   using node_like_type = typename node_like_trait<NODE_LIKE>::type;
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

   private:
      typename node_like_type::template instance_type<TUPLE>  down_stream_node_;
   };
};

GRAPH_DSL_NS_END

#define __port(name) auto (name)

#endif //GRAPH_LINK_DESC_H
