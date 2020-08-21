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
   const bool is_preview;
   const uint8_t device_id;

   inline constexpr auto tie() const noexcept {
      return std::tie(device_id, is_preview);
   }
};

inline constexpr auto operator==(device_info const& lhs, device_info const& rhs) noexcept -> bool {
   return lhs.tie() == rhs.tie();
}

inline constexpr auto operator!=(device_info const& lhs, device_info const& rhs) noexcept -> bool {
   return !operator==(lhs, rhs);
}

inline constexpr auto operator<(device_info const& lhs, device_info const& rhs) noexcept -> bool {
   return lhs.tie() < rhs.tie();
}

inline constexpr auto operator<=(device_info const& lhs, device_info const& rhs) noexcept -> bool {
   return lhs.tie() <= rhs.tie();
}

inline constexpr auto operator>(device_info const& lhs, device_info const& rhs) noexcept -> bool {
   return lhs.tie() > rhs.tie();
}

inline constexpr auto operator>=(device_info const& lhs, device_info const& rhs) noexcept -> bool {
   return lhs.tie() >= rhs.tie();
}

struct root_state {
   device_info const* device_info;
   size_t size;
};

inline constexpr auto operator==(root_state const& lhs, root_state const& rhs) noexcept -> bool {
   if(lhs.size != rhs.size) return false;
   for(auto i=0; i<lhs.size; i++) {
      if(lhs.device_info[i] != rhs.device_info[i]) return false;
   }
   return true;
}

inline constexpr auto operator!=(root_state const& lhs, root_state const& rhs) noexcept -> bool {
   return !operator==(lhs, rhs);
}

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
   constexpr static auto Sorted_Devices =
      hana::sort(hana::tuple_t<device_trait<DEVICEs>...>, [](auto l, auto r) {
      return hana::char_c<decltype(l)::type::Device_Id> < hana::char_c<decltype(r)::type::Device_Id>;
   });

   template<typename ... Ts>
   struct devices_type {
      constexpr static device_info Devices[] = {
         {Ts::Is_Preview, Ts::Device_Id}...
      };
      constexpr static root_state Root_State { .device_info = Devices, .size = Num_Of_Devices };
   };

   using devices = hana_tuple_trait_t<decltype(Sorted_Devices), devices_type>;
   constexpr static auto Devices = devices::Devices;

   constexpr static auto Root_State = devices::Root_State;

   template<typename DEVICE>
   inline static constexpr auto content_equal() -> bool {
      for(size_t i=0; i<Num_Of_Devices; i++) {
         if (Devices[i] != DEVICE::Devices[i]) {
            return false;
         }
      }
      return true;
   }

   template<typename DEVICE>
   inline static constexpr auto equals() noexcept {
      if constexpr (Num_Of_Devices != DEVICE::Num_Of_Devices) {
         return hana::integral_constant<bool, false>{};
      } else {
         if constexpr (content_equal<DEVICE>()) {
            return hana::integral_constant<bool, true>{};
         } else {
            return hana::integral_constant<bool, false>{};
         }
      }
   }

   template<typename DEVICE>
   inline static constexpr auto content_less_than() -> bool {
      for(size_t i=0; i<Num_Of_Devices; i++) {
         if (Devices[i] >= DEVICE::Devices[i]) {
            return false;
         }
      }
      return true;
   }

   template<typename DEVICE>
   inline static constexpr auto less_than() noexcept {
      if constexpr (Num_Of_Devices < DEVICE::Num_Of_Devices) {
         return hana::integral_constant<bool, true>{};
      } else if constexpr(Num_Of_Devices > DEVICE::Num_Of_Devices) {
         return hana::integral_constant<bool, false>{};
      } else {
         if constexpr (content_less_than<DEVICE>()) {
            return hana::integral_constant<bool, true>{};
         } else {
            return hana::integral_constant<bool, false>{};
         }
      }
   }
};


template<uint8_t SCENE>
struct scene {
   template<typename DICT>
   static auto matches(const DICT& dict) -> bool {
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
   inline static auto return_if_matches(const DICT& dict, root_state& result) -> bool {
      if(COND::matches(dict)) {
         result = STATE::Root_State;
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
      return hana::size_c<decltype(l)::type::Num_Of_Conditions> > hana::size_c<decltype(r)::type::Num_Of_Conditions>;
   });

public:
   template<typename ... Entries>
   struct entries_type {
      template<typename DICT>
      static auto find(const DICT& dict) {
         root_state result;
         (Entries::return_if_matches(dict, result) || ...);
         return result;
      }
   };

   using sorted_entries = hana_tuple_trait_t<decltype(Sorted_Entries), entries_type>;
};

GRAPH_DSL_NS_END

#define __g_WHEN(...)           auto (GRAPH_DSL_NS::state_select_condition<__VA_ARGS__>)
#define __g_SCENE_MODE(value)   GRAPH_DSL_NS::scene<value>
#define __g_COND_1(...)         GRAPH_DSL_NS::condition_1<__CUB_interval(__VA_ARGS__)>
#define __g_COND_2(...)         GRAPH_DSL_NS::condition_2<__CUB_interval(__VA_ARGS__)>
#define __g_COND_3(...)         GRAPH_DSL_NS::condition_3<__CUB_interval(__VA_ARGS__)>
#define __g_PREVIEW(device)     GRAPH_DSL_NS::preview_tag<device>
#define __g_STATE(...)          GRAPH_DSL_NS::device_state<__VA_ARGS__>
#define __g_STATE_SELECTOR(...) GRAPH_DSL_NS::target_state_selector<__VA_ARGS__>

#endif //GRAPH_TARGET_STATE_SELECTOR_H
