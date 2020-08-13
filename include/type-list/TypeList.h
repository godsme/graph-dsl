//
// Created by Darwin Yuan on 2020/8/13.
//

#ifndef GRAPH_DSL_TYPELIST_H
#define GRAPH_DSL_TYPELIST_H

template<typename ... Ts>
struct TypeList {

   template<typename TL>
   using concat = typename TL::template append<Ts...>;

   template<typename ... Ts2>
   using append = TypeList<Ts..., Ts2...>;

   template<typename T>
   using appendType = TypeList<Ts..., T>;

   template<template<typename ...> typename R>
   using exportTo = R<Ts...>;
};

#endif //GRAPH_DSL_TYPELIST_H
