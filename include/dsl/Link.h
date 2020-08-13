//
// Created by Darwin Yuan on 2020/8/13.
//

#ifndef GRAPH_DSL_LINK_H
#define GRAPH_DSL_LINK_H

#include <dsl/NodeTrait.h>

template<typename T>
struct Link;

template<typename PORT, typename NODE_LIST>
struct Link<auto (PORT) -> NODE_LIST> {
   using PortType = PORT;
   using NodeList = typename NodeTrait<NODE_LIST>::NodeList;
};

#endif //GRAPH_DSL_LINK_H
