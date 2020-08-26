//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_SORT_H
#define GRAPH_SORT_H

#include <holo/holo_ns.h>
#include <tuple>
#include <holo/types/tuple_t.h>
#include <holo/algo/head.h>
#include <holo/algo/filter.h>
#include <holo/algo/partition.h>

HOLO_NS_BEGIN

template <typename TUPLE, typename F>
constexpr auto sort(F&& f, const TUPLE& tuple) {
   if constexpr (std::tuple_size_v<TUPLE> == 0) {
      return tuple_t<>;
   } else {
      auto e = head(tuple);
      auto parts = partition([&](auto& elem) { return f(elem, e); }, tail(tuple));
      return concat(append(e, sort(std::forward<F>(f), parts.first)), sort(std::forward<F>(f), parts.second));
   }
}

HOLO_NS_END

#endif //GRAPH_SORT_H
