//
// Created by Darwin Yuan on 2020/8/14.
//

#ifndef GRAPH_NODE_DESC_H
#define GRAPH_NODE_DESC_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/graph_context.h>
#include <graph/core/link_desc.h>
#include <tuple>
#include <vector>
#include <boost/hana.hpp>
#include <graph/function/unique.h>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

template<typename NODE, typename ... LINKS>
struct node_desc final {
   using node_type = NODE;
   constexpr static auto direct_decedents =
      unique(hana::flatten(hana::make_tuple(link_desc<LINKS>::node_list...)));
};

GRAPH_DSL_NS_END

#define __node(...) GRAPH_DSL_NS::node_desc<__VA_ARGS__>
#define __root(...) GRAPH_DSL_NS::node_desc<true,  __VA_ARGS__>

#endif //GRAPH_NODE_DESC_H
