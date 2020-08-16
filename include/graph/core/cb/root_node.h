//
// Created by Darwin Yuan on 2020/8/16.
//

#ifndef GRAPH_ROOT_NODE_H
#define GRAPH_ROOT_NODE_H

#include <graph/graph_ns.h>
#include <nano-caf/core/actor/actor_handle.h>
#include <graph/util/tuple_element_by_type.h>
#include <graph/core/graph_context.h>
#include <graph/status.h>
#include <cstdint>
#include <iostream>
#include <graph/util/assertion.h>

GRAPH_DSL_NS_BEGIN

template<typename NODE>
struct root_node {
   using node_type = NODE;

   nano_caf::actor_handle actor_handle_;

   auto start(graph_context& context) -> status_t {
      if(!running_) {
         actor_handle_ = NODE::spawn(context);
         GRAPH_EXPECT_TRUE(actor_handle_.exists());
         running_ = true;
      }

      return status_t::Ok;
   }

   auto stop() {
      if(running_) {
         actor_handle_.exit(nano_caf::exit_reason::normal);
         running_ = false;
         actor_handle_.release();
      }
   }

protected:
   bool    running_{false};
};

GRAPH_DSL_NS_END

#endif //GRAPH_ROOT_NODE_H
