//
// Created by Darwin Yuan on 2020/8/21.
//

#ifndef GRAPH_MULTI_DEVICE_GRAPH_H
#define GRAPH_MULTI_DEVICE_GRAPH_H

#include <graph/graph_ns.h>
#include <graph/util/assertion.h>
#include <graph/core/GraphContext.h>
#include <graph/core/dsl/multi_devices.h>

GRAPH_DSL_NS_BEGIN

template<typename GRAPH, typename STATE_SELECTOR, typename STATE_TRANSITION>
struct multi_device_graph : private GRAPH {
   template<typename ENV>
   auto on_env_change(GraphContext& context, ENV const& env) -> status_t {
      paths_ = multi_device_.GetTransitions(env);

      if(paths_.size > 0) {
         current_state_ = paths_.size == 1 ? 0 : 1;
         context.UpdateRootState(paths_.state[current_state_]);
         return GRAPH::refresh(context);
      } else {
         current_state_ = 0;
      }

      return status_t::Ok;
   }

   auto on_switch_done(GraphContext& context) -> status_t {
      if(current_state_ < paths_.size) {
          multi_device_.UpdateState(paths_.state[current_state_]);
         if((++current_state_) < paths_.size) {
            context.UpdateRootState(paths_.state[current_state_]);
            return GRAPH::refresh(context);
         }
      }
      return status_t::Ok;
   }

   auto stop() {
      GRAPH::stop();
      multi_device_.cleanup();
      current_state_ = 0;
       paths_.CleanUp();
   }

   using GRAPH::get_root;
   using GRAPH::refresh;

private:
   MultiDevice<STATE_SELECTOR, STATE_TRANSITION> multi_device_{};
   StatePath paths_{};
   size_t current_state_{0};
};

GRAPH_DSL_NS_END

#endif //GRAPH_MULTI_DEVICE_GRAPH_H
