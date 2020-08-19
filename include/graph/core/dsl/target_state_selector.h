//
// Created by Darwin Yuan on 2020/8/19.
//

#ifndef GRAPH_TARGET_STATE_SELECTOR_H
#define GRAPH_TARGET_STATE_SELECTOR_H

#include <graph/graph_ns.h>
#include <graph/util/interval.h>
#include <type_traits>
#include <tuple>

GRAPH_DSL_NS_BEGIN

///////////////////////////////////////////////////////////////////////////////
template<typename DEVICE>
struct preview_tag {
   constexpr static bool Is_Preview = true;
   using device_type = DEVICE;
};

///////////////////////////////////////////////////////////////////////////////
template<typename DEVICE, typename = void>
struct device_trait {
   constexpr static bool Is_Preview = false;
   using device_type = DEVICE;
};

template<typename DEVICE>
struct device_trait<DEVICE, std::enable_if_t<DEVICE::Is_Preview>> : DEVICE {};

///////////////////////////////////////////////////////////////////////////////
template<typename ... DEVICEs>
struct device_state {
   using type = std::tuple<device_trait<DEVICEs>...>;
};

template<typename INTERVAL>
struct scope_1 {
};

///////////////////////////////////////////////////////////////////////////////
template<typename ... CONDs>
struct target_select_condition {
};

GRAPH_DSL_NS_END

#define __g_PREVIEW(device) GRAPH_DSL_NS::preview_tag<device>
#define __g_STATE(...)      GRAPH_DSL_NS::device_state<__VA_ARGS__>

#define __g_WHEN(...)       auto (GRAPH_DSL_NS::target_select_condition<__VA_ARGS__>)

#define __g_SCOPE_1(...)    GRAPH_DSL_NS::scope_1<__CUB_interval(__VA_ARGS__)>

#endif //GRAPH_TARGET_STATE_SELECTOR_H
