//
// Created by Darwin Yuan on 2020/8/18.
//

#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include <graph/graph_ns.h>
#include <graph/core/dsl/sub_graph_selector.h>
#include <graph/util/assertion.h>
#include <graph/function/tuple_foreach.h>
#include <maco/map.h>

GRAPH_DSL_NS_BEGIN

template<typename ROOTS, typename ... SUB_GRAPH_SELECTOR>
struct Graph {
   auto Refresh(GraphContext& context) -> Status {
      GRAPH_EXPECT_SUCC(Build(context));
      GRAPH_EXPECT_SUCC(Start(context));
      return Status::OK;
   }

   auto Stop() {
      roots_.Stop();
      tuple_foreach_void(sub_graphs_, [](auto& sub){ sub.Stop(); });
   }

   template<size_t I>
   inline auto GetRoot() -> decltype(auto) {
      return (std::get<I>(roots_.roots_).GetHandle());
   }

private:
   inline auto Build(GraphContext& context) -> Status {
      GRAPH_EXPECT_SUCC(roots_.Build(context));
      return tuple_foreach(sub_graphs_, [&](auto& sub) {
         return sub.Build(context);
      });
   }

   auto Start(GraphContext& context) -> Status {
       roots_.CleanUp();
      tuple_foreach_void(sub_graphs_, [](auto& sub){
          sub.CleanUp();
      });
      GRAPH_EXPECT_SUCC(tuple_foreach(sub_graphs_, [&](auto& sub) {
         return sub.Start(context);
      }));
      GRAPH_EXPECT_SUCC(tuple_foreach(roots_.roots_, [&](auto& root) {
         if(!root.Present()) return Status::OK;
         auto ports = std::make_unique<ActorPorts>();
         auto status = tuple_foreach(sub_graphs_, [&](auto& sub) {
            return sub.ConnectRoot(context, root, *ports);
         });
         if(status != Status::OK) return status;
         return root.UpdatePorts(context, std::move(ports));
      }));

      return Status::OK;
   }

private:
   template<typename T>
   using selector = typename sub_graph_selector<T>::template instance_type<typename ROOTS::type>;

private:
   ROOTS roots_;
   std::tuple<selector<SUB_GRAPH_SELECTOR>...> sub_graphs_;
};

GRAPH_DSL_NS_END

#define __sUb_gRaPh_each_condition(x) , auto x
#define __sUb_gRaPh_conditions(roots, ...) \
__g_ROOTS roots __MACO_map(__sUb_gRaPh_each_condition, __VA_ARGS__)

#define __g_GRAPH(...) GRAPH_DSL_NS::Graph<__sUb_gRaPh_conditions(__VA_ARGS__)>

#endif //GRAPH_GRAPH_H

