//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_NODE_DESC_H
#define GRAPH_NODE_DESC_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/graph_context.h>
#include <graph/core/link_desc.h>
#include <tuple>
#include <boost/hana.hpp>
#include <graph/function/unique.h>
#include <graph/core/subgraph_node.h>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

template<typename NODE, typename ... LINKS>
struct node_desc final {
   using node_type = NODE;
   constexpr static auto direct_decedents =
      unique(hana::flatten(hana::make_tuple(link_desc<LINKS>::node_list...)));

   template<typename TUPLE>
   struct instance_type {
      auto build(graph_context& context) -> status_t {
         return build_links(context, std::make_index_sequence<sizeof...(LINKS)>{});
      }

   private:
      template<size_t ... I>
      auto build_links(graph_context& context, std::index_sequence<I...>) -> status_t {
         status_t status = status_t::Ok;
         (((status = std::get<I>(links_).build(context)) == status_t::Ok) && ...);
         return status;
      }

   private:
      std::tuple<typename link_desc<LINKS>::template instance_type<TUPLE> ...> links_;
   };
};

GRAPH_DSL_NS_END

#define __node(...) GRAPH_DSL_NS::node_desc<__VA_ARGS__>
#define __root(...) GRAPH_DSL_NS::node_desc<true,  __VA_ARGS__>

#endif //GRAPH_NODE_DESC_H
