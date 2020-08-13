//
// Created by Darwin Yuan on 2020/8/13.
//

#ifndef GRAPH_DSL_FOLD_H
#define GRAPH_DSL_FOLD_H

#include <type-list/TypeList.h>

namespace detail {
   template<template<typename, typename> typename OP, typename ... Ts>
   struct FoldL;

   template<template<typename, typename> typename OP, typename ACC, typename ... Ts>
   struct FoldL<OP, ACC, Ts...> {
      using type = ACC;
   };

   template<template<typename, typename> typename OP, typename ACC, typename H, typename ... Ts>
   struct FoldL<OP, ACC, H, Ts...> {
      using type = typename FoldL<OP, typename OP<ACC, H>::type, Ts...>::type;
   };
}

template<template <typename, typename> typename OP, typename TL>
struct FoldL {
   template<typename ... Ts>
   using fold = typename detail::FoldL<OP, Ts...>::type;
   using type = typename TL::template exportTo<fold>;
};

template<template <typename, typename> typename OP, typename TL>
using FoldL_tl = typename FoldL<OP, TL>::type;

template<template <typename, typename> typename OP, typename ... Ts>
using FoldL_t = typename detail::FoldL<OP, Ts...>::type;

#endif //GRAPH_DSL_FOLD_H
