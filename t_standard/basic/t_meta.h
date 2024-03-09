#pragma once
#include <type_traits>
#include <utility>
namespace tcx
{

template<typename T,typename = void>
constexpr bool __has_opr_smaller = false;

template<typename T>
constexpr bool __has_opr_smaller<T,std::void_t<decltype(std::declval<T&>() < std::declval<T&>())>> = true;

template<typename T,typename = void>
constexpr bool __has_opr_smaller_eq = false;

template<typename T>
constexpr bool __has_opr_smaller_eq<T,std::void_t<decltype(std::declval<T&>() <= std::declval<T&>())>> = true;

template<typename T,typename = void>
constexpr bool __has_opr_bigger = false;

template<typename T>
constexpr bool __has_opr_bigger<T,std::void_t<decltype(std::declval<T&>() > std::declval<T&>())>> = true;

template<typename T,typename = void>
constexpr bool __has_opr_bigger_eq = false;

template<typename T>
constexpr bool __has_opr_bigger_eq<T,std::void_t<decltype(std::declval<T&>() >= std::declval<T&>())>> = true;

template<typename T,typename = void>
constexpr bool __has_opr_eq = false;

template<typename T>
constexpr bool __has_opr_eq<T,std::void_t<decltype(std::declval<T&>() == std::declval<T&>())>> = true;

template<typename T,typename = void>
constexpr bool __has_opr_ineq = false;

template<typename T>
constexpr bool __has_opr_ineq<T,std::void_t<decltype(std::declval<T&>() != std::declval<T&>())>> = true;

template<typename T>
constexpr bool __is_eq_comparable = __has_opr_ineq<T> || __has_opr_eq<T>;

// check if a template class has a content type
template<typename T,typename = void>
constexpr bool __has_content_type = false;
template<typename T>
constexpr bool __has_content_type<T,std::void_t<typename T::ContentType>> = true;

template<typename T,typename ...Args>
constexpr bool __belongs_to = false;

template<typename T, typename _1, typename ...Args>
constexpr bool __belongs_to<T(_1,Args...)> = __belongs_to<T(_1)> || __belongs_to<T(Args...)>;

template<typename T,typename _1>
constexpr bool __belongs_to<T(_1)> = std::is_same_v<T,_1>;



template<typename _First, typename ... Types>
constexpr bool __completely_moveable = __completely_moveable<_First> && __completely_moveable<Types...>;

template<typename _First>
constexpr bool __completely_moveable<_First> = std::is_move_assignable_v<_First> && std::is_move_constructible_v<_First>;


} // namespace tcx
