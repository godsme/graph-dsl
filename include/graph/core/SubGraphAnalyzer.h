//
// Created by Darwin Yuan on 2020/8/15.
//

#ifndef GRAPH_SUB_GRAPH_ANALYZER_H
#define GRAPH_SUB_GRAPH_ANALYZER_H

#include <graph/graph_ns.h>
#include <graph/core/NodeCategory.h>
#include <holo/holo.h>
#include <maco/detail/int_succ.h>

GRAPH_DSL_NS_BEGIN

template <typename T, NodeCategory CATEGORY>
struct NodeTrait {
   using NodeType = T;
   constexpr static NodeCategory category = CATEGORY;
};

template<typename ... NODES>
struct SubGraphAnalyzer final {
   constexpr static auto nodesMap = holo::zip(
      holo::list_t<typename NODES::node_type...>,
      holo::make_list(NODES::direct_decedents...));

   template<typename T>
   constexpr static auto get_all_decedents(T list) {
      return list | holo::fold_left(holo::make_list(), [](auto acc, auto elem) {
         auto entry = nodesMap | holo::find_if([&](auto v){ return holo::first(v) == elem; });
         if constexpr(holo::is_nothing(entry)) {
            return holo::append(elem, acc);
         } else {
            return
            get_all_decedents(holo::second(entry))
            | holo::append(elem)
            | holo::concat(acc);
         }
      });
   }

   constexpr static auto allDecedentsMap = nodesMap | holo::transform([](auto elem) {
      return holo::make_pair(holo::first(elem), get_all_decedents(holo::second(elem)) | holo::unique());
   });

   constexpr static auto sortedNonLeafNodes =
           allDecedentsMap
           | holo::sort([](auto l, auto r) {
         return holo::contains(holo::first(l), holo::second(r)); })
      | holo::transform([](auto elem) {
         return holo::first(elem); })
      | holo::reverse();


   constexpr static auto rootNodes =
      holo::list_t<NODES...>
      | holo::filter([](auto elem){
         return decltype(elem)::type::is_root == holo::true_c; })
      | holo::transform([](auto elem){
         return holo::type_c<typename decltype(elem)::type::node_type>;
      });


   constexpr static auto sortedTaggedIntermediateNodes =
           sortedNonLeafNodes
           | holo::remove_if([](auto elem) {
         return holo::contains(elem, rootNodes); })
      | holo::transform([](auto elem) {
         return holo::type_c<NodeTrait<typename decltype(elem)::type, NodeCategory::INTERMEDIATE>>;
      });


   constexpr static auto leafTaggedNodes =
           allDecedentsMap
           | holo::fold_left(holo::make_list(), [](auto acc, auto elem){
         return holo::concat(acc, holo::second(elem)); })
      | holo::unique()
      | holo::remove_if([](auto elem) {
         return holo::contains(elem, sortedNonLeafNodes); })
      | holo::transform([](auto elem) {
         return holo::type_c<NodeTrait<typename decltype(elem)::type, NodeCategory::LEAF>>;
      });

public:
   constexpr static auto allSortedSubGraphNodes = holo::concat(sortedTaggedIntermediateNodes, leafTaggedNodes);
   constexpr static auto sortedNodesDesc =
           sortedNonLeafNodes
           | holo::transform([](auto elem){
         constexpr auto entry =
            holo::list_t<NODES...>
            | holo::find_if([=](auto v){
               return holo::type_c<typename decltype(v)::type::node_type> == elem;
            });

         static_assert(!holo::is_nothing(entry));
         return entry;
      });
};

GRAPH_DSL_NS_END

#endif //GRAPH_SUB_GRAPH_ANALYZER_H
