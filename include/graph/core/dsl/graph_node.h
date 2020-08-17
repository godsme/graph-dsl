//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_GRAPH_NODE_H
#define GRAPH_GRAPH_NODE_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/util/assertion.h>
#include <graph/core/graph_context.h>
#include <graph/core/dsl/graph_port.h>
#include <tuple>
#include <boost/hana.hpp>
#include <graph/function/unique.h>
#include <graph/core/cb/subgraph_node_cb.h>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

template<bool ROOT, typename NODE, typename ... PORTS>
struct graph_node final {
   constexpr static auto is_root = hana::bool_c<ROOT>;
   using node_type = NODE;
   constexpr static auto direct_decedents =
      unique(hana::flatten(hana::make_tuple(graph_port<PORTS>::node_list...)));
   constexpr static auto sequence = std::make_index_sequence<sizeof...(PORTS)>{};

   template<typename ROOTS_CB, typename NODES_CB>
   struct instance_type {
      constexpr static auto is_root = ROOT;
      using node_type = NODE;
      auto build(graph_context& context) -> status_t {
         auto& this_node = node_index<NODE, NODES_CB>::get_node(context);
         // skip all nodes whose ref count is 0
         if constexpr (is_root) {
            if(!node_index<NODE, ROOTS_CB>::get_root_node(context).present()) { return status_t::Ok; }
            GRAPH_EXPECT_SUCC(build_ports(context, sequence));
            if(enabled(sequence)) { this_node.enable(); }
            else { this_node.disable(); }
            return status_t::Ok;
         } else{
            if(!this_node.present()) { return status_t::Ok; }
            return build_ports(context, sequence);
         }
      }

      auto collect_actor_ports(graph_context& context, actor_ports& ports) -> status_t {
         return collect_actor_ports(context, ports, sequence);
      }

      auto collect_actor_ports(graph_context& context, root_actor_ports& ports) -> status_t {
         if constexpr (is_root) {
            return collect_actor_ports(context, ports, sequence);
         } else {
            return status_t::Failed;
         }
      }

   private:
      template<size_t ... I>
      auto build_ports(graph_context& context, std::index_sequence<I...>) -> status_t {
         status_t status = status_t::Ok;
         return (((status = std::get<I>(ports_).build(context)) == status_t::Ok) && ...) ?
            status_t::Ok : status;
      }

      template<size_t ... I>
      auto collect_actor_ports(graph_context& context, actor_ports& ports, std::index_sequence<I...>) -> status_t {
         status_t status = status_t::Ok;
         return (((status = std::get<I>(ports_).collect_actor_port(context, ports)) == status_t::Ok) && ...) ?
                status_t::Ok : status;
      }

      template<size_t ... I>
      auto collect_actor_ports(graph_context& context, root_actor_ports& ports, std::index_sequence<I...>) -> status_t {
         status_t status = status_t::Ok;
         return (((status = std::get<I>(ports_).collect_actor_port(context, ports)) == status_t::Ok) && ...) ?
                status_t::Ok : status;
      }

      template<size_t ... I>
      auto enabled(std::index_sequence<I...>) -> bool {
         return (std::get<I>(ports_).enabled() || ...);
      }

   private:
      std::tuple<typename graph_port<PORTS>::template instance_type<NODES_CB> ...> ports_;
   };
};

GRAPH_DSL_NS_END

#define __g_NODE(...) GRAPH_DSL_NS::graph_node<false, __VA_ARGS__>
#define __g_ROOT(...) GRAPH_DSL_NS::graph_node<true,  __VA_ARGS__>

#endif //GRAPH_SUBGRAPH_NODE_CB_H
