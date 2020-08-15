//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_LINK_DESC_H
#define GRAPH_LINK_DESC_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/graph_context.h>
#include <graph/core/node_like_trait.h>
#include <graph/core/port_desc.h>

GRAPH_DSL_NS_BEGIN

template<typename T>
struct link_desc;

template<typename PORT, typename NODE_LIKE>
struct link_desc<auto (PORT) -> NODE_LIKE> {
   using node_like_type = typename node_like_trait<NODE_LIKE>::type;
   constexpr static auto node_list = node_like_type::node_list;

   struct link {
      auto build(graph_context& context) -> status_t {
         return down_stream_node_.build(context);
      }

   private:
      port_desc<PORT> port_;
      node_like_type  down_stream_node_;
   };
};

GRAPH_DSL_NS_END

#endif //GRAPH_LINK_DESC_H
