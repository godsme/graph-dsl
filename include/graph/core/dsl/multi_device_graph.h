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

template<typename GRAPH, typename STATE_SELECTOR, typename STATE_TRANSITION>
struct multi_device_graph : private GRAPH {
   template<typename ENV>
   auto on_env_change(graph_context& context, ENV const& env) -> status_t {
      paths_ = multi_device_.get_transitions(env);

      if(paths_.size > 0) {
         current_state_ = paths_.size == 1 ? 0 : 1;
         context.update_root_state(paths_.state[current_state_]);
         return GRAPH::refresh(context);
      } else {
         current_state_ = 0;
      }

      return status_t::Ok;
   }

   auto on_switch_done(graph_context& context) -> status_t {
      if(current_state_ < paths_.size) {
         multi_device_.update_state(paths_.state[current_state_]);
         if((++current_state_) < paths_.size) {
            context.update_root_state(paths_.state[current_state_]);
            return GRAPH::refresh(context);
         }
      } else {
         return status_t::Ok;
      }
   }

   auto stop() {
      GRAPH::stop();
      multi_device_.cleanup();
      current_state_ = 0;
      paths_.cleanup();
   }

   using GRAPH::get_root;
   using GRAPH::refresh;

private:
   multi_device<STATE_SELECTOR, STATE_TRANSITION> multi_device_{};
   state_path paths_{};
   size_t current_state_{0};
};

GRAPH_DSL_NS_END

#endif //GRAPH_MULTI_DEVICE_GRAPH_H
