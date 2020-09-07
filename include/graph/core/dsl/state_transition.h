//
// Created by Darwin Yuan on 2020/8/20.
//

#ifndef GRAPH_STATE_TRANSITION_H
#define GRAPH_STATE_TRANSITION_H

#include <graph/graph_ns.h>
#include <graph/core/dsl/target_state_selector.h>
#include <graph/core/transition_shortcut_search.h>
#include <graph/core/root_state.h>
#include <maco/basic.h>
#include <maco/map.h>
#include <holo/holo.h>
#include <optional>

GRAPH_DSL_NS_BEGIN

struct state_path {
   root_state const* state{nullptr};
   size_t size{0};

   auto get_last() const noexcept -> std::optional<root_state> {
      if(size == 0) return std::nullopt;
      return state[size - 1];
   }

   auto cleanup() {
      state = nullptr;
      size  = 0;
   }
};

template<typename ... Ts> struct transition_trait;

template<typename FROM, typename TO1, typename ... TOs>
struct transition_trait<auto (FROM) -> TO1, TOs...> {
   using from_state = FROM;
   using to_state = device_state<TO1, TOs...>;
};

template<typename = void, typename ... TRANS>
struct state_transitions {
   constexpr static auto All_Direct_Transitions =
      __HOLO_make_tuple(__HOLO_make_pair(holo::type_c<typename TRANS::from_state>, holo::type_c<typename TRANS::to_state>)...);

private:
   template<typename ... STATES>
   struct to_path {
      constexpr static auto Num_Of_States = sizeof...(STATES);
      constexpr static root_state States[] = {
         STATES::Root_State...
      };
      constexpr static state_path Path{States, Num_Of_States};
   };

public:
   constexpr static auto All_Transitions_Paths =
        holo::product( holo::unique(holo::type_list_t<typename TRANS::from_state ...>),
                       holo::unique(holo::type_list_t<typename TRANS::to_state ...>))
      | holo::remove_if( [](auto elem) {
           return holo::typeof_c(holo::first(elem)) == holo::typeof_c(holo::second(elem)); })
      | holo::transform([](auto elem) {
            auto shortcut = state_transition_algo::find_shortcut(elem, All_Direct_Transitions);
            return holo::make_type_pair(holo::make_type_pair(holo::typeof_c(holo::first(elem)), holo::typeof_c(holo::second(elem))), shortcut); })
      | holo::remove_if([](auto const& elem) {
            return holo::length(holo::second(elem)) == holo::size_c<0>; })
      | holo::transform([](auto const& elem) {
            using path = holo::type_transform_t<decltype(holo::second(elem)), to_path>;
            return holo::make_type_pair(holo::first(elem), path{}); });

public:
   template<typename FROM, typename TO, typename PATH>
   static auto matches(__HOLO_pair<__HOLO_pair<FROM, TO>, PATH>, const root_state& from, const root_state& to, state_path& path) {
      if((FROM::Root_State == from) && (TO::Root_State == to)) {
         path = PATH::Path;
         return true;
      }
      return false;
   }

   template<typename ... Ts>
   inline static auto find(const root_state& from, const root_state& to, state_path& path, __HOLO_tuple<Ts...>) {
      return (matches(Ts{}, from, to, path) || ...);
   }

public:
   static auto find(const root_state& from, const root_state& to) -> state_path {
      state_path path{};
      find(from, to, path, All_Transitions_Paths);
      return path;
   }
};

GRAPH_DSL_NS_END

#define __graph_StAtE_transform_2(...)       auto ( GRAPH_DSL_NS::device_state<__VA_ARGS__> )
#define __graph_StAtE_transform(...)         GRAPH_DSL_NS::transition_trait< __graph_StAtE_transform_2 __VA_ARGS__ >
#define __graph_StAtE_each_transition(x)     , __graph_StAtE_transform x
#define __g_STATE_TRANSITIONS(...) \
GRAPH_DSL_NS::state_transitions<void __MACO_map(__graph_StAtE_each_transition, __VA_ARGS__)>

#endif //GRAPH_STATE_TRANSITION_H
