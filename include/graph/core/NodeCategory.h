//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_NODE_CATEGORY_H
#define GRAPH_NODE_CATEGORY_H

#include <graph/graph_ns.h>
#include <graph/util/tuple_element_by_type.h>
#include <graph/core/GraphContext.h>

GRAPH_DSL_NS_BEGIN

enum class NodeCategory {
   ROOT,
   INTERMEDIATE,
   LEAF
};

GRAPH_DSL_NS_END

#endif //GRAPH_NODE_CATEGORY_H
