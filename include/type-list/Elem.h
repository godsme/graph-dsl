//
// Created by Darwin Yuan on 2020/8/13.
//

#ifndef GRAPH_DSL_ELEM_H
#define GRAPH_DSL_ELEM_H

#include <type_traits>
#include <type-list/TypeList.h>

namespace detail {
   template<typename T, typename = void, typename ... Ts>
   struct Elem {
      constexpr static bool value = false;
   };

   template<typename T, typename H, typename ... Ts>
   struct Elem<T, std::enable_if_t<std::is_same_v<T, H>>, H, Ts...> {
      constexpr static bool value = true;
   };

   template<typename T, typename H, typename ... Ts>
   struct Elem<T, std::enable_if_t<!std::is_same_v<T, H>>, H, Ts...> {
      constexpr static bool value = Elem<T, void, Ts...>::value;
   };
}


template<typename T, typename TL>
struct Elem {
   template<typename ... Ts>
   using elem = detail::Elem<T, void, Ts...>;

   constexpr static bool value = TL::template exportTo<elem>::value;
};

template<typename T, typename TL>
constexpr bool Elem_v = Elem<T, TL>::value;

#endif //GRAPH_DSL_ELEM_H
