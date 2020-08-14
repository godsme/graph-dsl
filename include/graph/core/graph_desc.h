//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_GRAPH_DESC_H
#define GRAPH_GRAPH_DESC_H

#include <graph/graph_ns.h>
#include <boost/hana.hpp>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

template<typename ... NODES>
struct graph_desc {
   constexpr static auto nodes_map = hana::make_tuple(
         hana::make_pair(hana::type_c<typename NODES::node_type>, NODES::direct_decedents)...);

   template<typename T>
   constexpr static auto get_all_decedents(T list) {
      return hana::fold_left(list, hana::tuple_t<>, [](auto acc, auto elem) {
         auto entry = hana::find_if(nodes_map, [=](auto v){ return hana::first(v) == elem; });
         if constexpr(hana::is_nothing(entry)) {
            return hana::append(acc, elem);
         } else {
            return hana::concat(acc, hana::prepend(get_all_decedents(hana::second(entry.value())), elem));
         }
      });
   }

   constexpr static auto all_decedents_map = hana::transform(nodes_map, [](auto elem) {
      return hana::make_pair(hana::first(elem), unique(get_all_decedents(hana::second(elem))));
   });

   constexpr static auto sorted_map = hana::sort(all_decedents_map, [](auto l, auto r) {
      return hana::contains(hana::second(r), hana::first(l));
   });

   constexpr static auto sorted_nodes = hana::reverse(hana::transform(sorted_map, [](auto elem) {
      return hana::first(elem);
   }));

   constexpr static auto all_decedents = hana::remove_if(unique(
      hana::fold_left(all_decedents_map, hana::tuple_t<>, [](auto acc, auto elem){
         return hana::concat(acc, hana::second(elem));
      })),
      [](auto elem) {
         return hana::contains(sorted_nodes, elem);
      });

   constexpr static auto all_sorted_nodes = hana::concat(sorted_nodes, all_decedents);
};

GRAPH_DSL_NS_END

#define __graph(...) GRAPH_DSL_NS::graph_desc<__VA_ARGS__>

#endif //GRAPH_GRAPH_DESC_H
