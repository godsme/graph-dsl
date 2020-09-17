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
   template<typename FROM, typename TARGET, typename REST, typename TRANSITIONS, std::size_t DEPTH>
   constexpr static auto
   search_next_layer(FROM const &from, TARGET const &target, TRANSITIONS const &transitions, REST const &rest, holo::size_c_t<DEPTH>) {
      auto result = transitions | holo::fold_left(holo::list_t<>, [&](auto shortcut, auto elem) {
         auto size = holo::length(shortcut);
         constexpr auto search_depth = size == 0 ? DEPTH : size - 1;
         auto result = find_shortcut(holo::second(elem), target, rest, holo::size_c<search_depth>);
         if constexpr (holo::length(result) == 0) {
            return shortcut;
         } else {
            return result;
         }
      });

      if constexpr (holo::length(result) == holo::size_c<0>) {
         return holo::list_t<>;
      } else {
         return result | holo::prepend(from);
      }
   }

   template<typename FROM, typename TARGET, typename REST, typename TRANSITIONS, std::size_t DEPTH>
   constexpr static auto
   find_shortcut_(FROM const &from, TARGET const &target, TRANSITIONS const& direct_transition, REST const &rest, holo::size_c_t<DEPTH>) {
      auto result = direct_transition | holo::find_if([&](auto elem) {
            return holo::typeof_c(holo::second(elem)) == holo::typeof_c(target);
         });

      if constexpr (!holo::is_nothing(result)) {
         // we got the shortcut
         return holo::make_list(from, holo::second(result));
      } else {
         return search_next_layer(from, target, direct_transition, rest, holo::size_c<DEPTH - 1>);
      }
   }

   template<typename TRANS_PAIR, typename REST, std::size_t DEPTH>
   constexpr static auto find_shortcut(TRANS_PAIR const& trans, REST const& rest, holo::size_c_t<DEPTH>) {
      return find_shortcut(holo::first(trans), holo::second(trans), rest, holo::size_c<DEPTH>);
   }


   template<typename FROM, typename TARGET, typename REST, std::size_t DEPTH>
   constexpr static auto find_shortcut(FROM const& from, TARGET const& target, REST const& rest, holo::size_c_t<DEPTH>) {
      if constexpr (DEPTH == 0) {
         return holo::list_t<>;
      } else {
         // find the direct neighbors
         auto parts = rest | holo::partition([&](auto elem) {
            return holo::typeof_c(holo::first(elem)) == holo::typeof_c(from);
         });

         if constexpr (holo::Is_True_V<decltype(holo::length(holo::first(parts)) == holo::size_c<0>)>) {
            return holo::list_t<>;
         } else {
            return find_shortcut_(from, target, holo::first(parts), holo::second(parts), holo::size_c<DEPTH>);
         }
      }
   }

public:
   template<typename FROM, typename TARGET, typename REST>
   constexpr static auto find_shortcut(FROM const& from, TARGET const& target, REST const& rest) {
      return find_shortcut(from, target, rest, holo::max_size_c);
   }

   template<typename TRANS_PAIR, typename REST>
   constexpr static auto find_shortcut(TRANS_PAIR const& trans, REST const& rest) {
      return find_shortcut(holo::first(trans), holo::second(trans), rest, holo::max_size_c);
   }
};

GRAPH_DSL_NS_END

#endif //GRAPH_TRANSITION_SHORTCUT_SEARCH_H
