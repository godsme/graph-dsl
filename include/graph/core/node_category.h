//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_NODE_CATEGORY_H
#define GRAPH_NODE_CATEGORY_H

#include <graph/graph_ns.h>
#include <graph/util/tuple_element_by_type.h>
#include <graph/core/graph_context.h>

GRAPH_DSL_NS_BEGIN

enum class node_category {
   Root,
   Intermediate,
   Leaf
};

GRAPH_DSL_NS_END

#endif //GRAPH_NODE_CATEGORY_H
