//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_AP_H
#define GRAPH_AP_H

#include <holo/holo_ns.h>
#include <holo/types/type_c.h>
#include <type_traits>
#include <tuple>
#include <holo/types/tuple_t.h>
#include "flatten.h"

HOLO_NS_BEGIN

template<typename T, typename LIST_2, typename LIST>
constexpr auto ap_2(const T& value, const LIST_2& list2, const LIST& result) {
   if constexpr (std::tuple_size_v<LIST_2> == 0) {
      return result;
   } else {
      return ap_2(value, tail(list2), append(std::make_pair(value, head(list2)), result));
   }
}

template<typename LIST_1, typename LIST_2, size_t ... I>
constexpr auto ap_1(const LIST_1& list_1, const LIST_2& list_2, std::index_sequence<I...>) {
   return holo::flatten(std::make_tuple(ap_2(std::get<I>(list_1), list_2, holo::tuple_t<>) ...));
}

template<typename LIST_1, typename LIST_2>
constexpr auto ap(const LIST_1& list1, const LIST_2& list2) {
   if constexpr (std::tuple_size_v<LIST_1> == 0 || std::tuple_size_v<LIST_2> == 0) {
      return holo::tuple_t<>;
   } else {
      return ap_1(list1, list2, std::make_index_sequence<std::tuple_size_v<LIST_1>>{});
   }
}

HOLO_NS_END

#endif //GRAPH_AP_H
