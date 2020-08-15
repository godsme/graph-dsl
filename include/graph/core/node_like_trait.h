//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_NODE_LIKE_TRAIT_H
#define GRAPH_NODE_LIKE_TRAIT_H

#include <graph/graph_ns.h>
#include <graph/util/result_t.h>
#include <graph/core/node_index.h>
#include <boost/hana/basic_tuple.hpp>
#include <boost/hana/fwd/tuple.hpp>
#include <boost/hana/concat.hpp>
#include <type_traits>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

//////////////////////////////////////////////////////////////////////////////
template<typename NODE, typename = void>
struct node_like_trait;



//////////////////////////////////////////////////////////////////////////////
template<typename ... NODEs_LIKE>
struct fork {
   constexpr static auto node_list =
      hana::flatten(hana::make_tuple(node_like_trait<NODEs_LIKE>::type::node_list...));

private:
   template<typename NODE_LIKE>
   static auto enabled(graph_context& context, result_t<bool>& result) -> bool {
      result = node_like_trait<NODE_LIKE>::type::enabled(context);
      return result.value_or_else(true);
   }

public:
   static auto enabled(graph_context& context) -> result_t<bool> {
      result_t result = false;
      (enabled<node_like_trait<NODEs_LIKE>>(context, result) || ...);
      return result;
   }
};

template<typename ... NODEs_LIKE>
struct node_like_trait<fork<NODEs_LIKE...>, void> {
   using type = fork<NODEs_LIKE...>;
};

GRAPH_DSL_NS_END

#define __fork(...) GRAPH_DSL_NS::fork<__VA_ARGS__>
#define __maybe(...) GRAPH_DSL_NS::maybe<__VA_ARGS__>
#define __exclusive(...) GRAPH_DSL_NS::exclusive<__VA_ARGS__>

#endif //GRAPH_NODE_LIKE_TRAIT_H
