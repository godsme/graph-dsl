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
   private:
      bool selected_;
      bool alive_;
      typename SUB_GRAPH::template by_roots<ROOTS> subgraph_;
   };
};

GRAPH_DSL_NS_END

#define __g_WHEN(cond) auto(cond)

#endif //GRAPH_SUB_GRAPH_SELECTOR_H
