//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_TUPLE_TRAIT_H
#define GRAPH_TUPLE_TRAIT_H

#include <holo/holo_ns.h>
#include <holo/types/type_c.h>
#include <type_traits>
#include <tuple>

HOLO_NS_BEGIN


template<typename T, template<typename ...> typename C>
struct tuple_trait;

template <template<typename ...> typename C, typename ... Ts>
struct tuple_trait<std::tuple<Ts...>, C> {
   using type = C<typename Ts::type...>;
};

template<typename T, template<typename ...> typename C>
using tuple_trait_t = typename tuple_trait<std::decay_t<T>, C>::type;

HOLO_NS_END

#endif //GRAPH_TUPLE_TRAIT_H
