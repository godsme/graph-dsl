//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_CONTAINS_H
#define GRAPH_CONTAINS_H

#include <holo/holo_ns.h>
#include <holo/types/type_c.h>
#include <type_traits>
#include <tuple>
#include <holo/algo/head.h>
#include <holo/types/integral_c.h>

HOLO_NS_BEGIN

template <typename TUPLE, typename T>
constexpr auto contains(T&& value, const TUPLE& tuple) {
   if constexpr (std::tuple_size_v<TUPLE> == 0) {
      return integral_c<bool, false>{};
   } else if constexpr (std::is_same_v<integral_c<bool, true>, decltype(holo::head(tuple) == value)>) {
      return integral_c<bool, true>{};
   } else {
      return contains(std::forward<T>(value), holo::tail(tuple));
   }
}

HOLO_NS_END

#endif //GRAPH_CONTAINS_H
