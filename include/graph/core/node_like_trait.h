//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_NODE_LIKE_TRAIT_H
#define GRAPH_NODE_LIKE_TRAIT_H

#include <graph/graph_ns.h>
#include <boost/hana/basic_tuple.hpp>
#include <hana/include/boost/hana/fwd/tuple.hpp>
#include <hana/include/boost/hana/concat.hpp>
#include <type_traits>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

struct node_signature {};

//////////////////////////////////////////////////////////////////////////////
template<typename NODE, typename = void>
struct node_like_trait;

template<typename NODE>
struct node_like_trait<NODE, std::enable_if_t<std::is_base_of_v<node_signature, NODE>>> {
   constexpr static auto node_list = hana::tuple_t<NODE>;
};

//////////////////////////////////////////////////////////////////////////////
template<typename COND, typename NODE_LIKE>
struct maybe {
   constexpr static auto node_list = node_like_trait<NODE_LIKE>::node_list;
};

template<typename COND, typename NODE_LIKE>
struct node_like_trait<maybe<COND, NODE_LIKE>, void> {
   constexpr static auto node_list = maybe<COND, NODE_LIKE>::node_list;
};

//////////////////////////////////////////////////////////////////////////////
template<typename COND, typename NODE_LIKE_1, typename NODE_LIKE_2>
struct exclusive {
   constexpr static auto node_list = \
      hana::concat
         ( node_like_trait<NODE_LIKE_1>::node_list
         , node_like_trait<NODE_LIKE_2>::node_list);
};

template<typename COND, typename NODE_LIKE_1, typename NODE_LIKE_2>
struct node_like_trait<exclusive<COND, NODE_LIKE_1, NODE_LIKE_2>, void> {
   constexpr static auto node_list = exclusive<COND, NODE_LIKE_1, NODE_LIKE_2>::node_list;
};

//////////////////////////////////////////////////////////////////////////////
template<typename ... NODEs_LIKE>
struct fork {
   constexpr static auto node_list =
      hana::flatten(hana::make_tuple(node_like_trait<NODEs_LIKE>::node_list...));
};

template<typename ... NODEs_LIKE>
struct node_like_trait<fork<NODEs_LIKE...>, void> {
   constexpr static auto node_list = fork<NODEs_LIKE...>::node_list;
};

GRAPH_DSL_NS_END

#define __fork(...) GRAPH_DSL_NS::fork<__VA_ARGS__>
#define __maybe(...) GRAPH_DSL_NS::maybe<__VA_ARGS__>
#define __exclusive(...) GRAPH_DSL_NS::exclusive<__VA_ARGS__>

#endif //GRAPH_NODE_LIKE_TRAIT_H
