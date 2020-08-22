//
// Created by Darwin Yuan on 2020/8/18.
//

#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include <graph/graph_ns.h>
#include <graph/core/dsl/sub_graph_selector.h>
#include <graph/util/assertion.h>
#include <graph/function/tuple_foreach.h>

GRAPH_DSL_NS_BEGIN

template<typename ROOTS, typename ... SUB_GRAPH_SELECTOR>
struct graph {
   auto refresh(graph_context& context) -> status_t {
      GRAPH_EXPECT_SUCC(build(context));
      GRAPH_EXPECT_SUCC(start(context));
      return status_t::Ok;
   }

   auto stop() {
      roots_.stop();
      tuple_foreach_void(sub_graphs_, [](auto& sub){ sub.stop(); });
   }

   template<size_t I>
   inline auto get_root() -> decltype(auto) {
      return (std::get<I>(roots_.roots_).get_handle());
   }

private:
   inline auto build(graph_context& context) -> status_t {
      GRAPH_EXPECT_SUCC(roots_.build(context));
      return tuple_foreach(sub_graphs_, [&](auto& sub) {
         return sub.build(context);
      });
   }

   auto start(graph_context& context) -> status_t {
      roots_.cleanup();
      tuple_foreach_void(sub_graphs_, [](auto& sub){ sub.cleanup(); });
      GRAPH_EXPECT_SUCC(tuple_foreach(sub_graphs_, [&](auto& sub) {
         return sub.start(context);
      }));
      GRAPH_EXPECT_SUCC(tuple_foreach(roots_.roots_, [&](auto& root) {
         if(!root.present()) return status_t::Ok;
         auto ports = std::make_unique<root_ports>();
         auto status = tuple_foreach(sub_graphs_, [&](auto& sub) {
            return sub.connect_root(context, root, *ports);
         });
         if(status != status_t::Ok) return status;
         return root.update_ports(context, std::move(ports));
      }));

      return status_t::Ok;
   }

private:
   template<typename T>
   using selector = typename sub_graph_selector<T>::template instance_type<typename ROOTS::type>;

private:
   ROOTS roots_;
   std::tuple<selector<SUB_GRAPH_SELECTOR>...> sub_graphs_;
};

GRAPH_DSL_NS_END

#define __sUb_gRaPh_each_condition(n, x) , auto x
#define __sUb_gRaPh_conditions(roots, ...) \
__g_ROOTS roots __CUB_overload(__CUB_repeat_call_, __VA_ARGS__) (__sUb_gRaPh_each_condition, 0, __VA_ARGS__)

#define __g_GRAPH(...) GRAPH_DSL_NS::graph<__sUb_gRaPh_conditions(__VA_ARGS__)>


#endif //GRAPH_GRAPH_H
