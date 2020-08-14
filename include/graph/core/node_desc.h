//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_NODE_DESC_H
#define GRAPH_NODE_DESC_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/graph_context.h>
#include <tuple>
#include <vector>
#include <boost/hana.hpp>
#include <graph/function/unique.h>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

struct link;

template<bool ROOT, typename NODE, typename ... LINKS>
struct node_desc final {
   constexpr static auto direct_decedents =
      unique(hana::fold_left
         ( hana::make_tuple(LINKS::node_list...)
         , hana::concat));

//private:
//   using link_set = std::vector<link*>;
//   constexpr static auto Index_Seq = std::make_index_sequence<sizeof...(LINKS)>{};
//
//public:
//   using node_type = NODE;
//
//   auto build(graph_context& context) -> status_t {
//      if constexpr(sizeof...(LINKS) > 0) {
//         link_set alive_links(sizeof...(LINKS));
//         GRAPH_EXPECT_SUCC(build_links(context, alive_links, Index_Seq));
//         GRAPH_EXPECT_SUCC(node_.build(context, alive_links));
//      } else {
//         GRAPH_EXPECT_SUCC(node_.build(context));
//      }
//
//      return status_t::Ok;
//   }
//
//private:
//   template<size_t ... I>
//   auto build_links(graph_context& context, link_set& links, std::index_sequence<I...>) -> status_t {
//      status_t status = status_t::Ok;
//      (((status = std::get<I>(links_).build(context, links)) == status_t::Ok) && ...);
//      return status;
//   }
//
//private:
//   NODE node_;
//   std::tuple<LINKS...> links_;
};

GRAPH_DSL_NS_END

#define __node(...) GRAPH_DSL_NS::node_desc<false, __VA_ARGS__>
#define __root(...) GRAPH_DSL_NS::node_desc<true,  __VA_ARGS__>

#endif //GRAPH_NODE_DESC_H
