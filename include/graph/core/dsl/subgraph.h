//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_SUBGRAPH_H
#define GRAPH_SUBGRAPH_H

#include <graph/graph_ns.h>
#include <graph/core/graph_analizer.h>
#include <graph/util/hana_tuple_trait.h>
#include <boost/hana.hpp>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

template<typename ... NODES>
struct subgraph final {
   constexpr static auto all_sorted_nodes = graph_analizer<NODES...>::all_sorted_nodes;

private:
   template<typename ... Ts>
   using cb_container = std::tuple<subgraph_node_cb<typename Ts::node_type, Ts::category>...>;
   using nodes_cb = hana_tuple_trait_t<decltype(all_sorted_nodes), cb_container>;

   constexpr static auto sequence = std::make_index_sequence<sizeof...(NODES)>{};
   constexpr static auto node_cb_seq = std::make_index_sequence<std::tuple_size_v<nodes_cb>>{};

public:
   auto build(graph_context& context) -> status_t {
      context.switch_subgraph_context(nodes_cb_);
      return build(context, sequence);
   }

   auto start(graph_context& context) -> status_t {
      return start(context, node_cb_seq);
   }

private:
   template<size_t ... I>
   auto build(graph_context& context, std::index_sequence<I...>) {
      status_t status = status_t::Ok;
      return (((status = std::get<I>(nodes_links_).build(context)) == status_t::Ok) && ...) ?
         status_t::Ok : status;
   }

   template<size_t ... I>
   auto start(graph_context& context, std::index_sequence<I...>) {
      status_t status = status_t::Ok;
      return (... && ((status = std::get<sizeof...(I) - 1 - I>(nodes_cb_).start(context, nodes_links_)) == status_t::Ok)) ?
             status_t::Ok : status;
   }

private:
   constexpr static auto sorted_nodes_desc = graph_analizer<NODES...>::sorted_nodes_desc;
   static_assert(hana::size(sorted_nodes_desc) == sizeof...(NODES));

   template<typename ... Ts>
   using desc_container  = std::tuple<typename Ts::template instance_type<nodes_cb>...>;
   using nodes_links = hana_tuple_trait_t<decltype(sorted_nodes_desc), desc_container>;

private:
   nodes_cb    nodes_cb_;
   nodes_links nodes_links_;
};

GRAPH_DSL_NS_END

#define __sub_graph(...) GRAPH_DSL_NS::subgraph<__VA_ARGS__>

#endif //GRAPH_SUBGRAPH_H
