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
};

GRAPH_DSL_NS_END

#define __graph(...) GRAPH_DSL_NS::graph_desc<__VA_ARGS__>

#endif //GRAPH_GRAPH_DESC_H
