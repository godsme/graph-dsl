//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/graph_context.h>
#include <tuple>
#include <vector>

GRAPH_DSL_NS_BEGIN

struct link;

struct node {
   auto build(graph_context& context) -> status_t;
   auto build(graph_context& context, std::vector<link*>&) -> status_t;
};

template<bool ROOT, typename NODE, typename ... LINKS>
struct node_desc {
   using node_type = NODE;
   using link_set = std::vector<link*>;

   auto build(graph_context& context) -> status_t {
      if constexpr(sizeof...(LINKS) > 0) {
         link_set alive_links(sizeof...(LINKS));
         GRAPH_EXPECT_SUCC_CALL(build_links(context, alive_links, std::make_index_sequence<sizeof...(LINKS)>{}));
         GRAPH_EXPECT_SUCC_CALL(node.build(context, alive_links));
      } else {
         GRAPH_EXPECT_SUCC_CALL(node.build(context));
      }

      return status_t::ok;
   }

private:
   template<size_t ... I>
   auto build_links(graph_context& context, link_set& links, std::index_sequence<I...>) -> status_t {
      status_t status = status_t::ok;
      (((status = std::get<I>(links).build(context, links)) == status_t::ok) && ...);
      return status;
   }

private:
   NODE node;
   std::tuple<LINKS...> links;
};

GRAPH_DSL_NS_END

#define __node(...) GRAPH_DSL_NS::node_desc<false, __VA_ARGS__>
#define __root(...) GRAPH_DSL_NS::node_desc<true,  __VA_ARGS__>

#endif //GRAPH_NODE_H
