//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_GRAPH_MSGS_H
#define GRAPH_GRAPH_MSGS_H

#include <nano-caf/msg/PredefinedMsgs.h>
#include <graph/graph_ns.h>
#include <graph/core/ActorPorts.h>
#include <memory>

GRAPH_DSL_NS_BEGIN

CAF_def_message(PortsUpdateMsg,
    (ports, std::unique_ptr<ActorPorts>));

GRAPH_DSL_NS_END

#endif //GRAPH_GRAPH_MSGS_H
