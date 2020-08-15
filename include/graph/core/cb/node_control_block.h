//
// Created by Darwin Yuan on 2020/8/15.
//

#ifndef GRAPH_NODE_CONTROL_BLOCK_H
#define GRAPH_NODE_CONTROL_BLOCK_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <graph/core/graph_context.h>
#include <graph/util/tuple_element_by_type.h>
#include <tuple>
#include <vector>

GRAPH_DSL_NS_BEGIN

struct link_desc;

struct node_control_block {
   virtual auto build(graph_context& context) -> status_t = 0;
   virtual ~node_control_block() = default;
};

template <typename NODE>
struct node_control_block_base : node_control_block {
   using node_type = NODE;
   using link_set = std::vector<link_desc*>;

protected:
   auto build_node(graph_context& context) -> status_t {
      refs_++;
      return status_t::Ok;
   }

private:
   uint8_t refs_{0};
   bool running_{false};
};

template<typename NODE, typename ... LINKs>
struct actual_node_control_block : node_control_block_base<NODE> {
private:
   using parent = node_control_block_base<NODE>;
   using link_set = std::vector<link_desc*>;
   constexpr static auto Index_Seq = std::make_index_sequence<sizeof...(LINKs)>{};

public:
   auto build(graph_context& context) -> status_t {
      typename parent::link_set alive_links(sizeof...(LINKs));
      GRAPH_EXPECT_SUCC(build_links(context, alive_links, Index_Seq));
      GRAPH_EXPECT_SUCC(parent::build_node(context));

      return status_t::Ok;
   }

private:
   template<size_t ... I>
   auto build_links(graph_context& context, link_set& links, std::index_sequence<I...>) -> status_t {
      status_t status = status_t::Ok;
      (((status = std::get<I>(links_).build(context, links)) == status_t::Ok) && ...);
      return status;
   }

private:
   link_set link_set_;
   std::tuple<LINKs...> links_;
};

template<typename NODE>
struct actual_node_control_block<NODE> : node_control_block_base<NODE> {
   using parent = node_control_block_base<NODE>;
   auto build(graph_context& context) -> status_t override {
      return parent::build_node(context);
   }
};

GRAPH_DSL_NS_END

#endif //GRAPH_NODE_CONTROL_BLOCK_H
