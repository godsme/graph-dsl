//
// Created by Darwin Yuan on 2020/8/19.
//

#ifndef GRAPH_TARGET_STATE_SELECTOR_H
#define GRAPH_TARGET_STATE_SELECTOR_H

#include <graph/graph_ns.h>
#include <graph/util/interval.h>
#include <graph/core/dsl/device_state.h>
#include <holo/algo/sort.h>
#include <holo/types/size_c.h>
#include <holo/types/tuple_trait.h>

GRAPH_DSL_NS_BEGIN

template<uint8_t SCENE>
struct scene_mode {
   template<typename ENV>
   static auto matches(const ENV& env) -> bool {
      return env.get_scene() == SCENE;
   }
};

template<typename INTERVAL>
struct condition_1 {
   template<typename ENV>
   inline static auto matches(ENV const& env) -> bool {
      return INTERVAL::contains(env.get_condition_1());
   }
};

template<typename INTERVAL>
struct condition_2 {
   template<typename ENV>
   inline static auto matches(ENV const& env) -> bool {
      return INTERVAL::contains(env.get_condition_2());
   }
};

template<typename INTERVAL>
struct condition_3 {
   template<typename ENV>
   inline static auto matches(ENV const& env) -> bool {
      return INTERVAL::contains(env.get_condition_3());
   }
};

///////////////////////////////////////////////////////////////////////////////
template<typename ... CONDs>
struct state_select_condition {
   constexpr static size_t Num_Of_Conditions = sizeof...(CONDs);
   template<typename ENV>
   inline static auto matches(ENV const& env) -> bool {
      return (CONDs::matches(env) && ...);
   }
};

///////////////////////////////////////////////////////////////////////////////
namespace detail {
   template<typename PAIR>
   struct target_state_entry;

   template<typename COND, typename STATE>
   struct target_state_entry<auto (COND) -> STATE> {
      constexpr static size_t Num_Of_Conditions = COND::Num_Of_Conditions;

      template<typename ENV>
      inline static auto return_if_matches(ENV const& env, root_state& result) -> bool {
         if(!COND::matches(env)) return false;
         result = STATE::Root_State;
         return true;
      }
   };
}


///////////////////////////////////////////////////////////////////////////////
template<typename ... ENTRIES>
struct target_state_selector {
private:
   constexpr static auto Sorted_Entries =
        holo::tuple_t<detail::target_state_entry<ENTRIES>...>
      | holo::sort([](auto l, auto r) {
          return holo::size_c<decltype(l)::type::Num_Of_Conditions> > holo::size_c<decltype(r)::type::Num_Of_Conditions>;
        });

public:
   template<typename ... ALL_ENTRIES>
   struct entries_type {
      template<typename ENV>
      static auto find(const ENV& env) {
         root_state result;
         (ALL_ENTRIES::return_if_matches(env, result) || ...);
         return result;
      }
   };

   using sorted_entries = holo::tuple_trait_t<decltype(Sorted_Entries), entries_type>;

   template<typename ENV>
   inline static auto find(ENV const& env) -> root_state {
      return sorted_entries::find(env);
   }
};

template<typename, typename ... ENTRIES>
using target_state_selector_t = target_state_selector<ENTRIES...>;

GRAPH_DSL_NS_END

#define __g_SCENE_MODE(value)   GRAPH_DSL_NS::scene_mode<value>
#define __g_COND_1(...)         GRAPH_DSL_NS::condition_1<__CUB_interval(__VA_ARGS__)>
#define __g_COND_2(...)         GRAPH_DSL_NS::condition_2<__CUB_interval(__VA_ARGS__)>
#define __g_COND_3(...)         GRAPH_DSL_NS::condition_3<__CUB_interval(__VA_ARGS__)>

#define __gRaPh_when(...)           auto (GRAPH_DSL_NS::state_select_condition<__VA_ARGS__>)
#define __gRaPh_each_case(n, x) , __gRaPh_when x
#define __gRaPh_state_selector(...) \
void __CUB_overload(__CUB_repeat_call_, __VA_ARGS__) (__gRaPh_each_case, 0, __VA_ARGS__)

#define __g_STATE_SELECTOR(...) \
GRAPH_DSL_NS::target_state_selector_t<__gRaPh_state_selector(__VA_ARGS__)>

#endif //GRAPH_TARGET_STATE_SELECTOR_H
