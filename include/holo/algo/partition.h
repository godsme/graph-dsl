//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_PARTITION_H
#define GRAPH_PARTITION_H

#include <holo/holo_ns.h>
#include <holo/types/type_c.h>
#include <holo/types/tuple_t.h>
#include <holo/algo/concat.h>
#include <type_traits>
#include <tuple>
#include <holo/algo/detail/filter_algo.h>

HOLO_NS_BEGIN

namespace detail {
   template <size_t I, typename TUPLE, typename SECOND_PART, typename F, typename ... Args>
   constexpr auto partition(F&& f, TUPLE&& tuple, SECOND_PART&& second, Args&& ... args) {
      if constexpr (I == std::tuple_size_v<std::decay_t<TUPLE>>) {
         return std::make_pair(std::make_tuple<Args...>(std::forward<Args>(args)...), std::forward<SECOND_PART>(second));
      } else if constexpr (std::is_same_v<decltype(f(std::get<I>(tuple))), std::integral_constant<bool, true>>) {
         return partition<I+1>(std::forward<F>(f), std::forward<TUPLE>(tuple), std::forward<SECOND_PART>(second), std::forward<Args>(args)..., std::get<I>(tuple));
      } else {
         return partition<I+1>(std::forward<F>(f), std::forward<TUPLE>(tuple), append(std::get<I>(tuple), second), std::forward<Args>(args)...);
      }
   }
}

template <typename TUPLE, typename F>
constexpr auto partition(F&& f, TUPLE&& tuple) {
   return detail::partition<0>
      ( std::forward<F>(f), std::forward<TUPLE>(tuple), tuple_t<>);
}

HOLO_NS_END

#endif //GRAPH_PARTITION_H
