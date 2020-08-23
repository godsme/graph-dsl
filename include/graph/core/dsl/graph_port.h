//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_GRAPH_PORT_H
#define GRAPH_GRAPH_PORT_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/graph_context.h>
#include <graph/core/dsl/down-stream/down_stream_trait.h>
#include <graph/core/actor_ports.h>

GRAPH_DSL_NS_BEGIN

template<typename T>
struct graph_port;

template<typename PORT, typename NODE_LIKE>
struct graph_port<auto (PORT) -> NODE_LIKE> final {
   using node_like_type = typename down_stream_trait<NODE_LIKE>::type;
   constexpr static auto node_list = node_like_type::node_list;

   template<typename TUPLE>
   struct instance_type {
      inline auto build(graph_context& context) -> status_t {
         return down_stream_node_.build(context);
      }

      inline auto release(graph_context& context) {
         down_stream_node_.release(context);
      }

      template<bool Is_Root>
      auto collect_actor_port(graph_context& context, actor_ports& ports) -> status_t {
         if(down_stream_node_.enabled()) {
            GRAPH_EXPECT_SUCC(down_stream_node_.collect_actor_handle(context, ports[PORT::port_id].actor_handles_));
            if constexpr (!Is_Root) {
               ports[PORT::port_id].format_ = PORT::get_port_format(context);
            }
         }
         return status_t::Ok;
      }

      inline auto enabled() -> bool { return down_stream_node_.enabled(); }

   private:
      typename node_like_type::template instance_type<TUPLE>  down_stream_node_;
   };
};

GRAPH_DSL_NS_END

#endif //GRAPH_GRAPH_PORT_H
