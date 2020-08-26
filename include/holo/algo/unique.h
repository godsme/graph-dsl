//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_UNIQUE_H
#define GRAPH_UNIQUE_H

#include <holo/algo/head.h>
#include <holo/algo/partition.h>
#include <holo/algo/concat.h>

HOLO_NS_BEGIN

template<typename LIST>
constexpr auto unique(const LIST& list) {
   if constexpr (std::tuple_size_v<LIST> == 0) return holo::tuple_t<>;
   else {
      auto elem = holo::head(list);
      auto parts = holo::partition([=](auto x) { return x == elem; }, list);
      return holo::prepend(elem, holo::unique(parts.second));
   }
}

HOLO_NS_END

#endif //GRAPH_UNIQUE_H
