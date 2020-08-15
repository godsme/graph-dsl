//
// Created by Darwin Yuan on 2020/8/15.
//

#ifndef GRAPH_PORT_DESC_H
#define GRAPH_PORT_DESC_H

#include <graph/graph_ns.h>
#include <graph/status.h>
#include <vector>
#include <graph/core/graph_context.h>

GRAPH_DSL_NS_BEGIN

enum class image_type : uint8_t {
   RGB,
   YUV
};

struct port_format {
   uint32_t   width_;
   uint32_t   height_;
   image_type image_type_;
};

template <typename PORT>
struct port_desc {
   auto build(graph_context& context) -> status_t {
      return PORT::get_format(context, format_);
   }

private:
   port_format format_{};
};

GRAPH_DSL_NS_END

#endif //GRAPH_PORT_DESC_H
