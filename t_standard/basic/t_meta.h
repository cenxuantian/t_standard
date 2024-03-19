#pragma once
#include <type_traits>
#include <utility>
#include "t_int.h"
#include "t_float.h"

namespace tcx
{

template<typename T,typename = void>
constexpr bool __has_opr_smaller_v = false;

template<typename T>
constexpr bool __has_opr_smaller_v<T,std::void_t<decltype(std::declval<T&>() < std::declval<T&>())>> = true;

template<typename T,typename = void>
constexpr bool __has_opr_smaller_eq_v = false;

template<typename T>
constexpr bool __has_opr_smaller_eq_v<T,std::void_t<decltype(std::declval<T&>() <= std::declval<T&>())>> = true;

template<typename T,typename = void>
constexpr bool __has_opr_bigger_v = false;

template<typename T>
constexpr bool __has_opr_bigger_v<T,std::void_t<decltype(std::declval<T&>() > std::declval<T&>())>> = true;

template<typename T,typename = void>
constexpr bool __has_opr_bigger_eq_v = false;

template<typename T>
constexpr bool __has_opr_bigger_eq_v<T,std::void_t<decltype(std::declval<T&>() >= std::declval<T&>())>> = true;

template<typename T,typename = void>
constexpr bool __has_opr_eq_v = false;

template<typename T>
constexpr bool __has_opr_eq_v<T,std::void_t<decltype(std::declval<T&>() == std::declval<T&>())>> = true;

template<typename T,typename = void>
constexpr bool __has_opr_ineq_v = false;

template<typename T>
constexpr bool __has_opr_ineq_v<T,std::void_t<decltype(std::declval<T&>() != std::declval<T&>())>> = true;

template<typename T>
constexpr bool __is_eq_comparable_v = __has_opr_ineq_v<T> || __has_opr_eq_v<T>;

// check if a template class has a content type
template<typename T,typename = void>
constexpr bool __has_content_type_v = false;
template<typename T>
constexpr bool __has_content_type_v<T,std::void_t<typename T::ContentType>> = true;

template<typename T,typename ...Args>
constexpr bool __belongs_to_v = false;

template<typename T, typename _1, typename ...Args>
constexpr bool __belongs_to_v<T(_1,Args...)> = __belongs_to_v<T(_1)> || __belongs_to_v<T(Args...)>;

template<typename T,typename _1>
constexpr bool __belongs_to_v<T(_1)> = std::is_same_v<T,_1>;



template<typename _First, typename ... Types>
constexpr bool __completely_moveable_v = __completely_moveable_v<_First> && __completely_moveable_v<Types...>;

template<typename _First>
constexpr bool __completely_moveable_v<_First> = std::is_move_assignable_v<_First> && std::is_move_constructible_v<_First>;


template<typename T>
constexpr bool __is_ref_faster_v = !std::is_pointer_v<std::decay_t<T>>;
template<> constexpr bool __is_ref_faster_v<u8> = false;
template<> constexpr bool __is_ref_faster_v<i8> = false;
template<> constexpr bool __is_ref_faster_v<u16> = false;
template<> constexpr bool __is_ref_faster_v<i16> = false;
template<> constexpr bool __is_ref_faster_v<u32> = false;
template<> constexpr bool __is_ref_faster_v<i32> = false;
template<> constexpr bool __is_ref_faster_v<u64> = false;
template<> constexpr bool __is_ref_faster_v<i64> = false;
template<> constexpr bool __is_ref_faster_v<f32> = false;
template<> constexpr bool __is_ref_faster_v<f64> = false;


template<typename T>
using __Faster_func_param_t = std::conditional_t<std::is_reference_v<T>,T const&,std::conditional_t<__is_ref_faster_v<std::decay_t<T>>,T const&, T>>;

} // namespace tcx
