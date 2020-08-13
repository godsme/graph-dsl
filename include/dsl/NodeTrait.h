//
// Created by Darwin Yuan on 2020/8/13.
//

#ifndef GRAPH_DSL_NODETRAIT_H
#define GRAPH_DSL_NODETRAIT_H

#include <type-list/Unique.h>

template<typename NODE>
struct NodeTrait {
   using NodeList = TypeList<NODE>;
};

template<typename PRED, typename NODE>
struct Maybe {
   using PredType = PRED;
   using NodeList = Unique_tl<typename NodeTrait<NODE>::NodeList>;
};

template<typename COND, typename NODE>
struct NodeTrait<Maybe<COND, NODE>> {
   using NodeList = typename Maybe<COND, NODE>::NodeList;
};

template<typename PRED, typename NODE_1, typename NODE_2>
struct Exclusive {
   using PredType = PRED;
   using NodeList = Unique_tl<typename NodeTrait<NODE_1>::NodeList::template concat<typename NodeTrait<NODE_2>::NodeList>>;
};

template<typename COND, typename NODE_1, typename NODE_2>
struct NodeTrait<Exclusive<COND, NODE_1, NODE_2>> {
   using NodeList = typename Exclusive<COND, NODE_1, NODE_2>::NodeList;
};

#endif //GRAPH_DSL_NODETRAIT_H
