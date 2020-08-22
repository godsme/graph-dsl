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
#include <nano-caf/util/macro_basic.h>
#include <nano-caf/util/macro_pp_size.h>
#include <nano-caf/util/macro_struct.h>
#include <nano-caf/util/macro_reflex_call.h>
#include <tuple>
#include <boost/hana.hpp>
#include <graph/function/unique.h>
#include <graph/function/tuple_foreach.h>
#include <graph/core/cb/subgraph_node_cb.h>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

template<bool ROOT, typename NODE, typename ... PORTS>
struct graph_node final {
   constexpr static auto is_root = hana::bool_c<ROOT>;
   using node_type = NODE;
   constexpr static auto direct_decedents =
      unique(hana::flatten(hana::make_tuple(graph_port<PORTS>::node_list...)));

   template<typename NODES_CB, bool IS_ROOT>
   struct instance_type_base {
      constexpr static auto is_root = IS_ROOT;
      using node_type = NODE;
   protected:
      auto build_(graph_context &context, bool present) -> status_t {
         if (present) {
            GRAPH_EXPECT_SUCC(tuple_foreach(ports_, [&](auto &port) { return port.build(context); }));
         } else {
            tuple_foreach_void(ports_, [&](auto &port) { port.release(context); });
         }
         return status_t::Ok;
      }

      std::tuple<typename graph_port<PORTS>::template instance_type<NODES_CB> ...> ports_;
   };

   template<typename ROOTS_CB, typename NODES_CB, bool IS_ROOT>
   struct instance_trait;

   template<typename ROOTS_CB, typename NODES_CB>
   struct instance_trait<ROOTS_CB, NODES_CB, true> : instance_type_base<NODES_CB, true> {
      using self = instance_type_base<NODES_CB, true>;

      auto build(graph_context& context) -> status_t {
         return self::build_(context, node_index<NODE, ROOTS_CB>::get_root_node(context).present());
      }

      auto collect_actor_ports(graph_context& context, root_ports& ports) -> status_t {
         if(!tuple_exists(self::ports_, [](auto& port) { return port.enabled(); })) {
            return status_t::Ok;
         }
         return tuple_foreach(self::ports_, [&](auto& port) {
            return port.collect_actor_port(context, ports);
         });
      }
   };

   template<typename ROOTS_CB, typename NODES_CB>
   struct instance_trait<ROOTS_CB, NODES_CB, false> : instance_type_base<NODES_CB, false> {
      using self = instance_type_base<NODES_CB, false>;
      auto build(graph_context& context) -> status_t {
         return self::build_(context, node_index<NODE, NODES_CB>::get_node(context).present());
      }

      auto collect_actor_ports(graph_context& context, actor_ports& ports) -> status_t {
         return tuple_foreach(self::ports_, [&](auto& port) {
            return port.collect_actor_port(context, ports);
         });
      }
   };

   template<typename ROOTS_CB, typename NODES_CB>
   using instance_type = instance_trait<ROOTS_CB, NODES_CB, ROOT>;
};

GRAPH_DSL_NS_END

#define __gRaPh_each_link(n, x) , auto x
#define __gRaPh_links(node, ...) \
node __CUB_overload(__CUB_repeat_call_, __VA_ARGS__) (__gRaPh_each_link, 0, __VA_ARGS__)

#define __g_NODE(...) GRAPH_DSL_NS::graph_node<false, __gRaPh_links(__VA_ARGS__)>
#define __g_ROOT(...) GRAPH_DSL_NS::graph_node<true,  __gRaPh_links(__VA_ARGS__)>

#endif //GRAPH_SUBGRAPH_NODE_CB_H
