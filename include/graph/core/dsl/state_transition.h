//
// Created by Darwin Yuan on 2020/8/20.
//

#ifndef GRAPH_STATE_TRANSITION_H
#define GRAPH_STATE_TRANSITION_H

#include <graph/graph_ns.h>
#include <graph/core/dsl/target_state_selector.h>
#include <graph/core/TransitionShortcutSearch.h>
#include <graph/core/RootState.h>
#include <maco/basic.h>
#include <maco/map.h>
#include <holo/holo.h>
#include <optional>

GRAPH_DSL_NS_BEGIN

struct StatePath {
   RootState const* state{nullptr};
   size_t size{0};

   auto GetLast() const noexcept -> std::optional<RootState> {
      if(size == 0) return std::nullopt;
      return state[size - 1];
   }

   auto CleanUp() {
      state = nullptr;
      size  = 0;
   }
};

template<typename ... Ts> struct TransitionTrait;

template<typename FROM, typename TO1, typename ... TOs>
struct TransitionTrait<auto (FROM) -> TO1, TOs...> {
   using from_state = FROM;
   using to_state = DeviceState<TO1, TOs...>;
};

template<typename = void, typename ... TRANS>
struct StateTransitions {
   constexpr static auto All_Direct_Transitions =
      holo::make_list(holo::pair_t<typename TRANS::from_state, typename TRANS::to_state>...);

private:
   template<typename ... STATES>
   struct to_path {
      constexpr static auto Num_Of_States = sizeof...(STATES);
      constexpr static RootState States[] = {
         STATES::Root_State...
      };
      constexpr static StatePath Path{States, Num_Of_States};
   };

public:
   constexpr static auto All_Transitions_Paths =
        holo::product( holo::unique(holo::list_t<typename TRANS::from_state ...>),
                       holo::unique(holo::list_t<typename TRANS::to_state ...>))
      | holo::remove_if( [](auto elem) {
           return holo::typeof_c(holo::first(elem)) == holo::typeof_c(holo::second(elem)); })
      | holo::transform([](auto elem) {
            auto shortcut = StateTransitionAlgo::FindShortcut(elem, All_Direct_Transitions);
            return holo::make_pair(holo::make_pair(holo::typeof_c(holo::first(elem)), holo::typeof_c(holo::second(elem))), shortcut); })
      | holo::remove_if([](auto const& elem) {
            return holo::length(holo::second(elem)) == holo::size_c<0>; })
      | holo::transform([](auto const& elem) {
            using path = holo::type_transform_t<decltype(holo::second(elem)), to_path>;
            return holo::make_pair(holo::first(elem), path{}); });

public:
   template<typename FROM, typename TO, typename PATH>
   static auto matches(holo::type_pair<holo::type_pair<FROM, TO>, PATH>, const RootState& from, const RootState& to, StatePath& path) {
      if((FROM::Root_State == from) && (TO::Root_State == to)) {
         path = PATH::Path;
         return true;
      }
      return false;
   }

   template<typename ... Ts>
   inline static auto find(const RootState& from, const RootState& to, StatePath& path, holo::type_list<Ts...>) {
      return (matches(Ts{}, from, to, path) || ...);
   }

public:
   static auto find(const RootState& from, const RootState& to) -> StatePath {
      StatePath path{};
      find(from, to, path, All_Transitions_Paths);
      return path;
   }
};

GRAPH_DSL_NS_END

#define __graph_StAtE_transform_2(...)       auto ( GRAPH_DSL_NS::DeviceState<__VA_ARGS__> )
#define __graph_StAtE_transform(...)         GRAPH_DSL_NS::TransitionTrait< __graph_StAtE_transform_2 __VA_ARGS__ >
#define __graph_StAtE_each_transition(x)     , __graph_StAtE_transform x
#define __g_STATE_TRANSITIONS(...) \
GRAPH_DSL_NS::StateTransitions<void __MACO_map(__graph_StAtE_each_transition, __VA_ARGS__)>

#endif //GRAPH_STATE_TRANSITION_H
