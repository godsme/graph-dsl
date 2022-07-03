//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_STATUS_H
#define GRAPH_STATUS_H

#include <graph/graph_ns.h>
#include <cstdint>

GRAPH_DSL_NS_BEGIN

enum class Status : uint32_t {
   Ok,
   Failed
};

GRAPH_DSL_NS_END

#endif //GRAPH_STATUS_H
