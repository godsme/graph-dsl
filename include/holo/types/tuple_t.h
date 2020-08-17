//
// Created by Darwin Yuan on 2020/8/17.
//

#ifndef GRAPH_TUPLE_T_H
#define GRAPH_TUPLE_T_H

#include <holo/holo_ns.h>
#include <holo/types/type_c.h>
#include <type_traits>
#include <tuple>

HOLO_NS_BEGIN

template <typename ... Ts>
constexpr std::tuple<type_c_t<Ts>...> tuple_t{};

HOLO_NS_END

#endif //GRAPH_TUPLE_T_H
