//
// Created by Darwin Yuan on 2020/8/18.
//

#ifndef GRAPH_FILTER_H
#define GRAPH_FILTER_H

#include <holo/holo_ns.h>
#include <holo/types/type_c.h>
#include <type_traits>
#include <tuple>

HOLO_NS_BEGIN

namespace detail {
   template <size_t I, typename TUPLE, typename F, typename ... Args>
   constexpr auto filter(F&& f, TUPLE&& tuple, Args&& ... args) {
      if constexpr (I == std::tuple_size_v<std::decay_t<TUPLE>>) {
         return std::make_tuple<Args...>(std::forward<Args>(args)...);
      } else if constexpr (std::is_same_v<decltype(f(std::get<I>(tuple))), std::integral_constant<bool, true>>) {
         return filter<I+1>(std::forward<F>(f), std::forward<TUPLE>(tuple), std::forward<Args>(args)..., std::get<I>(tuple));
      } else {
         return filter<I+1>(std::forward<F>(f), std::forward<TUPLE>(tuple), std::forward<Args>(args)...);
      }
   }
}

template <typename TUPLE, typename F>
constexpr auto filter(F&& f, TUPLE&& tuple) {
   return detail::filter<0>
      ( std::forward<F>(f)
      , std::forward<TUPLE>(tuple));
}

HOLO_NS_END

#endif //GRAPH_FILTER_H
