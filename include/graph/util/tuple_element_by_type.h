//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_TUPLE_ELEMENT_BY_TYPE_H
#define GRAPH_TUPLE_ELEMENT_BY_TYPE_H

#include <graph/graph_ns.h>
#include <tuple>

GRAPH_DSL_NS_BEGIN

template<typename T, typename TUPLE, int I = 0, typename = void>
struct tuple_element_by_type;

template<typename T, int I, class H, class... Tail >
struct tuple_element_by_type<T, std::tuple<H, Tail...>, I, std::enable_if_t<std::is_same_v<T, H>>> {
   constexpr static int Index = I;
};

template<typename T, int I, class H, class... Tail >
struct tuple_element_by_type<T, std::tuple<H, Tail...>, I, std::enable_if_t<!std::is_same_v<T, H>>> {
   constexpr static int Index = tuple_element_by_type<T, std::tuple<Tail...>, I+1>::Index;
};

template<typename T, int I>
struct tuple_element_by_type<T, std::tuple<>, I> {
   constexpr static int Index = -1;
};


GRAPH_DSL_NS_END

#endif //GRAPH_TUPLE_ELEMENT_BY_TYPE_H
