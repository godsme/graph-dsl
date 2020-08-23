//
// Created by Darwin Yuan on 2020/8/18.
//

#ifndef GRAPH_SUB_GRAPH_SELECTOR_H
#define GRAPH_SUB_GRAPH_SELECTOR_H

#include <graph/graph_ns.h>

GRAPH_DSL_NS_BEGIN

template<typename T>
struct sub_graph_selector;

template<typename COND, typename SUB_GRAPH>
struct sub_graph_selector<auto (COND) -> SUB_GRAPH> final {
   template<typename ROOTS>
   struct instance_type {
      auto build(graph_context& context) -> status_t {
         return COND{}(context).with_value([&](auto satisfied) {
            if(satisfied) {
               GRAPH_EXPECT_SUCC(subgraph_.build(context));
               selected_ = true;
            } else {
               selected_ = false;
            }

            return status_t::Ok;
         });
      }

      auto start(graph_context& context) -> status_t {
         if(selected_) {
            GRAPH_EXPECT_SUCC(subgraph_.start(context));
            alive_ = true;
         }
         return status_t::Ok;
      }

      auto cleanup() {
         if(alive_) {
            if(!selected_) { subgraph_.cleanup(); }
            else { stop(); }
         }
      }

      auto stop() {
         if(alive_) {
            subgraph_.stop();
            alive_ = false;
         }
      }

      template <typename ROOT>
      auto connect_root(graph_context& context, ROOT& root, actor_ports& ports) -> status_t {
         if(selected_) {
            GRAPH_EXPECT_TRUE(alive_);
            return subgraph_.connect_root(context, root, ports);
         }

         return status_t::Ok;
      }
   private:
      typename SUB_GRAPH::template instance_type<ROOTS> subgraph_{};
      bool selected_{false};
      bool alive_{false};
   };
};

GRAPH_DSL_NS_END

#endif //GRAPH_SUB_GRAPH_SELECTOR_H
