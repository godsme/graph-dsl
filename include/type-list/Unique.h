//
// Created by Darwin Yuan on 2020/8/13.
//

#ifndef GRAPH_DSL_UNIQUE_H
#define GRAPH_DSL_UNIQUE_H

#include <type_traits>
#include <type-list/Elem.h>

namespace detail {
   template<typename TL, typename = void, typename ... Ts>
   struct Unique {
      using type = TL;
   };

   template<typename TL, typename H, typename ... Ts>
   struct Unique<TL, std::enable_if_t<Elem_v<H, TL>>, H, Ts...> {
      using type = typename Unique<TL, void, Ts...>::type;
   };

   template<typename TL, typename H, typename ... Ts>
   struct Unique<TL, std::enable_if_t<!Elem_v<H, TL>>, H, Ts...> {
      using type = typename Unique<typename TL::template appendType<H>, void, Ts...>::type;
   };
}

template<typename ... Ts>
using Unique_t = typename detail::Unique<TypeList<>, void, Ts...>::type;

template<typename TL>
using Unique_tl = typename TL::template exportTo<Unique_t>;

#endif //GRAPH_DSL_UNIQUE_H
