//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_HEAD_H
#define GRAPH_HEAD_H

#include <holo/holo_ns.h>
#include <tuple>
#include <holo/types/tuple_t.h>

HOLO_NS_BEGIN

namespace detail {
   template <typename TUPLE, size_t ... I>
   constexpr auto tail(const TUPLE& tuple, std::index_sequence<I...>) {
      return std::make_tuple(std::get<I+1>(tuple)...);
   }
}

template <typename TUPLE>
constexpr auto head(const TUPLE& tuple) {
   static_assert(std::tuple_size_v<TUPLE> > 0, "empty list");
   return std::get<0>(tuple);
}

template <typename TUPLE>
constexpr auto tail(const TUPLE& tuple) {
   static_assert(std::tuple_size_v<TUPLE> > 0, "empty list");
   return detail::tail(tuple, std::make_index_sequence<std::tuple_size_v<TUPLE> - 1>{});
}

HOLO_NS_END

#endif //GRAPH_HEAD_H
