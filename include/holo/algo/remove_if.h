//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_REMOVE_IF_H
#define GRAPH_REMOVE_IF_H

#include <holo/algo/detail/filter_algo.h>

HOLO_NS_BEGIN

template<typename TUPLE, typename F>
constexpr auto remove_if(F&& f, TUPLE&& tuple) {
   return detail::filter_algo<false, 0>
      ( std::forward<F>(f)
         , std::forward<TUPLE>(tuple));
}

HOLO_NS_END

#endif //GRAPH_REMOVE_IF_H
