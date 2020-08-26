//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_FILTER_ALGO_H
#define GRAPH_FILTER_ALGO_H

#include <holo/holo_ns.h>
#include <holo/types/type_c.h>
#include <type_traits>
#include <tuple>

HOLO_NS_BEGIN

namespace detail {
   template <bool IN, size_t I, typename TUPLE, typename F, typename ... Args>
   constexpr auto filter_algo(F&& f, TUPLE&& tuple, Args&& ... args) {
      if constexpr (I == std::tuple_size_v<std::decay_t<TUPLE>>) {
         return std::make_tuple<Args...>(std::forward<Args>(args)...);
      } else if constexpr (std::is_same_v<decltype(f(std::get<I>(tuple))), integral_c<bool, IN>>) {
         return filter_algo<IN, I+1>(std::forward<F>(f), std::forward<TUPLE>(tuple), std::forward<Args>(args)..., std::get<I>(tuple));
      } else {
         return filter_algo<IN, I+1>(std::forward<F>(f), std::forward<TUPLE>(tuple), std::forward<Args>(args)...);
      }
   }
}

HOLO_NS_END

#endif //GRAPH_FILTER_H
