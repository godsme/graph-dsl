//
// Created by Darwin Yuan on 2020/8/20.
//

#ifndef GRAPH_TRANSITION_SHORTCUT_SEARCH_H
#define GRAPH_TRANSITION_SHORTCUT_SEARCH_H

#include <graph/graph_ns.h>
#include <graph/core/dsl/target_state_selector.h>
#include <holo/holo.h>
#include <holo/types/typeof_c.h>

GRAPH_DSL_NS_BEGIN

class state_transition_algo {
   template<typename FROM, typename TARGET, typename REST, typename TRANSITIONS>
   constexpr static auto
   search_next_layer(FROM const &from, TARGET const &target, TRANSITIONS const &transitions, REST const &rest) {
      auto all_non_empty_paths =
         transitions
         | holo::transform([&](auto const &elem) {
            return find_shortcut(holo::second(elem), target, rest); })
         | holo::remove_if([](auto const &elem) {
            return holo::length(elem) == holo::size_c<0>; });

      if constexpr (holo::length(all_non_empty_paths) == holo::size_c<0>) {
         return __HOLO_tuple_t<>;
      } else if constexpr (holo::length(all_non_empty_paths) == holo::size_c<1>) {
         return all_non_empty_paths | holo::head() | holo::prepend(from);
      } else {
         return all_non_empty_paths
                | holo::sort([](auto const &l, auto const &r) {
                   return holo::length(l) < holo::length(r); })
                | holo::head()
                | holo::prepend(from);
      }
   }

   template<typename FROM, typename TARGET, typename REST, typename TRANSITIONS>
   constexpr static auto
   find_shortcut_(FROM const &from, TARGET const &target, TRANSITIONS const& direct_transition, REST const &rest) {
      auto result =
         direct_transition
         | holo::find_if([&](auto elem) {
            return holo::typeof_c(holo::second(elem)) == holo::typeof_c(target);
         });

      if constexpr (!holo::is_nothing(result)) {
         // we got the shortcut
         return __HOLO_make_tuple(from, holo::second(result));
      } else {
         return search_next_layer(from, target, direct_transition, rest);
      }
   }

public:
   template<typename FROM, typename TARGET, typename REST>
   constexpr static auto find_shortcut(FROM const& from, TARGET const& target, REST const& rest) {
      auto parts =
         rest
         | holo::partition([&](auto elem) {
            return holo::typeof_c(holo::first(elem)) == holo::typeof_c(from);
         });

      if constexpr (holo::Is_True_V<decltype(holo::length(holo::first(parts)) == holo::size_c<0>)>) {
         return __HOLO_tuple_t<>;
      } else {
         return find_shortcut_(from, target, holo::first(parts), holo::second(parts));
      }
   }

   template<typename TRANS_PAIR, typename REST>
   constexpr static auto find_shortcut(TRANS_PAIR const& trans, REST const& rest) {
      return find_shortcut(holo::first(trans), holo::second(trans), rest);
   }
};

GRAPH_DSL_NS_END

#endif //GRAPH_TRANSITION_SHORTCUT_SEARCH_H
