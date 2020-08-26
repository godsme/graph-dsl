//
// Created by Darwin Yuan on 2020/8/15.
//

#ifndef GRAPH_SUB_GRAPH_ANALIZER_H
#define GRAPH_SUB_GRAPH_ANALIZER_H

#include <graph/graph_ns.h>
#include <graph/core/node_category.h>
#include <holo/holo.h>

GRAPH_DSL_NS_BEGIN

template <typename T, node_category CATEGORY>
struct node_trait {
   using node_type = T;
   constexpr static node_category category = CATEGORY;
};

template<typename ... NODES>
class sub_graph_analizer final {
   constexpr static auto nodes_map = holo::make_tuple(
      holo::make_pair(holo::type_c<typename NODES::node_type>, NODES::direct_decedents)...);

   template<typename T>
   constexpr static auto get_all_decedents(T list) {
      return holo::fold_left(holo::tuple_t<>, [](auto acc, auto elem) {
         auto entry = holo::find_if([=](auto v){ return holo::first(v) == elem; }, nodes_map);
         if constexpr(holo::is_nothing(entry)) {
            return holo::append(elem, acc);
         } else {
            return holo::concat(acc, holo::append(elem, get_all_decedents(holo::second(entry))));
         }
      }, list);
   }

   constexpr static auto all_decedents_map = nodes_map |
      holo::transform([](auto elem) {
         return holo::make_pair(holo::first(elem), holo::unique(get_all_decedents(holo::second(elem))));
      });

   constexpr static auto sorted_non_leaf_nodes = all_decedents_map
      | holo::sort([](auto l, auto r) {
          return holo::contains(holo::first(l), holo::second(r)); })
      | holo::transform([](auto elem) {
         return holo::first(elem); })
      | holo::reverse();

   constexpr static auto root_nodes = holo::tuple_t<NODES...>
      | holo::filter([](auto elem){
          return decltype(elem)::type::is_root == holo::bool_c<true>; })
      | holo::transform([](auto elem){
          return holo::type_c<typename decltype(elem)::type::node_type>;
       });

   constexpr static auto sorted_tagged_intermediate_nodes = sorted_non_leaf_nodes
      | holo::remove_if([](auto elem) {
         return holo::contains(elem, root_nodes); })
      | holo::transform([](auto elem) {
         return holo::type_c<node_trait<typename decltype(elem)::type, node_category::Intermediate>>;
        });

   constexpr static auto leaf_tagged_nodes = all_decedents_map
      | holo::fold_left(holo::tuple_t<>, [](auto acc, auto elem){
         return holo::concat(acc, holo::second(elem)); })
      | holo::unique()
      | holo::remove_if([](auto elem) {
         return holo::contains(elem, sorted_non_leaf_nodes); })
      | holo::transform([](auto elem) {
         return holo::type_c<node_trait<typename decltype(elem)::type, node_category::Leaf>>;
        });

public:
   constexpr static auto all_sorted_sub_graph_nodes = holo::concat(sorted_tagged_intermediate_nodes, leaf_tagged_nodes);
   constexpr static auto sorted_nodes_desc = sorted_non_leaf_nodes
      | holo::transform([](auto elem){
         constexpr auto entry = holo::tuple_t<NODES...>
            | holo::find_if([=](auto v){
               return holo::type_c<typename decltype(v)::type::node_type> == elem; });

         static_assert(!holo::is_nothing(entry));
         return entry;
      });
};

GRAPH_DSL_NS_END

#endif //GRAPH_SUB_GRAPH_ANALIZER_H
