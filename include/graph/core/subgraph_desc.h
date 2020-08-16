//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_SUBGRAPH_DESC_H
#define GRAPH_SUBGRAPH_DESC_H

#include <graph/graph_ns.h>
#include <graph/core/graph_trait.h>
#include <graph/util/hana_tuple_trait.h>
#include <boost/hana.hpp>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

template<typename ... NODES>
struct subgraph_desc {
   constexpr static auto all_sorted_nodes = graph_trait<NODES...>::all_sorted_nodes;

private:
   template<typename ... Ts>
   using cb_container = std::tuple<subgraph_node<typename Ts::node_type, Ts::is_leaf>...>;
   using nodes_cb = hana_tuple_trait_t<decltype(all_sorted_nodes), cb_container>;

   constexpr static auto sequence = std::make_index_sequence<sizeof...(NODES)>{};
   constexpr static auto node_cb_size = std::tuple_size_v<nodes_cb>;
   constexpr static auto node_cb_seq = std::make_index_sequence<std::tuple_size_v<nodes_cb>>{};

public:
   auto build(graph_context& context) -> status_t {
      context.switch_subgraph_context(nodes_cb_);
      return build(context, sequence);
   }

   auto start(graph_context& context) -> status_t {
      return start(context, node_cb_seq);
   }

   auto dump() {
      dump(node_cb_seq);
   }

private:
   template<size_t ... I>
   auto build(graph_context& context, std::index_sequence<I...>) {
      status_t status = status_t::Ok;
      return (((status = std::get<I>(nodes_desc_).build(context)) == status_t::Ok) && ...) ?
         status_t::Ok : status;
   }

   template<size_t ... I>
   auto start(graph_context& context, std::index_sequence<I...>) {
      status_t status = status_t::Ok;
      return (... && ((status = std::get<sizeof...(I) - 1 - I>(nodes_cb_).start(context, nodes_desc_)) == status_t::Ok)) ?
             status_t::Ok : status;
   }

   template<size_t... I>
   auto dump(std::index_sequence<I...>) {
      (std::get<I>(nodes_cb_).dump(), ...);
   }

private:
   constexpr static auto sorted_nodes_desc = graph_trait<NODES...>::sorted_nodes_desc;
   static_assert(hana::size(sorted_nodes_desc) == sizeof...(NODES));

   template<typename ... Ts>
   using desc_container  = std::tuple<typename Ts::template instance_type<nodes_cb>...>;
   using nodes_desc = hana_tuple_trait_t<decltype(sorted_nodes_desc), desc_container>;

private:
   nodes_cb   nodes_cb_;
   nodes_desc nodes_desc_;
};

GRAPH_DSL_NS_END

#define __sub_graph(...) GRAPH_DSL_NS::subgraph_desc<__VA_ARGS__>

#endif //GRAPH_SUBGRAPH_DESC_H
