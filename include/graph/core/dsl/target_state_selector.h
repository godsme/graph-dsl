//
// Created by Darwin Yuan on 2020/8/19.
//

#ifndef GRAPH_TARGET_STATE_SELECTOR_H
#define GRAPH_TARGET_STATE_SELECTOR_H

#include <graph/graph_ns.h>
#include <graph/util/interval.h>
#include <type_traits>
#include <tuple>
#include <boost/hana.hpp>
#include <graph/util/hana_tuple_trait.h>

GRAPH_DSL_NS_BEGIN

namespace hana = boost::hana;

struct device_info {
   bool is_preview;
   uint8_t device_id;
};

///////////////////////////////////////////////////////////////////////////////
template<typename DEVICE>
struct preview_tag {
   constexpr static auto Device_Id = DEVICE::Device_Id;
   constexpr static auto Is_Preview = true;
};

///////////////////////////////////////////////////////////////////////////////
template<typename DEVICE, typename = void>
struct device_trait {
   constexpr static auto Device_Id = DEVICE::Device_Id;
   constexpr static auto Is_Preview = false;
};

template<typename DEVICE>
struct device_trait<DEVICE, std::enable_if_t<DEVICE::Is_Preview>> : DEVICE {};

///////////////////////////////////////////////////////////////////////////////
template<typename ... DEVICEs>
struct device_state {
   constexpr static size_t Num_Of_Devices = sizeof...(DEVICEs);
   constexpr static device_info Devices[] = {
      {device_trait<DEVICEs>::Is_Preview, device_trait<DEVICEs>::Device_Id}...
   };
};

template<uint8_t SCENE>
struct scene {
   template<typename DICT>
   auto matches(const DICT& dict) -> bool {
      return dict.get_scene() == SCENE;
   }
};

template<typename INTERVAL>
struct condition_1 {
   template<typename DICT>
   inline static auto matches(const DICT& dict) -> bool {
      return INTERVAL::contains(dict.get_condition_1());
   }
};

template<typename INTERVAL>
struct condition_2 {
   template<typename DICT>
   inline static auto matches(const DICT& dict) -> bool {
      return INTERVAL::contains(dict.get_condition_2());
   }
};

template<typename INTERVAL>
struct condition_3 {
   template<typename DICT>
   inline static auto matches(const DICT& dict) -> bool {
      return INTERVAL::contains(dict.get_condition_3());
   }
};

///////////////////////////////////////////////////////////////////////////////
template<typename ... CONDs>
struct state_select_condition {
   constexpr static size_t Num_Of_Conditions = sizeof...(CONDs);
   template<typename DICT>
   inline static auto matches(const DICT& dict) -> bool {
      return (CONDs::matches(dict) && ...);
   }
};

///////////////////////////////////////////////////////////////////////////////
template<typename PAIR>
struct target_state_entry;

template<typename COND, typename STATE>
struct target_state_entry<auto (COND) -> STATE> {
   constexpr static size_t Num_Of_Conditions = COND::Num_Of_Conditions;
   template<typename DICT>
   inline static auto return_if_matches(const DICT& dict, device_info*& devices, size_t& size) -> bool {
      if(COND::matches(dict)) {
         devices = STATE::Devices;
         size = STATE::Num_Of_Devices;
         return true;
      }
      return false;
   }
};

///////////////////////////////////////////////////////////////////////////////
template<typename ... ENTRIES>
struct target_state_selector {
private:
   constexpr static auto Sorted_Entries =
      hana::sort(hana::tuple_t<target_state_entry<ENTRIES>...>, [](auto l, auto r) {
      return decltype(l)::type::Num_Of_Conditions < decltype(r)::type::Num_Of_Conditions;
   });

   template<typename ... Entries>
   struct sorted_entries {
      template<typename DICT>
      static auto find(const DICT& dict, device_info*& devices, size_t& size) {
         return (Entries::return_if_matches(dict, devices, size) || ...);
      }
   };

   using sorted_entries_t = hana_tuple_trait_t<decltype(Sorted_Entries), sorted_entries>;

public:
   using sorted_entries_t::find;
};

GRAPH_DSL_NS_END

#define __g_WHEN(...)           auto (GRAPH_DSL_NS::state_select_condition<__VA_ARGS__>)
#define __g_SCENE(value)        GRAPH_DSL_NS::scene<value>
#define __g_COND_1(...)         GRAPH_DSL_NS::condition_1<__CUB_interval(__VA_ARGS__)>
#define __g_COND_2(...)         GRAPH_DSL_NS::condition_2<__CUB_interval(__VA_ARGS__)>
#define __g_COND_3(...)         GRAPH_DSL_NS::condition_3<__CUB_interval(__VA_ARGS__)>
#define __g_PREVIEW(device)     GRAPH_DSL_NS::preview_tag<device>
#define __g_STATE(...)          GRAPH_DSL_NS::device_state<__VA_ARGS__>
#define __g_STATE_SELECTOR(...) GRAPH_DSL_NS::target_state_selector<__VA_ARGS__>

#endif //GRAPH_TARGET_STATE_SELECTOR_H
