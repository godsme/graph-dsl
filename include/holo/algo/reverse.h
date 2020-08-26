//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_REVERSE_H
#define GRAPH_REVERSE_H

#include <holo/holo_ns.h>
#include <tuple>

HOLO_NS_BEGIN

namespace detail {
   template<typename TUPLE, size_t ... I>
   constexpr auto reverse(const TUPLE& tuple, std::index_sequence<I...>) {
      return std::make_tuple(std::get<sizeof...(I) - I - 1>(tuple)...);
   }
}

template <typename TUPLE>
constexpr auto reverse(const TUPLE& tuple) {
   return detail::reverse(tuple, std::make_index_sequence<std::tuple_size_v<TUPLE>>{});
}

HOLO_NS_END

#endif //GRAPH_REVERSE_H
