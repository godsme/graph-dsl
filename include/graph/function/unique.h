//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_UNIQUE_H
#define GRAPH_UNIQUE_H

#include <graph/graph_ns.h>
#include <cstdint>
#include <boost/hana.hpp>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

template<typename LIST>
constexpr auto unique(LIST list) {
   if constexpr (hana::is_empty(list)) return hana::make_tuple();
   else {
      auto elem = hana::front(list);
      auto parts = hana::partition(list, [=](auto x) { return x == elem; });
      return hana::prepend(unique(hana::second(parts)), elem);
   }
}

GRAPH_DSL_NS_END

#endif //GRAPH_UNIQUE_H
