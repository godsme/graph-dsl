//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_STATUS_H
#define GRAPH_STATUS_H

#include <graph/graph_ns.h>
#include <cstdint>

GRAPH_DSL_NS_BEGIN

enum class status_t : uint32_t {
   ok,
   failed
};

GRAPH_DSL_NS_END

#endif //GRAPH_STATUS_H
