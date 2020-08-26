//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_APPEND_H
#define GRAPH_APPEND_H

#include <holo/holo_ns.h>
#include <holo/types/type_c.h>
#include <type_traits>
#include <tuple>

HOLO_NS_BEGIN

template <typename TUPLE, typename T>
constexpr auto append(T&& value, TUPLE&& tuple) {
   return std::tuple_cat(tuple, std::tuple<std::decay_t<T>>(std::forward<T>(value)));
}

template <typename TUPLE, typename T>
constexpr auto prepend(T&& value, TUPLE&& tuple) {
   return std::tuple_cat(std::tuple<std::decay_t<T>>(std::forward<T>(value)), tuple);
}

template <typename TUPLE1, typename TUPLE2>
constexpr auto concat(TUPLE1&& tuple1, TUPLE2&& tuple2) {
   return std::tuple_cat(tuple1, tuple2);
}

HOLO_NS_END

#endif //GRAPH_APPEND_H
