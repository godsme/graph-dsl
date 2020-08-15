//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_TUPLE_ELEMENT_BY_TYPE_H
#define GRAPH_TUPLE_ELEMENT_BY_TYPE_H

#include <graph/graph_ns.h>
#include <tuple>

GRAPH_DSL_NS_BEGIN

namespace detail {
   template<typename T, typename TUPLE, template<typename> typename F, int I = 0, typename = void>
   struct tuple_element_by_type;

   template<typename T, template<typename> typename F, int I, class H, class... Tail >
   struct tuple_element_by_type<T, std::tuple<H, Tail...>, F, I,
      std::enable_if_t<std::is_same_v<T, typename F<H>::type>>> {
      constexpr static int Index = I;
   };

   template<typename T, template<typename> typename F, int I, class H, class... Tail >
   struct tuple_element_by_type<T, std::tuple<H, Tail...>, F, I,
      std::enable_if_t<!std::is_same_v<T, typename F<H>::type>>> {
      constexpr static int Index = tuple_element_by_type<T, std::tuple<Tail...>, F, I+1>::Index;
   };

   template<typename T, template<typename> typename F, int I>
   struct tuple_element_by_type<T, std::tuple<>, F, I> {
      constexpr static int Index = -1;
   };
}

template<typename T, typename TUPLE, template<typename> typename F>
constexpr static int tuple_element_index_v = detail::tuple_element_by_type<T, TUPLE, F>::Index;

GRAPH_DSL_NS_END

#endif //GRAPH_TUPLE_ELEMENT_BY_TYPE_H
