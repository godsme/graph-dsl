//
// Created by Darwin Yuan on 2020/8/20.
//

#ifndef GRAPH_TRANSITION_SHORTCUT_SEARCH_H
#define GRAPH_TRANSITION_SHORTCUT_SEARCH_H

#include <graph/graph_ns.h>
#include <graph/util/hana_tuple_trait.h>
#include <graph/function/unique.h>
#include <boost/hana.hpp>
#include <graph/core/dsl/target_state_selector.h>
#include <iostream>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

class state_transition_algo {
   template<typename FROM, typename TARGET, typename REST, typename TRANSITIONS>
   constexpr static auto
   search_next_layer(FROM const &from, TARGET const &target, REST const &rest, TRANSITIONS const &transitions) {
      auto all_paths = hana::transform(transitions, [&](auto const &elem) {
         return find_shortcut(hana::second(elem), target, rest);
      });

      auto all_non_empty_paths = hana::remove_if(all_paths, [](auto const &elem) {
         return hana::size(elem) == hana::size_c<0>;
      });

      if constexpr (hana::size(all_non_empty_paths) == hana::size_c<0>) {
         return hana::tuple_t<>;
      } else if constexpr (hana::size(all_non_empty_paths) == hana::size_c<1>) {
         return hana::prepend(hana::front(all_non_empty_paths), from);
      } else {
         return hana::prepend(hana::front(hana::sort(all_non_empty_paths, [](auto const &l, auto const &r) {
            return hana::size(l) < hana::size(r);
         })), from);
      }
   }

   template<typename FROM, typename TARGET, typename REST, typename TRANSITIONS>
   constexpr static auto
   find_shortcut_(FROM const &from, TARGET const &target, TRANSITIONS const& direct_transition, REST const &rest) {
      auto result = hana::find_if(direct_transition, [&](auto const &elem) {
         using this_target = typename std::decay_t<decltype(hana::second(elem))>::type;
         return std::decay_t<decltype(target)>::type::template equals<this_target>();
      });
      if constexpr (hana::is_just(result)) {
         // we got the shortcut
         return hana::make_tuple(from, hana::second(*result));
      } else {
         return search_next_layer(from, target, rest, direct_transition);
      }
   }

public:
   template<typename FROM, typename TARGET, typename REST>
   constexpr static auto find_shortcut(FROM const& from, TARGET const& target, REST const& rest) {
      auto parts = hana::partition(rest, [&](auto const& elem) {
         using from_type = typename std::decay_t<decltype(from)>::type;
         return std::decay_t<decltype(hana::first(elem))>::type::template equals<from_type>();
      });
      if constexpr (hana::size(hana::first(parts)) == hana::size_c<0>) {
         return hana::tuple_t<>;
      } else {
         return find_shortcut_(from, target, hana::first(parts), hana::second(parts));
      }
   }
};

GRAPH_DSL_NS_END

#endif //GRAPH_TRANSITION_SHORTCUT_SEARCH_H
