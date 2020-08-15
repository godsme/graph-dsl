//
// Created by Darwin Yuan on 2020/8/15.
//

#ifndef GRAPH_EITHER_H
#define GRAPH_EITHER_H

#include <graph/graph_ns.h>
#include <graph/util/ptr_trait.h>
#include <utility>
#include <variant>

GRAPH_DSL_NS_BEGIN

template<typename L, typename R>
class either : private std::variant<L, R> {
   using parent = std::variant<L, R>;
public:
   struct left_tag {};
   struct right_tag{};

   constexpr either(L&& value) noexcept
      : parent{std::move(value)}
   {}

   constexpr either(R&& value) noexcept
      : parent{std::move(value)}
   {}

   constexpr either(const L& value) noexcept
      : parent{value}
   {}

   constexpr either(const R& value) noexcept
      : parent{value}
   {}

   template<typename ... Args>
   constexpr either(left_tag, Args&&...args)
      : parent{std::in_place_index_t<0>{}, std::forward<Args>(args)...}
   {}

   template<typename ... Args>
   constexpr either(right_tag, Args&&...args)
      : parent{std::in_place_index_t<1>{}, std::forward<Args>(args)...}
   {}

   constexpr either(const either& other)
      : parent {other}
   {}

   constexpr either(either&& other) noexcept
      : parent {std::move(other)}
   {}

   template<typename T, typename ... Args>
   inline constexpr auto emplace(Args&& ... args) -> T& {
      return parent::template emplace<T>(std::forward<Args>(args)...);
   }

   template<typename ... Args>
   inline constexpr auto emplace_left(Args&& ... args) -> L& {
      return parent::template emplace<0>(std::forward<Args>(args)...);
   }

   template<typename ... Args>
   inline constexpr auto emplace_right(Args&& ... args) -> R& {
      return parent::template emplace<1>(std::forward<Args>(args)...);
   }

   inline constexpr auto is_left() const noexcept -> bool {
      return parent::index() == 0;
   }

   inline constexpr auto is_right() const noexcept -> bool {
      return parent::index() == 1;
   }

   inline constexpr auto left() -> L& {
      return std::get<0>(*this);
   }

   inline constexpr auto right() -> R& {
      return std::get<1>(*this);
   }

   inline constexpr auto left() const -> const L& {
      return std::get<0>(*this);
   }

   inline constexpr auto left_or_else(const L& default_l) const -> const L& {
      return is_left() ? left() : default_l;
   }

private:
   template<typename T>
   struct is_either_r_type {
      constexpr static bool value = false;
   };

   template<typename T>
   struct is_either_r_type<either<T, R>> {
      constexpr static bool value = true;
   };

   template<typename T>
   constexpr static bool is_either_r_type_v = is_either_r_type<T>::value;

   template<typename F>
   using l_invoke_result = std::invoke_result_t<F, const L&>;

public:

   template<typename F, typename = std::enable_if_t<
      std::is_invocable_v<F, const L&> &&
      ( is_either_r_type_v<l_invoke_result<F>> || std::is_same_v<l_invoke_result<F>, R>)>>
   inline constexpr auto left_match(F&& f) const -> std::invoke_result_t<F, const L&> {
      return is_left() ? f(left()) : right();
   }

   inline constexpr auto right() const -> const R& {
      return std::get<1>(*this);
   }

   inline constexpr auto right_or_else(const R& default_r) const -> const R& {
      return is_right() ? right() : default_r;
   }

private:
   template<typename T>
   struct is_either_l_type {
      constexpr static bool value = false;
   };

   template<typename T>
   struct is_either_l_type<either<L, T>> {
      constexpr static bool value = true;
   };

   template<typename T>
   constexpr static bool is_either_l_type_v = is_either_l_type<T>::value;

   template<typename F>
   using r_invoke_result = std::invoke_result_t<F, const R&>;

public:

   template<typename F, typename = std::enable_if_t<
      std::is_invocable_v<F, const R&> &&
      ( is_either_l_type_v<r_invoke_result<F>> || std::is_same_v<r_invoke_result<F>, L>)>>
   inline constexpr auto right_match(F&& f) const -> std::invoke_result_t<F, const R&> {
      return is_left() ? left() : f(right());
   }

   template<typename F_L, typename F_R>
   constexpr auto match(F_L&& f_l, F_R&& f_r) const noexcept {
      static_assert(std::is_invocable_v<F_L, const L&>, "f_left type mismatch");
      static_assert(std::is_invocable_v<F_R, const R&>, "f_right type mismatch");

      using type_l = std::invoke_result_t<F_L, const L&>;
      using type_r = std::invoke_result_t<F_R, const R&>;
      if constexpr(std::is_null_pointer_v<type_l>) {
         static_assert(is_ptr_v<type_r>, "result type mismatch");
         return is_left() ? static_cast<type_r>(f_l(left())) : f_r(right());
      } else if constexpr(std::is_null_pointer_v<type_r>) {
         static_assert(is_ptr_v<type_l>, "result type mismatch");
         return is_left() ? f_l(left()) : static_cast<type_l>(f_r(right()));
      } else {
         static_assert(std::is_same_v<type_r, type_r>, "result type mismatch");
         return is_left() ? f_l(left()) : f_r(right());
      }
   }

   friend inline bool operator==(const either& lhs, const either& rhs) noexcept {
      return static_cast<const parent&>(lhs) == static_cast<const parent&>(rhs);
   }

   friend inline bool operator!=(const either& lhs, const either& rhs) noexcept {
      return !operator==(lhs, rhs);
   }

   friend inline bool operator<(const either& lhs, const either& rhs) noexcept {
      return static_cast<const parent&>(lhs) < static_cast<const parent&>(rhs);
   }

   friend inline bool operator<=(const either& lhs, const either& rhs) noexcept {
      return static_cast<const parent&>(lhs) <= static_cast<const parent&>(rhs);
   }

   friend inline bool operator>(const either& lhs, const either& rhs) noexcept {
      return static_cast<const parent&>(lhs) > static_cast<const parent&>(rhs);
   }

   friend inline bool operator>=(const either& lhs, const either& rhs) noexcept {
      return static_cast<const parent&>(lhs) >= static_cast<const parent&>(rhs);
   }
};

GRAPH_DSL_NS_END

#endif //GRAPH_EITHER_H
