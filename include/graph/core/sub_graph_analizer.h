//
// Created by Darwin Yuan on 2020/8/15.
//

#ifndef GRAPH_SUB_GRAPH_ANALIZER_H
#define GRAPH_SUB_GRAPH_ANALIZER_H

#include <graph/graph_ns.h>
#include <boost/hana.hpp>
#include <graph/core/node_category.h>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

template <typename T, node_category CATEGORY>
struct node_trait {
   using node_type = T;
   constexpr static node_category category = CATEGORY;
};

template<typename ... NODES>
class sub_graph_analizer final {
   constexpr static auto nodes_map = hana::make_tuple(
      hana::make_pair(hana::type_c<typename NODES::node_type>, NODES::direct_decedents)...);

   constexpr static auto all_nodes_desc = hana::tuple_t<NODES...>;

   template<typename T>
   constexpr static auto get_all_decedents(T list) {
      return hana::fold_left(list, hana::tuple_t<>, [](auto acc, auto elem) {
         auto entry = hana::find_if(nodes_map, [=](auto v){ return hana::first(v) == elem; });
         if constexpr(hana::is_nothing(entry)) {
            return hana::append(acc, elem);
         } else {
            return hana::concat(acc, hana::append(get_all_decedents(hana::second(*entry)), elem));
         }
      });
   }

   constexpr static auto all_decedents_map = hana::transform(nodes_map, [](auto elem) {
      return hana::make_pair(hana::first(elem), unique(get_all_decedents(hana::second(elem))));
   });

   constexpr static auto sorted_map = hana::sort(all_decedents_map, [](auto l, auto r) {
      return hana::contains(hana::second(r), hana::first(l));
   });

   constexpr static auto sorted_non_leaf_nodes = hana::reverse(
      hana::transform(sorted_map, [](auto elem) {
         return hana::first(elem);
      }));

   constexpr static auto nodes_partition =
      hana::filter(hana::tuple_t<NODES...>, [](auto elem){
         return decltype(elem)::type::is_root == hana::bool_c<true>;
      });

   constexpr static auto root_nodes =
      hana::transform(nodes_partition, [](auto elem){
         return hana::type_c<typename decltype(elem)::type::node_type>;
      });

   constexpr static auto sorted_tagged_intermediate_nodes = hana::transform(
      hana::remove_if(sorted_non_leaf_nodes, [](auto elem) {
         return hana::contains(root_nodes, elem);
      }),
      [](auto elem) {
         return hana::type_c<node_trait<typename decltype(elem)::type, node_category::Intermediate>>;
      });

   constexpr static auto root_tagged_nodes =
      hana::transform(root_nodes, [](auto elem){
         return hana::type_c<node_trait<typename decltype(elem)::type, node_category::Root>>;
      });

   constexpr static auto all_decedents = unique(
      hana::fold_left(all_decedents_map, hana::tuple_t<>, [](auto acc, auto elem){
         return hana::concat(acc, hana::second(elem));
      }));

   constexpr static auto leaf_tagged_nodes = hana::transform(
      hana::remove_if(all_decedents, [](auto elem) {
         return hana::contains(sorted_non_leaf_nodes, elem);
      }),
      [](auto elem) {
         return hana::type_c<node_trait<typename decltype(elem)::type, node_category::Leaf>>;
      });

public:
   constexpr static auto all_sorted_sub_graph_nodes = hana::concat(sorted_tagged_intermediate_nodes, leaf_tagged_nodes);
   constexpr static auto sorted_nodes_desc =
      hana::transform(sorted_non_leaf_nodes, [](auto elem){
         auto entry = hana::find_if(all_nodes_desc, [=](auto v){
            return hana::type_c<typename decltype(v)::type::node_type> == elem;
         });

         static_assert(hana::is_just(entry), "");
         return *entry;
      });
};

GRAPH_DSL_NS_END

#endif //GRAPH_SUB_GRAPH_ANALIZER_H
