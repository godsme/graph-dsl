//
// Created by Darwin Yuan on 2020/8/18.
//

#ifndef GRAPH_TRANSFORM_H
#define GRAPH_TRANSFORM_H

#include <holo/holo_ns.h>
#include <holo/types/type_c.h>
#include <type_traits>
#include <tuple>

HOLO_NS_BEGIN

namespace detail {
   template <size_t ... I, typename TUPLE, typename F>
   constexpr auto transform(F&& f, const TUPLE& tuple, std::index_sequence<I...>) {
      return std::tuple{f(std::get<I>(tuple))...};
   }
}

template <typename TUPLE, typename F>
constexpr auto transform(F&& f, const TUPLE& tuple) {
   return detail::transform
      ( std::forward<F>(f)
      , tuple
      , std::make_index_sequence<std::tuple_size_v<std::decay_t<TUPLE>>>{});
}

HOLO_NS_END

#endif //GRAPH_TRANSFORM_H
