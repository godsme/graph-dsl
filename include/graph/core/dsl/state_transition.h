//
// Created by Darwin Yuan on 2020/8/20.
//

#ifndef GRAPH_STATE_TRANSITION_H
#define GRAPH_STATE_TRANSITION_H

#include <graph/graph_ns.h>
#include <graph/util/hana_tuple_trait.h>
#include <graph/function/unique.h>
#include <boost/hana.hpp>
#include <graph/core/dsl/target_state_selector.h>
#include <nano-caf/util/macro_basic.h>
#include <nano-caf/util/macro_pp_size.h>
#include <nano-caf/util/macro_struct.h>
#include <nano-caf/util/macro_reflex_call.h>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

template<typename ... Ts> struct transition_trait;

template<typename FROM, typename TO1, typename ... TOs>
struct transition_trait<auto (FROM) -> TO1, TOs...> {
   using from_state = FROM;
   using to_state = device_state<TO1, TOs...>;
};

template<typename = void, typename ... TRANS>
struct state_transitions {
   constexpr static auto All_Possible_Transitions = hana::remove_if(
      hana::ap(hana::make_tuple(hana::make_pair),
         unique(hana::tuple_t<typename TRANS::from_state ...>),
         unique(hana::tuple_t<typename TRANS::to_state ...>)),
      [](auto const& elem) { return hana::first(elem) == hana::second(elem); });
   constexpr static auto All_Direct_Transitions =
      hana::make_tuple(hana::make_pair(hana::type_c<typename TRANS::from_state>, hana::type_c<typename TRANS::to_state>)...);

   template<typename TRANSITION, typename REST>
   constexpr static auto find_shortcut(TRANSITION const& transition, REST const& rest) {
      auto parts = hana::partition(rest, [&](auto const& elem) {
         using from_type = typename std::decay_t<decltype(hana::first(transition))>::type;
         return std::decay_t<decltype(hana::first(elem))>::type::template equals<from_type>();
      });
      constexpr auto result = hana::find_if(hana::first(parts), [&](auto const& elem) {
         using target = typename std::decay_t<decltype(hana::second(elem))>::type;
         return std::decay_t<decltype(hana::second(transition))>::type::template equals<target>();
      });
      if constexpr (hana::is_nothing(result)) {
         return hana::make_pair(false, parts);
      } else {
         return hana::make_pair(true, *result);
      }
   }
};

GRAPH_DSL_NS_END

#define __graph_StAtE_transform_2(...)   auto ( GRAPH_DSL_NS::device_state<__VA_ARGS__> )
#define __graph_StAtE_transform(...)     GRAPH_DSL_NS::transition_trait< __graph_StAtE_transform_2 __VA_ARGS__ >
#define __graph_StAtE_each_transition(n, x) , __graph_StAtE_transform x
#define __g_STATE_TRANSITIONS(...) \
GRAPH_DSL_NS::state_transitions<void __CUB_overload(__CUB_repeat_call_, __VA_ARGS__) (__graph_StAtE_each_transition, 0, __VA_ARGS__)>

#endif //GRAPH_STATE_TRANSITION_H
