//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_BOOL_C_H
#define GRAPH_BOOL_C_H

#include <holo/types/integral_c.h>
#include <type_traits>

HOLO_NS_BEGIN

template<bool V>
constexpr integral_c<bool, V> bool_c{};

HOLO_NS_END

#endif //GRAPH_BOOL_C_H
