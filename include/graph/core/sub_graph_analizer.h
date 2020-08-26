//
// Created by Darwin Yuan on 2020/8/15.
//

#ifndef GRAPH_SUB_GRAPH_ANALIZER_H
#define GRAPH_SUB_GRAPH_ANALIZER_H

#include <graph/graph_ns.h>
#include <graph/core/node_category.h>
#include <holo/types/type_c.h>
#include <holo/types/tuple_t.h>
#include <holo/algo/fold_left.h>
#include <holo/algo/find_if.h>
#include <holo/algo/concat.h>
#include <holo/algo/transform.h>
#include <holo/algo/contains.h>
#include <holo/algo/sort.h>
#include <holo/algo/reverse.h>
#include <holo/algo/unique.h>
#include <holo/algo/remove_if.h>
#include <holo/algo/is_nothing.h>
#include <holo/types/bool_c.h>

GRAPH_DSL_NS_BEGIN

template <typename T, node_category CATEGORY>
struct node_trait {
   using node_type = T;
   constexpr static node_category category = CATEGORY;
};

template<typename ... NODES>
class sub_graph_analizer final {
   constexpr static auto nodes_map = std::make_tuple(
      std::make_pair(holo::type_c<typename NODES::node_type>, NODES::direct_decedents)...);

   constexpr static auto all_nodes_desc = holo::tuple_t<NODES...>;

   template<typename T>
   constexpr static auto get_all_decedents(T list) {
      return holo::fold_left(holo::tuple_t<>, [](auto acc, auto elem) {
         auto entry = holo::find_if([=](auto v){ return v.first == elem; }, nodes_map);
         if constexpr(holo::is_nothing(entry)) {
            return holo::append(elem, acc);
         } else {
            return holo::concat(acc, holo::append(elem, get_all_decedents((*entry).second)));
         }
      }, list);
   }

   constexpr static auto all_decedents_map = holo::transform([](auto elem) {
      return std::make_pair(elem.first, holo::unique(get_all_decedents(elem.second)));
   }, nodes_map);

   constexpr static auto sorted_map = holo::sort([](auto l, auto r) {
      return holo::contains(l.first, r.second);
   }, all_decedents_map);

   constexpr static auto sorted_non_leaf_nodes = holo::reverse(
      holo::transform([](auto elem) {
         return elem.first;
      }, sorted_map));

   constexpr static auto nodes_partition =
      holo::filter([](auto elem){
         return decltype(elem)::type::is_root == holo::bool_c<true>;
      }, holo::tuple_t<NODES...>);

   constexpr static auto root_nodes =
      holo::transform([](auto elem){
         return holo::type_c<typename decltype(elem)::type::node_type>;
      }, nodes_partition);

   constexpr static auto sorted_tagged_intermediate_nodes = holo::transform(
      [](auto elem) {
         return holo::type_c<node_trait<typename decltype(elem)::type, node_category::Intermediate>>;
      },
      holo::remove_if([](auto elem) {
         return holo::contains(elem, root_nodes);
      }, sorted_non_leaf_nodes));

   constexpr static auto root_tagged_nodes =
      holo::transform([](auto elem){
         return holo::type_c<node_trait<typename decltype(elem)::type, node_category::Root>>;
      }, root_nodes);

   constexpr static auto all_decedents = holo::unique(
      holo::fold_left(holo::tuple_t<>, [](auto acc, auto elem){
         return holo::concat(acc, elem.second);
      }, all_decedents_map));

   constexpr static auto leaf_tagged_nodes = holo::transform(
      [](auto elem) {
         return holo::type_c<node_trait<typename decltype(elem)::type, node_category::Leaf>>;
      },
      holo::remove_if([](auto elem) {
         return holo::contains(elem, sorted_non_leaf_nodes);
      }, all_decedents));

public:
   constexpr static auto all_sorted_sub_graph_nodes = holo::concat(sorted_tagged_intermediate_nodes, leaf_tagged_nodes);
   constexpr static auto sorted_nodes_desc =
      holo::transform([](auto elem){
         constexpr auto entry = holo::find_if([=](auto v){
            return holo::type_c<typename decltype(v)::type::node_type> == elem;
         }, all_nodes_desc);

         static_assert(entry.has_value());
         return *entry;
      }, sorted_non_leaf_nodes);
};

GRAPH_DSL_NS_END

#endif //GRAPH_SUB_GRAPH_ANALIZER_H
