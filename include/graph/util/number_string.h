//
// Created by Darwin Yuan on 2020/8/19.
//

#ifndef GRAPH_NUMBER_STRING_H
#define GRAPH_NUMBER_STRING_H

#include <graph/graph_ns.h>
#include <type_traits>
#include <tuple>

GRAPH_DSL_NS_BEGIN

namespace num_s {
   constexpr auto int_power(size_t size) {
      unsigned long result = 1;
      for(size_t i = 0; i < size; i++) result *= 10;
      return result;
   }

   template<char ... CHs>
   struct char_to_int {
      constexpr static unsigned long value = 0;
   };

   template<char H, char ... CHs>
   struct char_to_int<H, CHs...> {
      constexpr static unsigned long h_value = H - '0';
      constexpr static unsigned long value =  h_value * int_power(sizeof...(CHs)) + char_to_int<CHs...>::value;
   };

   constexpr auto decimal_power(size_t size) -> double {
      // XXX: relax the float precision problem.
      switch(size) {
         case 0 : return 1.0;
         case 1 : return 0.1;
         case 2 : return 0.01;
         case 3 : return 0.001;
         case 4 : return 0.0001;
         case 5 : return 0.00001;
         default : break;
      }
      double result = 0.00001;
      for(size_t i = 0; i < size - 5; i++) result *= 0.1;
      return result;
   }

   template<char ... CHs>
   struct char_list {
      template<char CH> using append = char_list<CHs..., CH>;
      constexpr static unsigned long to_int = char_to_int<CHs...>::value;
      constexpr static double to_decimal = decimal_power(sizeof...(CHs)) * to_int;
   };

   template<typename INT_PART, char ... CHs>
   struct number_parser {
      using type = std::pair<INT_PART, char_list<>>;
   };

   template<typename INT_PART, char ... CHs>
   struct number_parser<INT_PART, '.', CHs...> {
      using type = std::pair<INT_PART, char_list<CHs...>>;
   };

   template<typename INT_PART, char H, char ... CHs>
   struct number_parser<INT_PART, H, CHs...> {
      using type = typename number_parser<typename INT_PART::template append<H>, CHs...>::type;
   };

   template<char ... CHs>
   struct number_trait {
      using result = typename number_parser<char_list<>, CHs...>::type;
      constexpr static double double_value = std::tuple_element_t<0, result>::to_int + std::tuple_element_t<1, result>::to_decimal;
      constexpr static double int_value = std::tuple_element_t<0, result>::to_int;
   };

   /////////////////////////////////////////////////////////////////////////////////////
   struct open_interval_signature {};
   struct number_string_signature {};

   template<char ... CHs>
   constexpr auto Num_Of_Dots = (((CHs == '.') ? 1 : 0) + ...);

   template<char CH>
   constexpr auto Is_Digit_Char = CH >= '0' && CH <= '9';

   template<char ... CHs>
   constexpr auto Is_Digit_Chars = (sizeof...(CHs) > 0) && (Is_Digit_Char<CHs> && ...);

   template<char ... CHs>
   constexpr auto Is_Num_Chars = (sizeof...(CHs) > 0) && (((CHs == '.' || Is_Digit_Char<CHs>)) && ...);

   template<char ... CHs>
   constexpr auto Is_Double_String = Is_Num_Chars<CHs...> &&  (Num_Of_Dots<CHs...> == 1);


   template<typename = void, char ... CHs>
   struct number_string;

   template<char ... CHs>
   struct number_string<std::enable_if_t<Is_Double_String<CHs...>>, '-', CHs...> : number_string_signature {
      constexpr static auto value = -1.0 * number_trait<CHs...>::double_value;
   };

   template<char ... CHs>
   struct number_string<std::enable_if_t<Is_Double_String<CHs...>>, '+', CHs...> : number_string_signature {
      constexpr static auto value = number_trait<CHs...>::double_value;
   };

   template<char ... CHs>
   struct number_string<std::enable_if_t<Is_Double_String<CHs...>>, CHs...> : number_string_signature {
      constexpr static auto value = number_trait<CHs...>::double_value;
   };

   template<char ... CHs>
   struct number_string<std::enable_if_t<Is_Digit_Chars<CHs...>>, '-', CHs...> : number_string_signature {
      constexpr static auto value = -1 * number_trait<CHs...>::int_value;
   };

   template<char ... CHs>
   struct number_string<std::enable_if_t<Is_Digit_Chars<CHs...>>, '+', CHs...> : number_string_signature {
      constexpr static auto value = number_trait<CHs...>::int_value;
   };

   template<char ... CHs>
   struct number_string<std::enable_if_t<Is_Digit_Chars<CHs...>>, CHs...> : number_string_signature {
      constexpr static auto value = number_trait<CHs...>::int_value;
   };

   template<>
   struct number_string<void, 'o', 'p', 'e', 'n', '_', 'i'> : open_interval_signature {};
}

GRAPH_DSL_NS_END

_Pragma("GCC diagnostic push")
#pragma GCC diagnostic ignored "-Wgnu-string-literal-operator-template"
template <typename T, T ... CHs>
constexpr auto operator "" _number_str() -> GRAPH_DSL_NS::num_s::number_string<void, CHs...> { return {}; }
_Pragma("GCC diagnostic pop")

#define __CUB_number_string(s) decltype(#s ##_number_str)


#endif //GRAPH_NUMBER_STRING_H
