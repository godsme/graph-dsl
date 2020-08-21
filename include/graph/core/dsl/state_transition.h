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
#include <graph/core/transition_shortcut_search.h>
#include <nano-caf/util/macro_basic.h>
#include <nano-caf/util/macro_pp_size.h>
#include <nano-caf/util/macro_struct.h>
#include <nano-caf/util/macro_reflex_call.h>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

struct state_path {
   root_state const* state;
   size_t size;
};

template<typename ... Ts> struct transition_trait;

template<typename FROM, typename TO1, typename ... TOs>
struct transition_trait<auto (FROM) -> TO1, TOs...> {
   using from_state = FROM;
   using to_state = device_state<TO1, TOs...>;
};

template<typename = void, typename ... TRANS>
struct state_transitions {
private:
   constexpr static auto All_Possible_Transitions = hana::remove_if(
      hana::ap(hana::make_tuple(hana::make_pair),
         unique(hana::tuple_t<typename TRANS::from_state ...>),
         unique(hana::tuple_t<typename TRANS::to_state ...>)),
      [](auto const& elem) { return hana::first(elem) == hana::second(elem); });

   constexpr static auto Sorted_Possible_Transitions = hana::sort(All_Possible_Transitions, [](auto l, auto r){
      using l_from = typename std::decay_t<decltype(hana::first(l))>::type;
      using r_from = typename std::decay_t<decltype(hana::first(r))>::type;

      if constexpr(l_from::template less_than<r_from>()) {
         return std::integral_constant<bool, true>{};
      }
      else if constexpr(l_from::template equals<r_from>()) {
         using l_to = typename std::decay_t<decltype(hana::second(l))>::type;
         using r_to = typename std::decay_t<decltype(hana::second(r))>::type;
         return l_to::template less_than<r_to>();
      } else {
         return std::integral_constant<bool, false>{};
      }
   });

   template<typename ... STATES>
   struct to_path {
      constexpr static auto Num_Of_States = sizeof...(STATES);
      constexpr static root_state States[] = {
         STATES::Root_State...
      };
      constexpr static state_path Path{States, Num_Of_States};
   };

public:
   constexpr static auto All_Direct_Transitions =
      hana::make_tuple(hana::make_pair(hana::type_c<typename TRANS::from_state>, hana::type_c<typename TRANS::to_state>)...);

private:
   using key_type = std::pair<root_state, root_state>;
   using elem_type = std::pair<key_type, state_path>;

   constexpr static auto All_Transitions_Paths =
      hana::fold_left(hana::transform(hana::remove_if(hana::transform(Sorted_Possible_Transitions,
   [](auto const& elem) {
      auto from_state = std::decay_t<decltype(hana::first(elem))>::type::Root_State;
      auto to_state   = std::decay_t<decltype(hana::second(elem))>::type::Root_State;
      return hana::make_pair(std::make_pair(from_state, to_state), state_transition_algo::find_shortcut(elem, All_Direct_Transitions));
   }),
   [](auto const& elem) {
      return hana::size(hana::second(elem)) == hana::size_c<0>;
   }),
   [](auto const& elem) {
      using path = hana_tuple_trait_t<decltype(hana::second(elem)), to_path>;
      return std::make_pair(hana::first(elem), path::Path);
   }), std::tuple<>{},
   [](auto const& acc, auto const& elem) {
       return std::tuple_cat(acc, std::make_tuple(elem));
   });

   static auto matches(const elem_type& elem, const root_state& from, const root_state& to, state_path& path) {
      auto& [key, value] = elem;
      if((key.first == from) && (key.second == to)) {
         path = value;
         return true;
      }
      return false;
   }

   template<size_t ... I>
   inline static auto find(const root_state& from, const root_state& to, state_path& path, std::index_sequence<I...>) {
      return (matches(std::get<I>(All_Transitions_Paths), from, to, path) || ...);
   }

   constexpr static auto sequence = std::make_index_sequence<std::tuple_size_v<decltype(All_Transitions_Paths)>>{};
public:
   static auto find(const root_state& from, const root_state& to) -> state_path {
      state_path path{};
      find(from, to, path, sequence);
      return path;
   }
};

GRAPH_DSL_NS_END

#define __graph_StAtE_transform_2(...)   auto ( GRAPH_DSL_NS::device_state<__VA_ARGS__> )
#define __graph_StAtE_transform(...)     GRAPH_DSL_NS::transition_trait< __graph_StAtE_transform_2 __VA_ARGS__ >
#define __graph_StAtE_each_transition(n, x) , __graph_StAtE_transform x
#define __g_STATE_TRANSITIONS(...) \
GRAPH_DSL_NS::state_transitions<void __CUB_overload(__CUB_repeat_call_, __VA_ARGS__) (__graph_StAtE_each_transition, 0, __VA_ARGS__)>

#endif //GRAPH_STATE_TRANSITION_H
