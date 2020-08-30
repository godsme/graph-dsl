//
// Created by Darwin Yuan on 2020/8/20.
//

#ifndef GRAPH_TRANSITION_SHORTCUT_SEARCH_H
#define GRAPH_TRANSITION_SHORTCUT_SEARCH_H

#include <graph/graph_ns.h>
#include <graph/core/dsl/target_state_selector.h>
#include <holo/holo.h>

GRAPH_DSL_NS_BEGIN

   template<typename T> struct S;
class state_transition_algo {
   template<typename FROM, typename TARGET, typename REST, typename TRANSITIONS>
   constexpr static auto
   search_next_layer(FROM const &from, TARGET const &target, TRANSITIONS const &transitions, REST const &rest) {
      auto all_paths = holo::transform([&](auto const &elem) {
         return find_shortcut(holo::second(elem), target, rest);
      }, transitions);

      auto all_non_empty_paths = holo::remove_if([](auto const &elem) {
         return holo::length(elem) == holo::size_c<0>;
      }, all_paths);

      if constexpr (holo::length(all_non_empty_paths) == holo::size_c<0>) {
         return __HOLO_tuple_t<>;
      } else if constexpr (holo::length(all_non_empty_paths) == holo::size_c<1>) {
         return holo::prepend(from, holo::head(all_non_empty_paths));
      } else {
         return holo::prepend(from, holo::head(
            holo::sort(
               [](auto const &l, auto const &r) {
                  return holo::length(l) < holo::length(r);
               }, all_non_empty_paths)));
      }
   }

   template<typename FROM, typename TARGET, typename REST, typename TRANSITIONS>
   constexpr static auto
   find_shortcut_(FROM const &from, TARGET const &target, TRANSITIONS const& direct_transition, REST const &rest) {
      auto result = holo::find_if([&](auto elem) {
         using to = typename std::decay_t<decltype(holo::second(elem))>::type;
         return std::decay_t<decltype(target)>::type::template equals<to>();
      }, direct_transition);

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
      auto parts = holo::partition([&](auto elem) {
         using from_type = typename std::decay_t<decltype(from)>::type;
         return std::decay_t<decltype(holo::first(elem))>::type::template equals<from_type>();
      }, rest);
      if constexpr (holo::length(holo::first(parts)) == holo::size_c<0>) {
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
