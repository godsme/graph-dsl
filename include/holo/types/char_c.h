//
// Created by Darwin Yuan on 2020/8/26.
//

#ifndef GRAPH_CHAR_C_H
#define GRAPH_CHAR_C_H

#include <holo/types/integral_c.h>

HOLO_NS_BEGIN

template <char V>
constexpr integral_c<char, V> char_c{};

HOLO_NS_END

#endif //GRAPH_CHAR_C_H
