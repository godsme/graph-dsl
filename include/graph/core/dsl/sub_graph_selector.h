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
      auto Build(GraphContext& context) -> Status {
         return COND::Satisfied(context).with_value([&](auto satisfied) {
            if(satisfied) {
               GRAPH_EXPECT_SUCC(subgraph_.Build(context));
               selected_ = true;
            } else {
               selected_ = false;
            }

            return Status::OK;
         });
      }

      auto Start(GraphContext& context) -> Status {
         if(selected_) {
            GRAPH_EXPECT_SUCC(subgraph_.Start(context));
            alive_ = true;
         }
         return Status::OK;
      }

      auto CleanUp() {
         if(alive_) {
            if(selected_) { subgraph_.CleanUp(); }
            else { Stop(); }
         }
      }

      auto Stop() {
         if(alive_) {
            subgraph_.Stop();
            alive_ = false;
         }
      }

      template <typename ROOT>
      auto ConnectRoot(GraphContext& context, ROOT& root, ActorPorts& ports) -> Status {
         if(selected_) {
            GRAPH_EXPECT_TRUE(alive_);
            return subgraph_.ConnectRoot(context, root, ports);
         }

         return Status::OK;
      }
   private:
      typename SUB_GRAPH::template InstanceType<ROOTS> subgraph_{};
      bool selected_{false};
      bool alive_{false};
   };
};

GRAPH_DSL_NS_END

#endif //GRAPH_SUB_GRAPH_SELECTOR_H
