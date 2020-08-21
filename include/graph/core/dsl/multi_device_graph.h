//
// Created by Darwin Yuan on 2020/8/21.
//

#ifndef GRAPH_MULTI_DEVICE_GRAPH_H
#define GRAPH_MULTI_DEVICE_GRAPH_H

#include <graph/graph_ns.h>
#include <graph/util/assertion.h>
#include <graph/core/graph_context.h>
#include <graph/core/dsl/multi_devices.h>

GRAPH_DSL_NS_BEGIN

template<typename GRAPH, typename MULTI_DEVICE>
struct multi_device_graph : private GRAPH {
   using GRAPH::stop;

   auto start(graph_context& context) -> status_t {
      root_state init_state = MULTI_DEVICE::Init_State;
      return status_t::Ok;
   }

private:
   MULTI_DEVICE multi_device_{};
};

GRAPH_DSL_NS_END

#endif //GRAPH_MULTI_DEVICE_GRAPH_H
