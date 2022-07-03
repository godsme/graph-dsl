//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_SUB_GRAPH_H
#define GRAPH_SUB_GRAPH_H

#include <graph/graph_ns.h>
#include <graph/core/SubGraphAnalyzer.h>
#include <graph/core/dsl/graph_node.h>
#include <holo/types/type_transform.h>
#include <holo/algo/head.h>
#include <maco/map.h>

GRAPH_DSL_NS_BEGIN

template<typename ... NODES>
struct sub_graph final {
   constexpr static auto ALL_SORTED_NODES = SubGraphAnalyzer<NODES...>::ALL_SORTED_SUBGRAPH_NODES;

   template<typename ROOTS_CB>
   struct InstanceType {
   private:
      template<typename ... Ts>
      using cb_container = std::tuple<subgraph_node_cb<typename Ts::NodeType, Ts::category>...>;
      static_assert(holo::length(ALL_SORTED_NODES) > 0, "");
      using nodes_cb = decltype(holo::map_to<cb_container>(ALL_SORTED_NODES));

   private:
      constexpr static auto sorted_nodes_desc = SubGraphAnalyzer<NODES...>::SORTED_NODES_DESC;
      static_assert(holo::length(sorted_nodes_desc) == sizeof...(NODES));

      template<typename ... Ts>
      using desc_container  = std::tuple<typename Ts::template InstanceType<ROOTS_CB, nodes_cb>...>;
      using nodes_links = decltype(holo::map_to<desc_container>(sorted_nodes_desc));

      template<typename T> struct desc_node_type { using type = typename T::NodeType; };
   public:
      auto Build(GraphContext& context) -> Status {
         context.SwitchSubgraphContext(nodes_cb_);
         return tuple_foreach(nodes_links_, [&](auto& link) {
            return link.Build(context);
         });
      }

      template<typename ROOT>
      auto ConnectRoot(GraphContext& context, ROOT& root, ActorPorts& ports) -> Status {
         context.template SwitchSubgraphContext(nodes_cb_);
         constexpr auto Index = tuple_element_index_v<typename ROOT::NodeType, nodes_links, desc_node_type>;
         if constexpr (Index >= 0) {
            return std::get<Index>(nodes_links_).CollectActorPorts(context, ports);
         } else {
            return Status::OK;
         }
      }

      auto Start(GraphContext& context) -> Status {
         context.SwitchSubgraphContext(nodes_cb_);
         return tuple_foreach_r(nodes_cb_, [&](auto& cb) {
            return cb.Start(context, nodes_links_);
         });
      }

      auto CleanUp() {
         return tuple_foreach_void(nodes_cb_, [](auto& cb) {
             cb.CleanUp();
         });
      }

      auto Stop() {
         return tuple_foreach_void(nodes_cb_, [](auto& cb) {
            cb.Stop();
         });
      }

   private:
      nodes_cb    nodes_cb_;
      nodes_links nodes_links_;
   };
};

template<typename, typename ... NODES>
using sub_graph_t = sub_graph<NODES...>;

GRAPH_DSL_NS_END


#define __sUb_gRaPh_each_node(x) , __sUb_gRaPh_node x
#define __sUb_gRaPh_nodes(...) __MACO_map(__sUb_gRaPh_each_node, __VA_ARGS__)
#define __g_SUB_GRAPH(...) GRAPH_DSL_NS::sub_graph_t<void __sUb_gRaPh_nodes(__VA_ARGS__)>

#endif //GRAPH_SUB_GRAPH_H
