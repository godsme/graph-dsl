//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_CONTAINS_H
#define GRAPH_CONTAINS_H

#include <holo/holo_ns.h>
#include <holo/types/type_c.h>
#include <type_traits>
#include <tuple>
#include <graph/function/tuple_foreach.h>

HOLO_NS_BEGIN

template <typename TUPLE, typename T>
constexpr auto contains(T&& value, TUPLE&& tuple) {
   return GRAPH_DSL_NS::tuple_exists(tuple, [&](auto &&elem) {
      return value == elem;
   });
}

HOLO_NS_END

#endif //GRAPH_CONTAINS_H
