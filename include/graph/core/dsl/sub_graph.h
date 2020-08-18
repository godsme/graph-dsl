//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_SUB_GRAPH_H
#define GRAPH_SUB_GRAPH_H

#include <graph/graph_ns.h>
#include <graph/core/sub_graph_analizer.h>
#include <graph/util/hana_tuple_trait.h>
#include <boost/hana.hpp>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

template<typename ... NODES>
struct sub_graph final {
   constexpr static auto all_sorted_nodes = sub_graph_analizer<NODES...>::all_sorted_sub_graph_nodes;

   template<typename ROOTS_CB>
   struct by_roots {
   private:
      template<typename ... Ts>
      using cb_container = std::tuple<subgraph_node_cb<typename Ts::node_type, Ts::category>...>;
      using nodes_cb = hana_tuple_trait_t<decltype(all_sorted_nodes), cb_container>;

      constexpr static auto sequence = std::make_index_sequence<sizeof...(NODES)>{};
      constexpr static auto node_cb_seq = std::make_index_sequence<std::tuple_size_v<nodes_cb>>{};

   private:
      constexpr static auto sorted_nodes_desc = sub_graph_analizer<NODES...>::sorted_nodes_desc;
      static_assert(hana::size(sorted_nodes_desc) == sizeof...(NODES));

      template<typename ... Ts>
      using desc_container  = std::tuple<typename Ts::template instance_type<ROOTS_CB, nodes_cb>...>;
      using nodes_links = hana_tuple_trait_t<decltype(sorted_nodes_desc), desc_container>;

      template<typename T> struct desc_node_type { using type = typename T::node_type; };
   public:
      auto build(graph_context& context) -> status_t {
         context.switch_subgraph_context(nodes_cb_);
         return build(context, sequence);
      }

      template<typename ROOT>
      auto connect_root(graph_context& context, ROOT& root, root_ports& ports) -> status_t {
         context.switch_subgraph_context(nodes_cb_);
         constexpr auto Index = tuple_element_index_v<typename ROOT::node_type, nodes_links, desc_node_type>;
         if constexpr (Index >= 0) {
            return std::get<Index>(nodes_links_).collect_actor_ports(context, ports);
         }
         return status_t::Ok;
      }

      auto start(graph_context& context) -> status_t {
         context.switch_subgraph_context(nodes_cb_);
         return start(context, node_cb_seq);
      }
      
      auto cleanup() { return cleanup(node_cb_seq); }
      auto stop() { return stop(node_cb_seq); }

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

      template<size_t ... I>
      auto cleanup(std::index_sequence<I...>) {
         (std::get<I>(nodes_cb_).cleanup(), ...);
      }

      template<size_t ... I>
      auto stop(std::index_sequence<I...>) {
         (std::get<I>(nodes_cb_).stop(), ...);
      }


   private:
      nodes_cb    nodes_cb_;
      nodes_links nodes_links_;
   };
};

GRAPH_DSL_NS_END

#define __sub_graph(...) GRAPH_DSL_NS::sub_graph<__VA_ARGS__>

#endif //GRAPH_SUB_GRAPH_H
