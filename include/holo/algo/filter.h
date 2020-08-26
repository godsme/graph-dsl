//
// Created by Darwin Yuan on 2020/8/18.
//

#ifndef GRAPH_FILTER_H
#define GRAPH_FILTER_H

#include <holo/holo_ns.h>
#include <holo/types/type_c.h>
#include <type_traits>
#include <tuple>
#include <holo/algo/detail/filter_algo.h>

HOLO_NS_BEGIN

template <typename TUPLE, typename F>
constexpr auto filter(F&& f, TUPLE&& tuple) {
   return detail::filter_algo<true, 0>
      ( std::forward<F>(f)
      , std::forward<TUPLE>(tuple));
}

HOLO_NS_END

#endif //GRAPH_FILTER_H
