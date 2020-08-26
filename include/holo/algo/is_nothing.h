//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_IS_NOTHING_H
#define GRAPH_IS_NOTHING_H

#include <holo/holo_ns.h>
#include <optional>

HOLO_NS_BEGIN

constexpr auto is_nothing(const std::nullopt_t& v) {
   return true;
}

template<typename T>
constexpr auto is_nothing(const T& v) {
   return false;
}

template<typename T>
constexpr auto is_just(const std::optional<T>& v) {
   return true;
}

template<typename T>
constexpr auto is_just(const T& v) {
   return false;
}

HOLO_NS_END

#endif //GRAPH_IS_NOTHING_H
